#include <vector>
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <spdlog/spdlog.h>
#include <math.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "pfr_rdata.h"
#include "pfr_rtr.h"
#include "pfr_dst_list.h"
#include "pfr_peers.h"

extern pfr_dst_list pfrList;
extern std::shared_ptr<spdlog::logger> syslog_logger;

//r->{dst_ip}->{probe_id}->{peer_id, thread_id, timestamp, {seq_num}{rtt(curr_timestamp-timestamp)}, avg_rtt, lost(send_pkt-recv_pkt for given probe_id)}
//r{dst_ip}{probe_id}->{peer (thread_id, timestamp)}{seq}{tparm}
// std::map<std::string, probe> dst_ip; 


std::map<int, pfr_peer> m;
pfr_peers pp(m);

/*
class tparm {
 int rtt;
 int avg_rtt;
 int lost;
 int timestamp;
 public:
  tparm() {};
  tparm(int rtt, int lost, int timestamp) {
    this->rtt = rtt;
    this->lost = lost;
    this->timestamp;
  };
};

class tlog {
    int peer_id;
    double rtt;
    double avg_rtt;
    int lost;
    int timestamp;
    public:
     tlog(int peer_id, double rtt, double avg_rtt, int lost, int timestamp);
     int get_peer();
     double get_rtt();
     double get_avg_rtt();
     int get_lost();
     int get_timestamp();
};

*/

extern int avg_rtt_new_cnt;
extern int scan_new_cnt;
extern int del_scan_new_cnt;
extern int del_proc_v4_new_cnt;
extern int pfr_ping_req;

extern int printr_new_cnt;

extern pthread_mutex_t mtr;

double tparm::get_rtt() { return rtt; }
double tparm::get_avg_rtt() { return avg_rtt; }
int    tparm::get_timestamp() { return timestamp; }
int    tparm::get_lost() { return lost; }

tlog::tlog(int peer_id, double rtt, double avg_rtt, int lost, int timestamp) {
    this->peer_id = peer_id;
    this->rtt = rtt;
    this->avg_rtt = avg_rtt;
    this->lost = lost;
    this->timestamp;
}
int tlog::get_peer() { return peer_id; }
double tlog::get_rtt() { return rtt; }
double tlog::get_avg_rtt() { return avg_rtt; }
int tlog::get_lost() { return lost; }
int tlog::get_timestamp() {return timestamp; }

rt_parm::rt_parm(int prev_peer_id, double pmin_rtt, int curr_peer_rtt, double cmin_rtt) {
 this->prev_peer_id = prev_peer_id;
 this->pmin_rtt = pmin_rtt;
 this->curr_peer_id = curr_peer_rtt;
 this->cmin_rtt = cmin_rtt;
}

int rt_parm::get_prev_peer() { return prev_peer_id; }
int rt_parm::get_curr_peer() { return curr_peer_id; }
double rt_parm::get_pmin_rtt() { return pmin_rtt; }
double rt_parm::get_cmin_rtt() { return cmin_rtt; }


//              |dst_ip               |probe_id      |peer_id     |seq_num
extern std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>> r;
//              |dst_ip               |probe_id
extern std::map<std::string, std::map<int, rt_parm *>> route;


// log
//              |dsp_ip               |probe_id     |peer_id
extern std::map<std::string, std::map<int, std::map<int,  tlog *>>> route_log0;
extern std::map<std::string, std::map<int, tlog *>> route_log1;

