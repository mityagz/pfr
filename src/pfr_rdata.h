#include<string>
#include<map>
#include<vector>

class tparm {
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
  double get_rtt();
  double get_avg_rtt();
};

void pfr_calc_avg_rtt(int probe_id); // calculate average rtt && saves it to r...[seq == 99]
void pfr_print_avg_rtt(int probe_id); // print average rtt from [seq == 99]

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
