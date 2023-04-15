#include <pthread.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/time.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <typeinfo>

// Boost libs
//#include <boost/thread.hpp>
//#include <boost/thread/mutex.hpp>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include "pfr_dst.h"
#include "pfr_dst_list.h"
#include "pfr_peers.h"
#include "pfr_rdata.h"
#include "pfr_rtr.h"
#include "ping0.h"
#include "pfr_sql_log.h"

using namespace std;

/*
typedef struct {
 int peer_id;
 int probe_id;
 pfr_dst_list *dst_list;
} idata;
*/

bool sig_usr1_flag = false;
bool sig_usr2_flag = false;
bool sig_int_flag = false;

static void sig_usr(int signo) {
    if (signo == SIGUSR1 || signo == SIGINT) {
        sig_usr1_flag = true;
        sig_int_flag = true;
    } else if (signo == SIGUSR2) {
        sig_usr2_flag = true;
    }
}

void *send_req(void *pin) {
 pthread_t thId = pthread_self();

 idata *in = (idata*)pin;
 send_v4(in, thId);
 return (void *)in;
}


int do_fork() {
    int status = 0;
    switch (fork()) {
        case 0:
            break;
        case -1:
            status = -1;
            break;
    default:
        // We should close master process with _exit(0)
        // We should not call exit() because it will destroy all global variables for programm
            _exit(0);
    }

    return status;
}

void print_rdata();

//pfr_dst_list pfrList(10);
//pfr_dst_list pfrList(10, 10);
pfr_dst_list pfrL;
pfr_dst_list &pfrList = pfrL;

//       |dsp_ip               |probe_id     |peer_id      |seq_num
std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>> r;
//       |dsp_ip               |probe_id
std::map<std::string, std::map<int, rt_parm *>> route;

//       |dsp_ip               |probe_id     |peer_id
std::map<std::string, std::map<int, std::map<int,  tlog *>>> route_log0;
std::map<std::string, std::map<int, tlog *>> route_log1;

int proc_v4_new_cnt = 0;
int avg_rtt_new_cnt = 0;
int scan_new_cnt = 0;

int del_proc_v4_new_cnt = 0;
int del_avg_rtt_new_cnt = 0;
int del_scan_new_cnt = 0;
int printr_new_cnt = 0;

pthread_mutex_t mtr;

int send_stopped = 0;
int req_stopped = 0;
pthread_mutex_t mt_req_send;

std::shared_ptr<spdlog::logger> syslog_logger;

// conf parameters
int pfr_ping_req = 5; //config_t, pfr_rdata.cpp, ping_send_v4.cpp
long int max_time_of_echo = 1200; // max time secs spent to send echo. config_t
int sleep_after_join = 6;
int sleep_after_update = 30;
int sleep_before_next_loop = 20;
bool enable_conection_tracking = true;
std::string pid_path;
int deep_delete = 3; //config_t, pfr_rdata.cpp
double min_rtt = 50000; //config_t, pfr_data.cpp
std::string src_addr = "1.0.5.230"; //config_t, ping_send_v4.cpp
int usleep_between_echo = 2500; //config_t, ping_send_v4.cpp
std::string gobgp_path;
std::string localnets;
int max_load = 0;
int df = 0;
int dscp = 0; 
std::string log_level = "debug";
bool enable_advertise = true;
bool enable_sql_log = true;

// conf parameters for postgres
std::string pghost = "127.0.0.1";
std::string pgport = "5432";
std::string db_name = "vc";
std::string login = "vc";
std::string pwd = "vc";


// Global map with parsed config file
//typedef std::map<std::string, std::string> configuration_map_t;
//configuration_map_t configuration_map;
std::map<std::string, std::string> configuration_map;
std::string global_config_path = "/etc/pfrd.conf";

// this part of code was stolen from fastnetmon

// convert string to integer
int convert_string_to_integer(std::string line) {
     return atoi(line.c_str());
}

