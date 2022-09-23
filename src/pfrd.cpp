#include <pthread.h>
#include <iostream>
#include <map>
#include <vector>
#include <unistd.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <typeinfo>
#include "pfr_dst.h"
#include "pfr_dst_list.h"
#include "pfr_peers.h"
#include "pfr_rdata.h"
#include "pfr_rtr.h"
#include "ping0.h"

using namespace std;

/*
typedef struct {
 int peer_id;
 int probe_id;
 pfr_dst_list *dst_list;
} idata;
*/

void *send_req(void *pin) {
 pthread_t thId = pthread_self();

 idata *in = (idata*)pin;
 send_v4(in, thId);
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
int pfr_ping_req = 5;

int main() {
    //main loop 
    //data for thread peer_id, probe_id, thread_id, timestamp
    idata itdata[999];
    std::map<int, pfr_peer> m;
    pfr_peers pp(m);
    int t_ct = pp.size();
    pthread_t thrds[t_ct];
    pthread_t thrdrd;
    bool fthread = 0;
    int m_ct = 10000;
    pthread_mutex_t mtxs[m_ct];

    std::string ident = "pfrd";
    syslog_logger = spdlog::syslog_logger_mt("pfr_syslog", ident, LOG_PID, LOG_LOCAL5);
    syslog_logger->set_level(spdlog::level::debug);
    syslog_logger->debug("pfrd was started...");

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

        pthread_mutex_lock(&mt_req_send); 
        req_stopped = 1;
        pthread_mutex_unlock(&mt_req_send); 
        syslog_logger->debug("sleep(6)");
        sleep(6);
        
        //pthread_join(thrdrd, NULL);
        
        pthread_mutex_lock(&mt_req_send);
        if(send_stopped == 1) {
         syslog_logger->debug("3:s Start of pfr_route_update()...");
         pfr_route_update(probe_id, pfrList);
         syslog_logger->debug("3:e End of pfr_route_update()...");

         syslog_logger->debug("sleep(60)");
         sleep(60);
         
         //print_rdata();

         syslog_logger->debug("4:s Start of pfr_calc_avg_rtt()...");
         pfr_calc_avg_rtt(probe_id);
         syslog_logger->debug("4:e End of pfr_calc_avg_rtt()...");

         //pfr_print_avg_rtt(probe_id);

         syslog_logger->debug("5:s Start of pfr_route_scan()...");
         pfr_route_scan(probe_id);
         syslog_logger->debug("5:e Start of pfr_route_scan()...");

         syslog_logger->debug("6:s Start of pfr_route_free()...");
         pfr_route_free(probe_id);
         syslog_logger->debug("6:e End of pfr_route_free()...");

         //pfr_route_print(probe_id);
         
         // pfr_log
         syslog_logger->debug("7:s Start of pfr_log_print()...");
         pfr_log_print(probe_id);
         syslog_logger->debug("7:e End of pfr_log_print()...");


         send_stopped = 0;
         req_stopped = 0;

         //start routes manipulation
         syslog_logger->debug("8:s Start of pfr_routes_man()...");
         pfr_routes_man(probe_id, m, br, route);
         syslog_logger->debug("8:e End of pfr_routes_man()...");

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
        syslog_logger->debug("sleep(700), next for(), probe_id++");
        sleep(700);
    }
}