/* This function removes all probe from r[0..probe_id - 2] and route[0..probe_id - 2]
 * left only two last probe
*/
void pfr_route_free(int probe_id) {
    if(probe_id < 2) return;
    probe_id -= 2;
    pthread_mutex_lock(&mtr); 
    std::string dst_ip;
    //int probe_id;
    int peer_id;
    int seq_id;
    for(std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>>::iterator it0 = r.begin(); it0 != r.end(); it0++) {
     dst_ip = it0->first;
     //for(std::map<int, std::map<int, std::map<int, tparm *>>>::iterator it1 = r[dst_ip].begin(); it1 != r[dst_ip].end(); it1++) {
      //probe_id = it1->first;
       for(std::map<int, std::map<int, tparm *>>::iterator it2 = r[dst_ip][probe_id].begin(); it2 != r[dst_ip][probe_id].end(); it2++) {
        peer_id = it2->first;
         for(std::map<int, tparm *>::iterator it3 = r[dst_ip][probe_id][peer_id].begin(); it3 != r[dst_ip][probe_id][peer_id].end(); it3++) {
            seq_id = it3->first;
            delete r[dst_ip][probe_id][peer_id][seq_id];
            r[dst_ip][probe_id][peer_id].erase(seq_id);
            //std::cout << "ROUTE_FREE delete r[" << dst_ip << "][" << probe_id << "][" << peer_id << "][" << seq_id << "]" << std::endl;
            del_proc_v4_new_cnt++;
         } 
         r[dst_ip][probe_id].erase(peer_id);
       }
       delete route[dst_ip][probe_id];
       route[dst_ip].erase(probe_id);
       //std::cout << "ROUTE_FREE delete route[" << dst_ip << "][" << probe_id << "]" << std::endl;
       del_scan_new_cnt++;
     //} //
    }
    //std::cout << "ROUTE_FREE: probe_id: " << probe_id << std::endl;
    syslog_logger->debug("ROUTE_FREE: probe_id :{}", probe_id);
    pthread_mutex_unlock(&mtr); 
}

/* Prints content of route[] */
void pfr_route_print(int probe_id) {
    for(std::map<std::string, std::map<int, rt_parm *>>::iterator it0 = route.begin(); it0 != route.end(); it0++) {
     for(std::map<int, rt_parm *>::iterator it1 = route[it0->first].begin(); it1 != route[it0->first].end(); it1++) {
        probe_id = it1->first;
        std::cout << "ROUTE_PRINT: dst_ip: " << it0->first << " : probe_id: " << probe_id << " :prev_peer: " << route[it0->first][probe_id]->get_prev_peer() << " :prev_min: " << route[it0->first][probe_id]->get_pmin_rtt() << " :peer_id: " << route[it0->first][probe_id]->get_curr_peer() << " : cmin_rtt : " << route[it0->first][probe_id]->get_cmin_rtt() << std::endl;
     } 
    }
    std::cout << "ROUTE_FREE: probe_id: " << probe_id << std::endl;
}