bool load_configuration_file() {
    std::ifstream config_file(global_config_path.c_str());
    std::string line;

    
    if (!config_file.is_open()) {
        syslog_logger->debug("Can't open config file");
        return false;
    }
    
    while (getline(config_file, line)) {
        std::vector<std::string> parsed_config;
    
        if (line.find("#") == 0 or line.empty()) {
            continue;
        }
    
        boost::split(parsed_config, line, boost::is_any_of(" ="), boost::token_compress_on);
    
        if (parsed_config.size() == 2) {
            configuration_map[parsed_config[0]] = parsed_config[1];
        } else {
            syslog_logger->debug("Can't parse config line: {}", line);
        }
    }
    
    if (configuration_map.count("pid_path") != 0) {
     pid_path = configuration_map["pid_path"];
    }

    if (configuration_map.count("src_addr") != 0) {
     src_addr = configuration_map["src_addr"];
    }

    if (configuration_map.count("gobgp_path") != 0) {
     gobgp_path = configuration_map["gobgp_path"];
    }

    if (configuration_map.count("localnets") != 0) {
     localnets = configuration_map["localnets"];
    }

    if (configuration_map.count("log_level") != 0) {
     log_level = configuration_map["log_level"];
    }

    if (configuration_map.count("pfr_ping_req") != 0) {
     pfr_ping_req = convert_string_to_integer(configuration_map["pfr_ping_req"]);
    }

    if (configuration_map.count("max_load") != 0) {
     max_load = convert_string_to_integer(configuration_map["max_load"]);
    }

    if (configuration_map.count("df") != 0) {
     df = convert_string_to_integer(configuration_map["df"]);
    }

    if (configuration_map.count("dscp") != 0) {
     dscp = convert_string_to_integer(configuration_map["dscp"]);
    }

    if (configuration_map.count("max_time_of_echo") != 0) {
     max_time_of_echo = convert_string_to_integer(configuration_map["max_time_of_echo"]);
    }
    
    if (configuration_map.count("deep_delete") != 0) {
     deep_delete = convert_string_to_integer(configuration_map["deep_delete"]);
    }

    if (configuration_map.count("min_rtt") != 0) {
     min_rtt = convert_string_to_integer(configuration_map["min_rtt"]);
    }

    if (configuration_map.count("usleep_between_echo") != 0) {
     usleep_between_echo = convert_string_to_integer(configuration_map["usleep_between_echo"]);
    }

    if (configuration_map.count("sleep_after_join") != 0) {
     sleep_after_join = convert_string_to_integer(configuration_map["sleep_after_join"]);
    }

    if (configuration_map.count("sleep_after_update") != 0) {
     sleep_after_update = convert_string_to_integer(configuration_map["sleep_after_update"]);
    }

    if (configuration_map.count("sleep_before_next_loop") != 0) {
     sleep_before_next_loop = convert_string_to_integer(configuration_map["sleep_before_next_loop"]);
    }

    if (configuration_map.count("pghost") != 0) {
     pghost = configuration_map["pghost"];
    }

    if (configuration_map.count("pgport") != 0) {
     pgport = configuration_map["pgport"];
    }

    if (configuration_map.count("db_name") != 0) {
     db_name = configuration_map["db_name"];
    }

    if (configuration_map.count("login") != 0) {
     login = configuration_map["login"];
    }

    if (configuration_map.count("pwd") != 0) {
     pwd = configuration_map["pwd"];
    }

    if (configuration_map.count("enable_advertise")) {
        if (configuration_map["enable_advertise"] == "on") {
            enable_advertise = true;
        } else {
            enable_advertise = false;
        }
    }

    if (configuration_map.count("enable_sql_log")) {
        if (configuration_map["enable_sql_log"] == "on") {
            enable_sql_log = true;
        } else {
            enable_sql_log = false;
        }
    }

    return true;
}

// check file existence
bool file_exists(std::string path) {
    FILE* check_file = fopen(path.c_str(), "r");
    if (check_file) {
        fclose(check_file);
        return true;
    } else {
        return false;
    }
}

bool read_pid_from_file(pid_t& pid, std::string pid_path) {
    std::fstream pid_file(pid_path.c_str(), std::ios_base::in);

    if (pid_file.is_open()) {
        pid_file >> pid;
        pid_file.close();
        return true;
    } else {
        return false;
    }
}


bool print_pid_to_file(pid_t pid, std::string pid_path) {
    std::ofstream pid_file;
    std::cout << "pid: " << pid << std::endl;
    std::cout << "pid_path: " << pid_path << std::endl;

    pid_file.open(pid_path.c_str(), std::ios::trunc);
    if (pid_file.is_open()) {
        pid_file << pid << "\n";
        pid_file.close();
        return true;
    } else {
        return false;    
    }
}


