#include<vector>
#include<string>
#include<map>
#include<iostream>
#include "pfr_rdata.h"
#include "pfr_dst_list.h"
#include "pfr_peers.h"

extern pfr_dst_list pfrList;

//r->{dst_ip}->{peer_id}->{probe_id, thread_id, timestamp, {seq_num}{rtt(curr_timestamp-timestamp)}, avg_rtt, lost(send_pkt-recv_pkt for given probe_id)}
//r{dst_ip}{peer_id}->{probe (thread_id, timestamp)}{seq}{tparm}
// std::map<std::string, peer> dst_ip; 


std::map<int, pfr_peer> m;
pfr_peers pp(m);


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

//       |dsp_ip               |peer_id      |probe_id     |seq_num
std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>> r;

void testdata() {
    tparm *tp = new tparm(0, 1, 2);
    r["1.1.1.1"][2][0][0] = tp;
    r["1.1.1.1"][2][1][0] = tp;
    
    tp = new tparm(0, 5, 5);
    r["8.8.8.8"][7][0][0] = tp;

    for(std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>>::iterator it0 = r.begin(); it0 != r.end(); ++it0) {
     std::cout << it0->first << std::endl;
     for(std::map<int, std::map<int, std::map<int, tparm *>>>::iterator it1 = r[it0->first].begin(); it1 != r[it0->first].end(); ++it1) {
      std::cout << it1->first << std::endl;
       for(std::map<int, std::map<int, tparm *>>::iterator it2 = r[it0->first][it1->first].begin(); it2 != r[it0->first][it1->first].end(); ++it2) {
        std::cout << it2->first << std::endl;
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