/* This function syncs ipfix data and dstList 
  its removes entities from r[] && route[] which don't exist in ipfix
  Important: need to remove entities from mxs too before remove from r[] && route[].
*/
void pfr_route_update(int probe_id, pfr_dst_list &dstList, std::map<int, pfr_peer> &p, pfr_asbrs &br) {
        std::map<std::string, int> rdst;
        std::set<std::string> deldst;

        pfr_dst_list::iterator it = dstList.begin();
        pfr_dst_list::iterator ite = dstList.end();
        
        int i = 0;
        while(it != ite) {
            std::string ip = (*it).get_ipv4();
            rdst[ip] = 1;
            it++;
            i++;
        } 

    std::string dst_ip;
    int peer_id, seq_id;
    int cnt_ip = 0;

    for(std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>>::iterator it0 = r.begin(); it0 != r.end(); it0++) {
     dst_ip = it0->first;
     if(rdst.count(dst_ip) == 1) {
        syslog_logger->debug("ROUTE_UPDATE route[ip] exists in dstList[ip]: {}:{}", cnt_ip, dst_ip);
     } else {
        // to remove entry from mxs
        if(route[dst_ip].count(probe_id) == 1 && route[dst_ip][probe_id]->get_curr_peer() > 0) {
          peer_id = route[dst_ip][probe_id]->get_curr_peer();
          pfr_delete_set_jrouter_rt(p, br, 0, 0, peer_id, dst_ip);
        }

        syslog_logger->debug("ROUTE_UPDATE route[ip] doesn't exist in dstList[ip] need to remove from route[]: {}:{}", cnt_ip, dst_ip);
        for(std::map<int, std::map<int, std::map<int, tparm *>>>::iterator it1 = r[dst_ip].begin(); it1 != r[dst_ip].end(); it1++) {
            probe_id = it1->first;
            for(std::map<int, std::map<int, tparm *>>::iterator it2 = r[dst_ip][probe_id].begin(); it2 != r[dst_ip][probe_id].end(); it2++) {
                peer_id = it2->first;
                for(std::map<int, tparm *>::iterator it3 = r[dst_ip][probe_id][peer_id].begin(); it3 != r[dst_ip][probe_id][peer_id].end(); it3++) {
                    seq_id = it3->first;
                    deldst.insert(dst_ip);
                    delete r[dst_ip][probe_id][peer_id][seq_id];
                    r[dst_ip][probe_id][peer_id].erase(seq_id);
                }
                del_proc_v4_new_cnt++;
                r[dst_ip][probe_id].erase(peer_id);
            }
            delete route[dst_ip][probe_id];
            route[dst_ip].erase(probe_id);
            r[dst_ip].erase(probe_id);
        }
        del_scan_new_cnt++;

        std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>>::iterator itr;
        std::map<std::string, std::map<int, rt_parm *>>::iterator itroute;
        itr = r.find (dst_ip);
        r.erase(itr);
        itroute = route.find(dst_ip);
        route.erase(itroute);
     }
    cnt_ip++;
    }
}

/* Print content route_log1 */
void pfr_log_print(int curr_probe_id) {
//                 |dsp_ip               |probe_id     |peer_id
// extern std::map<std::string, std::map<int, tlog *>> route_log1;
    std::string dst_ip;
    int probe_id;
    for(std::map<std::string, std::map<int, tlog *>>::iterator it0 = route_log1.begin(); it0 != route_log1.end(); it0++) {
        dst_ip = it0->first;
        for(std::map<int, tlog *>::iterator it1 = route_log1[dst_ip].begin(); it1 != route_log1[dst_ip].end(); it1++) {
            probe_id = it1->first;
            syslog_logger->debug("pfr_log_print(): dst_ip: {} : curr_probe_id: {} : probe_id : {} : peer_id : {}", dst_ip, curr_probe_id, probe_id,  \
                      route_log1[dst_ip][probe_id]->get_peer());
        }
    }
}