int main(int argc, char **argv) {
    //main loop 
    //data for thread peer_id, probe_id, thread_id, timestamp
    idata itdata[999];
    std::map<int, pfr_peer> m;
    pfr_peers pp(m);
    int t_ct = pp.size();
    pthread_t thrds[t_ct];
    pthread_t thrdrd;
    pthread_t thrddb;
    bool fthread = 0;
    int m_ct = 10000;
    pthread_mutex_t mtxs[m_ct];

    struct timeval tp;
    long int start_time_of_echo;
    long int end_time_of_echo;
    long int diff_time_of_echo;


    bool daemonize = false;
    namespace po = boost::program_options;
    std::string pfrd_version = "0.1";
    
    signal(SIGUSR1, sig_usr);
    signal(SIGUSR2, sig_usr);
    signal(SIGINT, sig_usr);

    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("version", "show version")
            ("daemonize", "detach from the terminal")
            ("configuration_file", po::value<std::string>(), "set path to custom configuration file")
            ;

            po::variables_map vm;
            po::store(po::parse_command_line(argc, argv, desc), vm);
            po::notify(vm);    
            if(vm.count("help")) {
                std::cout << desc << std::endl;
                exit(EXIT_SUCCESS);
            }
            
            if(vm.count("version")) {
                std::cout << "Version: " << pfrd_version << std::endl;
                exit(EXIT_SUCCESS);
            }
            
            if(vm.count("daemonize")) {
                daemonize = true;
            }


           if(vm.count("configuration_file")) {
                global_config_path = vm["configuration_file"].as<std::string>();
                std::cout << "We will use custom path to configuration file: " << global_config_path << std::endl;
           }
    } catch (po::error& e) {
                std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
                exit(EXIT_FAILURE);
    }


     // We use ideas from here https://github.com/bmc/daemonize/blob/master/daemon.c
     //
     if(daemonize) {
        int status = 0;
        printf("We will run in daemonized mode\n");
        if((status = do_fork()) < 0) {
            // fork failed
            status = -1;
        } else if (setsid() < 0) {
            // Create new session
            status = -1;
        } else if ((status = do_fork()) < 0) {
            status = -1;
        } else {
            // Clear inherited umask
            umask(0);
           // Chdir to root
           int chdir_result = chdir("/");
          // close all descriptors because we are daemon!
          //redirect_fds();
        }
     }


    /*
    std::string ident = "pfrd";
    syslog_logger = spdlog::syslog_logger_mt("pfr_syslog", ident, LOG_PID, LOG_LOCAL5);
    syslog_logger->set_level(spdlog::level::debug);
    syslog_logger->debug("pfrd was started...");
    */
    // Create a file rotating logger with 5mb size max and 3 rotated files
    int max_log_size = 1048576 * 50; //config_t
    int max_log_files = 5; //config_t
    syslog_logger = spdlog::rotating_logger_mt("pfr_syslog", "/var/log/pfrd.log", max_log_size, max_log_files);
    if (log_level.compare("debug") == 0) {
        syslog_logger->set_level(spdlog::level::debug);
    } else {
        syslog_logger->set_level(spdlog::level::info);
    }
    spdlog::flush_every(std::chrono::seconds(3)); //config_t
    
    bool load_config_result = load_configuration_file();
    
    syslog_logger->debug("");
    syslog_logger->debug("-------------------------------------------------------");
    syslog_logger->debug("pfrd was started...");
    syslog_logger->debug("global vars:");
    syslog_logger->debug("deep_delete: {}", deep_delete);
    syslog_logger->debug("min_rtt: {}", min_rtt);
    syslog_logger->debug("src_addr: {}", src_addr);
    syslog_logger->debug("max_time_of_echo: {}", max_time_of_echo);
    syslog_logger->debug("pid_file: {}", pid_path);
    syslog_logger->debug("pid: {}", getpid());
    syslog_logger->debug("gobgp_path: {}", gobgp_path);
    syslog_logger->debug("localnets: {}", localnets);
    syslog_logger->debug("pghost: {}", pghost);
    syslog_logger->debug("pgport: {}", pgport);
    syslog_logger->debug("db_name: {}", db_name);
    syslog_logger->debug("login: {}", login);
    syslog_logger->debug("pwd: {}", pwd);
    syslog_logger->debug("max_load: {}", max_load);
    syslog_logger->debug("df: {}", df);
    syslog_logger->debug("dscp: {}", dscp);
    syslog_logger->debug("log_level: {}", log_level);
    syslog_logger->debug("enable_advertise: {}", enable_advertise);
    syslog_logger->debug("enable_sql_log: {}", enable_sql_log);


    if (!load_config_result) {
        syslog_logger->debug("Can't open config file {} please create it!", global_config_path);
        exit(1);
    } 

    if (file_exists(pid_path)) {
        pid_t pid_from_file = 0;
        if (read_pid_from_file(pid_from_file, pid_path)) {
            // We could read pid
            if (pid_from_file > 0) {
                // We use signal zero for check process existence
                int kill_result = kill(pid_from_file, 0);
                if (kill_result == 0) {
                    syslog_logger->debug("pfrd is already running with pid: {}", pid_from_file);
                    exit(1);
                } else {
                    // Yes, we have pid with pid but it's zero
                }
            } else {
                // pid from file is broken, we assume tool is not running
            }
        } else {
            // We can't open file, let's assume it's broken and tool is not running
        }
    } else {
        // no pid file
    }
    
    // If we not failed in check steps we could run toolkit
    bool print_pid_to_file_result = print_pid_to_file(getpid(), pid_path);
    
    if (!print_pid_to_file_result) {
        syslog_logger->debug("Could not create pid file, please check permissions: {}", pid_path);
        syslog_logger->debug("exit(1)");
        exit(1);
    }

    pfr_sql_log sql_log = pfr_sql_log(enable_sql_log);
    thlog ithlog;
    ithlog.log = &sql_log;

    //pfr_dst_list pfrList(10);
    pfrList = pfr_dst_list(10, 10);

    //create asbr structure and connect to asbr by netconf
    pfr_asbrs br(m);

    for(;;) {
        //pfrList = pfr_dst_list(10, 10);

        static int probe_id = 0;
        int ct_data = 0;

        if(probe_id > 0) {
            //std::cout << "probe_id > 0: " << probe_id << std::endl;
            syslog_logger->debug("probe_id > 0: {}", probe_id);
            pfrList = pfr_dst_list(10, 10, pfrList);
        }

        for(int i = 0; i < t_ct; i++) {
            pthread_mutex_init(&mtxs[i], NULL);
        }

        for(std::map<int, pfr_peer>::iterator itm = m.begin(); itm != m.end(); ++itm) {
            int p_id = itm->first;
            itdata[ct_data] = (idata){ .mm = m, .peer_id = p_id, .probe_id = probe_id, .dst_list = &pfrList, .mtx = mtxs };
            ct_data++;
        }


        syslog_logger->debug("0:s Starting readloop thread...");
        if(!fthread) {
            pthread_create(&thrdrd, NULL, readloop, NULL);
            fthread = true;
        }
        syslog_logger->debug("0:e");


        gettimeofday(&tp, NULL);
        start_time_of_echo = tp.tv_sec;

        syslog_logger->debug("1:s Starting send_req threads...");
        for(int i = 0; i < ct_data; i++) {
             pthread_create(&thrds[i], NULL, send_req, &itdata[i]);
        }
        syslog_logger->debug("1:e");


        syslog_logger->debug("2:s Joining send_req threads...");
        for(int j = 0; j < ct_data; j++) {
            pthread_join(thrds[j], NULL);
        }
        syslog_logger->debug("2:e");
        gettimeofday(&tp, NULL);
        end_time_of_echo = tp.tv_sec;
        diff_time_of_echo = end_time_of_echo - start_time_of_echo;
        if(max_time_of_echo > diff_time_of_echo && probe_id > 0) {
            syslog_logger->debug("max_time_of_echo sleep({})", max_time_of_echo - diff_time_of_echo);
            sleep(max_time_of_echo - diff_time_of_echo);
        }

        pthread_mutex_lock(&mt_req_send); 
        req_stopped = 1;
        pthread_mutex_unlock(&mt_req_send); 
        syslog_logger->debug("sleep({})", sleep_after_join);
        sleep(sleep_after_join); //config_t, sleep_after_join default: 6
        
        
        pthread_mutex_lock(&mt_req_send);
        syslog_logger->debug("req_stopped({})", req_stopped);
        syslog_logger->debug("send_stopped({})", send_stopped);
        if(send_stopped == 1) {
         syslog_logger->debug("3:s Start of pfr_route_update()...");
         //pfr_route_update(probe_id, pfrList);
         pfr_route_update(probe_id, pfrList, m, br);
         syslog_logger->debug("3:e End of pfr_route_update()...");

         syslog_logger->debug("sleep({})", sleep_after_update);
         sleep(sleep_after_update); //config_t, sleep_after_update default: 30
         
         //print_rdata();

         syslog_logger->debug("4:s Start of pfr_calc_avg_rtt()...");
         pfr_calc_avg_rtt(probe_id);
         syslog_logger->debug("4:e End of pfr_calc_avg_rtt()...");

         //pfr_print_avg_rtt(probe_id);

         syslog_logger->debug("5:s Start of pfr_route_scan()...");
         pfr_route_scan(probe_id, sql_log);
         syslog_logger->debug("5:e Start of pfr_route_scan()...");
         
         syslog_logger->debug("5.1:s Starting write db thread...");
         ithlog.probe_id = probe_id;
         pthread_create(&thrddb, NULL, pfr_route_scan_sql, &ithlog);
         syslog_logger->debug("5.1:e Starting write db thread...");

         syslog_logger->debug("6:s Start of pfr_route_free()...");
         pfr_route_free(probe_id);
         syslog_logger->debug("6:e End of pfr_route_free()...");

         //pfr_route_print(probe_id);
         
         // pfr_log
         syslog_logger->debug("7:s Start of pfr_log_print()...");
         pfr_log_print(probe_id);
         syslog_logger->debug("7:e End of pfr_log_print()...");

         //start routes delete
         syslog_logger->debug("7.5:s Start of pfr_delete()...");
         pfr_delete(probe_id, m, br);
         syslog_logger->debug("7.5:e End of pfr_delete()...");

            
         // pfr graceful shutdown
         if(sig_usr1_flag || sig_int_flag) {
            syslog_logger->debug("graceful shutdown...");
            pfr_delete_all(probe_id, m, br);
            exit(1);       
         } 

         // pfr reread conf
         if(sig_usr2_flag) {
            syslog_logger->debug("reread configuration...");
            load_config_result = load_configuration_file();
            if (!load_config_result) {
                syslog_logger->debug("Can't open config file(reread) {} please create it!", global_config_path);
                exit(1);
            } 
    
            syslog_logger->debug("");
            syslog_logger->debug("-------------------------------------------------------");
            syslog_logger->debug("pfrd configuration file was reread...");
            syslog_logger->debug("global vars:");
            syslog_logger->debug("deep_delete: {}", deep_delete);
            syslog_logger->debug("min_rtt: {}", min_rtt);
            syslog_logger->debug("src_addr: {}", src_addr);
            syslog_logger->debug("max_time_of_echo: {}", max_time_of_echo);
            syslog_logger->debug("pid_file: {}", pid_path);
            syslog_logger->debug("pid: {}", getpid());
            syslog_logger->debug("gobgp_path: {}", gobgp_path);
            syslog_logger->debug("localnets: {}", localnets);
            syslog_logger->debug("enable_advertise: {}", enable_advertise);
         }

         send_stopped = 0;
         req_stopped = 0;

         //start routes manipulation
         if (enable_advertise) {
            syslog_logger->debug("8:s Start of pfr_routes_man()...");
            pfr_routes_man(probe_id, m, br, route);
            syslog_logger->debug("8:e End of pfr_routes_man()...");
         }

         syslog_logger->debug("proc_v4_new_cnt: {}", proc_v4_new_cnt);
         syslog_logger->debug("avg_rtt_new_cnt: {}", avg_rtt_new_cnt);
         syslog_logger->debug("scan_new_cnt: {}", scan_new_cnt);
         syslog_logger->debug("del_proc_v4_new_cnt: {}", del_proc_v4_new_cnt);
         syslog_logger->debug("del_avg_rtt_new_cnt: {}", del_avg_rtt_new_cnt);
         syslog_logger->debug("del_scan_new_cnt: {}", del_scan_new_cnt);
         syslog_logger->debug("printr_new_cnt: {}", printr_new_cnt);
         syslog_logger->debug("probe_id: {}", probe_id);

        }
        pthread_mutex_unlock(&mt_req_send);


        probe_id++;
        syslog_logger->debug("sleep({}), next for(), probe_id++: {}", sleep_before_next_loop, probe_id);
        sleep(sleep_before_next_loop); //config_t, sleep_before_next_loop default: 20
        pthread_join(thrddb, NULL);
    }
}
