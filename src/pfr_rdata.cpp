#include<vector>
#include<string>
#include<map>
#include<iostream>
#include "pfr_rdata.h"
#include "pfr_dst_list.h"
#include "pfr_peers.h"

extern pfr_dst_list pfrList;

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
*/

extern int avg_rtt_new_cnt;
extern int scan_new_cnt;
extern int del_scan_new_cnt;
extern int del_proc_v4_new_cnt;

extern int printr_new_cnt;

extern pthread_mutex_t mtr;

double tparm::get_rtt() { return rtt; }
double tparm::get_avg_rtt() { return avg_rtt; }

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
    std::cout << "ROUTE_FREE: probe_id: " << probe_id << std::endl;
    pthread_mutex_unlock(&mtr); 
}

void pfr_route_print(int probe_id) {
    for(std::map<std::string, std::map<int, rt_parm *>>::iterator it0 = route.begin(); it0 != route.end(); it0++) {
     for(std::map<int, rt_parm *>::iterator it1 = route[it0->first].begin(); it1 != route[it0->first].end(); it1++) {
        probe_id = it1->first;
        std::cout << "ROUTE_PRINT: dst_ip: " << it0->first << " : probe_id: " << probe_id << " :prev_peer: " << route[it0->first][probe_id]->get_prev_peer() << " :prev_min: " << route[it0->first][probe_id]->get_pmin_rtt() << " :peer_id: " << route[it0->first][probe_id]->get_curr_peer() << " : cmin_rtt : " << route[it0->first][probe_id]->get_cmin_rtt() << std::endl;
     } 
    }
    std::cout << "ROUTE_FREE: probe_id: " << probe_id << std::endl;
}

void pfr_route_scan(int probe_id) {
    double min_rtt = 50000; 
    double curr_rtt = 0; 
    int peer_id = 0; 
    //pthread_mutex_lock(&mtr); 
    for(std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>>::iterator it0 = r.begin(); it0 != r.end(); it0++) {
        std::string dst_ip = it0->first;
       for(std::map<int, std::map<int, tparm *>>::iterator it2 = r[dst_ip][probe_id].begin(); it2 != r[dst_ip][probe_id].end(); it2++) {
            //int peer_id = it2->first;
            if(probe_id == 0) {
                if(r[dst_ip][probe_id][it2->first][99] != NULL) 
                 min_rtt = r[dst_ip][probe_id][it2->first][99]->get_avg_rtt();
                route[it0->first][probe_id] = new rt_parm(0, min_rtt, 0, min_rtt);
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
       if(probe_id == 0) {
       } else if(probe_id > 0) {
         if(route.count(dst_ip) == 1 && route[dst_ip].count(probe_id - 1) == 1) {
            route[dst_ip][probe_id] = \
                new rt_parm(route[dst_ip][probe_id - 1]->get_curr_peer(), route[dst_ip][probe_id - 1]->get_cmin_rtt(), peer_id, min_rtt);
                scan_new_cnt++;
         } else {
            route[dst_ip][probe_id] = \
                new rt_parm(peer_id, min_rtt, peer_id, min_rtt);
                scan_new_cnt++;
         }
       }
       min_rtt = 50000; 
       curr_rtt = 0; 
       peer_id = 0; 
    }
    //pthread_mutex_unlock(&mtr); 
}

void pfr_calc_avg_rtt(int probe_id) {
    double avg_rtt = 0;
    double curr_rtt = 0;
    int cnt_rtt = 0;
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
            cnt_rtt++;
         } 
         r[dst_ip][probe_id][peer_id][99] = new tparm(0, curr_rtt / cnt_rtt, 0, 0);
         avg_rtt_new_cnt++;
         curr_rtt = 0;
         cnt_rtt = 0;
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