/* This function calculates min rtt and takes place it route[dst_ip][probe_id] 
   also this one fills route_log1[probe_id]
*/
void pfr_route_scan(int probe_id) {
    double min_rtt = 50000; 
    double curr_rtt = 0; 
    double avg_rtt = 0; 
    int peer_id = 0; 
    int ts = 0; 
    int lost = 0; 
    std::map<int, std::map<int, tparm *>>::iterator it_peer_id;
    //pthread_mutex_lock(&mtr); 
    for(std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>>::iterator it0 = r.begin(); it0 != r.end(); it0++) {
        std::string dst_ip = it0->first;
       for(std::map<int, std::map<int, tparm *>>::iterator it2 = r[dst_ip][probe_id].begin(); it2 != r[dst_ip][probe_id].end(); it2++) {
            //int peer_id = it2->first;
            it_peer_id = it2;
            if(probe_id == 0) {
                if(r[dst_ip][probe_id][it2->first][99] != NULL) 
                 min_rtt = r[dst_ip][probe_id][it2->first][99]->get_avg_rtt();
                
                syslog_logger->debug("pfr_route_scan(): probe_id : {} : {} : {}", probe_id, 0, it2->first);
                route[it0->first][probe_id] = new rt_parm(0, min_rtt, it2->first, min_rtt);
                peer_id = it2->first;
                scan_new_cnt++;
                break;
            } else {
                if(r[dst_ip][probe_id][it2->first][99] != NULL) 
                 curr_rtt = r[dst_ip][probe_id][it2->first][99]->get_avg_rtt();
                if(curr_rtt < min_rtt) {
                  min_rtt = curr_rtt;
                  peer_id = it2->first;
                } 
            }
       }
       if(r[dst_ip][probe_id][it_peer_id->first][99] != NULL) {
        avg_rtt = r[dst_ip][probe_id][it_peer_id->first][99]->get_avg_rtt();
        ts = r[dst_ip][probe_id][it_peer_id->first][99]->get_timestamp();
        lost = r[dst_ip][probe_id][it_peer_id->first][99]->get_lost();
       }
       if(probe_id == 0) {
         syslog_logger->info("pfr_log->sql_hist probe_id == 0: dst_ip: {}: probe_id: {} :peer_id {}:min_rtt: {}: avg_rtt {}:lost: {}: timestamp: {}", dst_ip, probe_id, peer_id, min_rtt, avg_rtt, lost, ts);
         route_log1[dst_ip][probe_id] = new tlog(peer_id, min_rtt, avg_rtt, lost, ts);
       } else if(probe_id > 0) {
         if(route.count(dst_ip) == 1 && route[dst_ip].count(probe_id - 1) == 1) {
            if(peer_id == 0) { 
                // case 2.0 no answer from dst
                route[dst_ip][probe_id] = \
                    new rt_parm(route[dst_ip][probe_id - 1]->get_curr_peer(), route[dst_ip][probe_id - 1]->get_cmin_rtt(), \
                            route[dst_ip][probe_id - 1]->get_curr_peer(), min_rtt);
                syslog_logger->debug("pfr_route_scan(): x -> x : probe_id : {} : {} : {}", probe_id, route[dst_ip][probe_id - 1]->get_curr_peer(), peer_id);
                syslog_logger->info("pfr_log->sql_hist 2.0: dst_ip: {}: probe_id: {} :peer_id {}:min_rtt: {}: avg_rtt: {}:lost: {}: timestamp: {}", dst_ip, probe_id, peer_id, min_rtt, avg_rtt, lost, ts);
                //              |dsp_ip               |probe_id     |peer_id
                //extern std::map<std::string, std::map<int, tlog *>> route_log1;
                route_log1[dst_ip][probe_id] = new tlog(peer_id, min_rtt, avg_rtt, lost, ts);
                scan_new_cnt++;
            } else {
                // case 2.1 there is answer from dst
                route[dst_ip][probe_id] = \
                    new rt_parm(route[dst_ip][probe_id - 1]->get_curr_peer(), route[dst_ip][probe_id - 1]->get_cmin_rtt(), peer_id, min_rtt);
                syslog_logger->debug("pfr_route_scan(): x -> y : probe_id : {} : {} : {}", probe_id, route[dst_ip][probe_id - 1]->get_curr_peer(), peer_id);
                syslog_logger->info("pfr_log->sql_hist 2.1: dst_ip: {}: probe_id: {} :peer_id {}:min_rtt: {}: avg_rtt: {}:lost: {}: timestamp: {}", dst_ip, probe_id, peer_id, min_rtt, avg_rtt, lost, ts);
                route_log1[dst_ip][probe_id] = new tlog(peer_id, min_rtt, avg_rtt, lost, ts);
            }
         } else {
            // case 1.1 there isn't prev answer from dst
            route[dst_ip][probe_id] = \
                new rt_parm(0, min_rtt, peer_id, min_rtt);
                scan_new_cnt++;
                syslog_logger->debug("pfr_route_scan() probe_id - 1: {}: peer_prev_id : {} : probe_id: {}: peer_id{}", probe_id - 1, 0, probe_id, peer_id);
                syslog_logger->info("pfr_log->sql_hist 1.1: dst_ip: {}: probe_id: {} :peer_id {}:min_rtt: {}: avg_rtt: {}:lost: {}: timestamp: {}", dst_ip, probe_id, peer_id, min_rtt, avg_rtt, lost, ts);
                route_log1[dst_ip][probe_id] = new tlog(peer_id, min_rtt, avg_rtt, lost, ts);
            }
         }
       min_rtt = 50000; 
       curr_rtt = 0; 
       peer_id = 0; 
    }
    //pthread_mutex_unlock(&mtr); 
}

