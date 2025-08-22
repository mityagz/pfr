#include<string>
#include<map>
#include<vector>
#include "pfr_dst_list.h"
#include "pfr_peers.h"
#include "pfr_sql_log.h"
#include "grpc/pfr_gobgp_grpc.h"

class tparm {
  double min_rtt;
  double max_rtt;
  double stddev_rtt;
  double mediana_rtt;
  double avg_rtt;
  int lost;
  double rtt;
  int timestamp;
 public:
  tparm(double rtt, double avg_rtt, int lost, int timestamp) {
  this->rtt = rtt;
  this->avg_rtt = avg_rtt;;
  this->lost = lost;
  this->timestamp = timestamp;
  }
  void print();
  double get_min_rtt();
  double get_max_rtt();
  double get_stddev_rtt();
  double get_mediana_rtt();
  void set_min_rtt(double min_rtt);
  void set_max_rtt(double max_rtt);
  void set_stddev_rtt(double stddev_rtt);
  void set_mediana_rtt(double mediana_rtt);
  double get_rtt();
  double get_avg_rtt();
  int get_timestamp();
  int get_lost();
};

void pfr_calc_avg_rtt(int probe_id); // calculate average rtt && saves it to r...[seq == 99]
void pfr_calc_stddev_rtt(int probe_id); // calculate stddev rtt && saves it to r...[seq == 99]
void pfr_print_avg_rtt(int probe_id); // print average rtt from [seq == 99]
void pfr_route_free(int probe_id); // free/delete route 
//void pfr_route_update(int probe_id, pfr_dst_list &p); // update from ipfix 
//void pfr_route_update(int probe_id, pfr_dst_list &dstList, std::map<int, pfr_peer> &p, pfr_asbrs &br, std::string dst_ip);
void pfr_log_print(int probe_id); // delete dst_ip who didn't answer 3 times
//void pfr_delete(int probe_id, std::map<int, pfr_peer> &p, pfr_asbrs &br); // delete dst_ip who didn't answer 3 times
//void pfr_delete_r_route(std::map<int, pfr_peer> &p, pfr_asbrs &br, std::string dst_ip, int peer_id); // delete dst_ip subfunction


class rt_parm {
    int prev_peer_id;
    double pmin_rtt;
    int curr_peer_id;
    double cmin_rtt;
 public:
    rt_parm(int prev_peer_id, double pmin_rtt, int curr_peer_rtt, double cmin_rtt);
    int get_prev_peer();
    int get_curr_peer();
    double get_pmin_rtt();
    double get_cmin_rtt();
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

void pfr_route_scan(int probe_id, pfr_sql_log &sql_log); // finds min average rtt from [seq == 99]
//void pfr_route_scan_sql(int probe_id, pfr_sql_log &sql_log); // finds min average rtt from [seq == 99]
void *pfr_route_scan_sql(void *); // finds min average rtt from [seq == 99]
void pfr_route_print(int probe_id); // print min average rtt from [seq == 99]

class pfr_rdata {
//r->{dst_ip}->{probe_id}->{peer_id, thread_id, timestamp, {seq_num}{rtt(curr_timestamp-timestamp)}, avg_rtt, lost(send_pkt-recv_pkt for given probe_id)}


 class tparm;
 class seq;
 class peer;
 class probe;

 std::map<std::string, std::map<int, peer> *> r; 


 class peer {
  int peer_id;
  std::map<int, probe> p;
 public:
  peer();
  peer(int peer_id, probe probe_s);
  //peer(int peer_id, probe *probe_s);
 };

 class probe {
  int peer_id;
  int probe_id;
  int thread_id;
  int timestamp;
  std::vector<seq> seq_id;
 public:
  probe(void);
  probe(int peer_id, int probe_id, int thread_id, int timetamp, seq seq_id);
  int get_peer_id(void) { return peer_id; };
  int get_probe_id(void) { return probe_id; };
 };

 class seq {
  std::vector<tparm> seq_num;
 public:
  seq(int seq_n, tparm tp);
 };

 class tparm {
  int avg_rtt;
  int lost;
  int rtt;
  int timestamp;
 public:
  tparm(int avg_rtt, int lost, int rtt, int timetamp);
  void print();
 };

 public:
  pfr_rdata();
  pfr_rdata(std::string dst_ip, int peer_id, int probe_id, int thread_id, int timestamp, \
            int seq_num, int rtt, int avg_rtt, int lost);
};