/* ^---
 * v---
 0. if traffic to dst_ip 0                       -> dst_ip will be removed by pfr_route_update from dstList and rrs, mxs.
    didn't send
    because ipfix absent 
 1. first appears:       0 0 no response         -> don't add to route
                         0 x there is responses  -> add to route
 2. following appears:   x 0 no response         -> !update route x -> x -> if there isn't three response, delete from route. we need history_route structure
                                                    and delete from mxes!
                                                    pfr_rdata::route_scan()
                                                    add func pfr_log_print()
                                                    add func pfr_list_noansw3x()
                                                    add func pfr_jdel_route(list_noanswer3x)
                                                    pfr_rdata::pfr_delete() { 
                                                          r[1.1.1.1][2, 3] -> route[1.1.1.1][3].get_peer() \ 
                                                          -> delete_route_from_jun() -> delete r[1.1.1.1][2, 3] \
                                                           -> delete route[1.1.1.1]/route_log1
                                                    }
                          x y there is responses  -> update route x -> y
 3. dissappear from dst: 
*/

/* This function deletes dst_ip && route from mxs 
 * which no response 3 times
*/
void pfr_delete(int probe_id, std::map<int, pfr_peer> &p, pfr_asbrs &br) {
//                |dsp_ip               |probe_id
//extern std::map<std::string, std::map<int, tlog *>> route_log1;
// TODO: conf_deep_delete
 bool delete_flag = true;
 int deep_delete = 3;
 int peer_id = 0;
 std::map<int, tlog *>::iterator it_peer_id;
 std::string dst_ip;
 if(probe_id <= deep_delete)
     return;
 for(std::map<std::string, std::map<int, tlog *>>::iterator it0 = route_log1.begin(); it0 != route_log1.end(); it0++) {
  dst_ip = it0->first;
  delete_flag = true;
  for(int i = 0; i <= deep_delete; i++) {
      if(route_log1[dst_ip].count(probe_id - i) == 1) {
       if(route_log1[dst_ip][probe_id - i]->get_peer() == 0) {

       } else {
              delete_flag = false;
              break;
       }
      }
  }
  syslog_logger->debug("pfr_delete() : delete_flag {}", delete_flag);
  if(delete_flag) {
   if(route.count(dst_ip) == 1 && route[dst_ip].count(probe_id) == 1 && route[dst_ip][probe_id] != NULL) {
    peer_id = route[dst_ip][probe_id]->get_curr_peer();
    syslog_logger->debug("pfr_delete() : delete_flag {}: dst_ip {} : probe_id - 1: {}: peer_prev_id : {} : probe_id: {}: peer_id {}", delete_flag, dst_ip, probe_id - 1, 0, probe_id, peer_id);
    pfr_delete_r_route(p, br, dst_ip, peer_id);
   }
   // delete r[dst_ip], route[dst_ip], delete_from_mx(dst_ip, peer_id)
   delete_flag = true;
  }
  delete_flag = true;
 }
}

void pfr_delete_r_route(std::map<int, pfr_peer> &p, pfr_asbrs &br, std::string dst_ip, int peer_id) {
    int probe_id = 0;
    int seq_id = 0;
    int last_peer_id = peer_id;
    syslog_logger->debug("0 pfr_r_route_delete() dst_ip {} : peer_id {}", dst_ip, last_peer_id);
    for(std::map<int, std::map<int, std::map<int, tparm *>>>::iterator it0 = r[dst_ip].begin(); it0 != r[dst_ip].end(); it0++) {
       probe_id = it0->first;
       for(std::map<int, std::map<int, tparm *>>::iterator it2 = r[dst_ip][probe_id].begin(); it2 != r[dst_ip][probe_id].end(); it2++) {
        peer_id = it2->first;
         for(std::map<int, tparm *>::iterator it3 = r[dst_ip][probe_id][peer_id].begin(); it3 != r[dst_ip][probe_id][peer_id].end(); it3++) {
            seq_id = it3->first;
            delete r[dst_ip][probe_id][peer_id][seq_id];
            r[dst_ip][probe_id][peer_id].erase(seq_id);
         } 
         r[dst_ip][probe_id].erase(peer_id);
       }
       delete route[dst_ip][probe_id];
       r[dst_ip].erase(probe_id);
       route[dst_ip].erase(probe_id);
    }
    syslog_logger->debug("1 pfr_r_route_delete() dst_ip {} : peer_id {}", dst_ip, last_peer_id);
    pfr_delete_set_jrouter_rt(p, br, 0, 0, last_peer_id, dst_ip);
    r.erase(dst_ip);
    route.erase(dst_ip);
}

void pfr_calc_avg_rtt(int probe_id) {
    double avg_rtt = 0;
    double curr_rtt = 0;
    int cnt_rtt = 0;
    int alive = 0;
    int ts = 0;
    //pthread_mutex_lock(&mtr); 
    for(std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>>::iterator it0 = r.begin(); it0 != r.end(); it0++) {
        std::string dst_ip = it0->first;
     //for(std::map<int, std::map<int, std::map<int, tparm *>>>::iterator it1 = r[dst_ip].begin(); it1 != r[dst_ip].end(); it1++) {
      //std::string probe_id = it1->first;
       for(std::map<int, std::map<int, tparm *>>::iterator it2 = r[dst_ip][probe_id].begin(); it2 != r[dst_ip][probe_id].end(); it2++) {
            int peer_id = it2->first;
         for(std::map<int, tparm *>::iterator it3 = r[dst_ip][probe_id][peer_id].begin(); it3 != r[dst_ip][probe_id][peer_id].end(); it3++) {
            int seq_num = it3->first;
            curr_rtt += (it3->second)->get_rtt();
            ts = (it3->second)->get_timestamp();
            cnt_rtt++;
            alive++;
         } 
         double avg_rtt = ceil(curr_rtt * 100 / cnt_rtt) / 100.0;
         r[dst_ip][probe_id][peer_id][99] = new tparm(0, avg_rtt, pfr_ping_req - alive, ts);
         avg_rtt_new_cnt++;
         curr_rtt = 0;
         cnt_rtt = 0;
         alive = 0;
         ts = 0;
         avg_rtt = 0;
       }
     //} 
    }
    //pthread_mutex_unlock(&mtr); 
}

void pfr_print_avg_rtt(int probe_id) {
    for(std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>>::iterator it0 = r.begin(); it0 != r.end(); it0++) {
     //std::cout << "dst_ip: " << it0->first << std::endl;
     for(std::map<int, std::map<int, std::map<int, tparm *>>>::iterator it1 = r[it0->first].begin(); it1 != r[it0->first].end(); it1++) {
      //std::cout << "probe_id: " << it1->first << " ";
       for(std::map<int, std::map<int, tparm *>>::iterator it2 = r[it0->first][it1->first].begin(); it2 != r[it0->first][it1->first].end(); it2++) {
            //std::cout << "peer_id: " << it2->first << " ";
           std::cout << "dst_ip: " << it0->first << " :probe_id " << it1->first << " :peer_id: " << it2->first << ": avg_rtt: " << r[it0->first][it1->first][it2->first][99]->get_avg_rtt() << std::endl;
       }
     } 
    }

}

void print_rdata() {
    for(std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>>::iterator it0 = r.begin(); it0 != r.end(); it0++) {
     std::cout << "dst_ip: " << it0->first << std::endl;
     for(std::map<int, std::map<int, std::map<int, tparm *>>>::iterator it1 = r[it0->first].begin(); it1 != r[it0->first].end(); it1++) {
      std::cout << "probe_id: " << it1->first << " " << std::endl;
       for(std::map<int, std::map<int, tparm *>>::iterator it2 = r[it0->first][it1->first].begin(); it2 != r[it0->first][it1->first].end(); it2++) {
        //if(it2->first == 0)
            std::cout << "peer_id: " << it2->first << " ";
        //else
            //std::cout << "           peer_id: " << it2->first << " ";
         for(std::map<int, tparm *>::iterator it3 = r[it0->first][it1->first][it2->first].begin(); it3 != r[it0->first][it1->first][it2->first].end(); it3++) {
            std::cout << "seq_num: " << it3->first << "->{ rtt: " << (it3->second)->get_rtt() << " } ";
            printr_new_cnt++;
         } 
            std::cout << std::endl;
       }
     } 
    }
}

pfr_rdata::pfr_rdata() {
    // needs to prefill datastruct pfr_rdata
    pfr_dst_list::iterator it = pfrList.begin();
    pfr_dst_list::iterator ite = pfrList.end();

    while(it != ite) {
        std::map <int, pfr_rdata::peer> *p = new std::map<int, pfr_rdata::peer>();
        r.insert(std::pair<std::string, std::map<int, pfr_rdata::peer> *>((*it).get_ipv4(), p)); // r->{dsp_ip:peer_id{int:probe_id}}
        for(std::map<int, pfr_peer>::iterator itm = m.begin(); itm != m.end(); ++itm) {
            const int p_id = itm->first;
            pfr_rdata::probe prb; //
            pfr_rdata::peer pr(p_id, prb); //
            (r[(*it).get_ipv4()])->insert(std::make_pair(p_id, pr));
        }
    }
}

       /*
pfr_rdata::pfr_rdata(std::string dest_ip, int peer_id, int probe_id, int thread_id, int timestamp, \
                     int seq_num, int rtt, int avg_rtt, int lost) {
    
    tparm tp = tparm(avg_rtt, lost, rtt, timestamp); 
    seq s = seq(seq_num, tp); 
    

    //for(std::map<std::string, peer>::iterator it = dst_ip.begin(); it != dst_ip.end(); ++it) {
       //(it->first, it->second));
       probe pr = dst_ip[dest_ip];
       if(pr != NULL) {
        if(peer_id == pfr_rdata::probe::get_peer_id() && probe_id == pfr_rdata::probe::get_probe_id()) {

        } else {
            probe probes = probe(peer_id, probe_id, thread_id, timestamp, s);
        }
       } else {
       
       }
    //}

}
       */

// tparm
/*
int avg_rtt;
int lost;
int rtt;
int timestamp;
*/

/*
pfr_rdata::tparm::tparm(int avg_rtt, int lost, int rtt, int timestamp) {
    this->avg_rtt = avg_rtt;
    this->lost = lost;
    this->rtt = rtt;
    this->timestamp = timestamp;
}

void pfr_rdata::tparm::print() {
    //std::cout << "avg_rtt: " << avg_rtt << " lost: " << lost << " rtt: " << rtt << " timestamp: " << timestamp << std::endl;
}
*/

// seq
//std::vector<tparm> seq_num;
/*
pfr_rdata::seq::seq(int seq_n, tparm tp) {
    seq_num[seq_n] = tp;  
}
*/

/*
int peer_id;
int probe_id;
int thread_id;
int timestamp;
std::vector<seq> seq_id;
*/
pfr_rdata::probe::probe() {}
pfr_rdata::probe::probe(int peer_id, int probe_id, int thread_id, int timestamp, seq seq_id) {

}

/*
int peer_id;
std::vector<probe> probe_id;
*/

pfr_rdata::peer::peer() { }

pfr_rdata::peer::peer(int peer_id, probe probe_b) { 
    this->peer_id = peer_id;
    p.insert(std::make_pair(peer_id, probe_b));
}

/*
pfr_rdata::peer::peer(int peer_id, probe *probe_s) {
    this->peer_id = peer_id;
    //this->probe = peer_s;
}
*/
