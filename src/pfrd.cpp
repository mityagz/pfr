#include <pthread.h>
#include <iostream>
#include <map>
#include <vector>
#include <unistd.h>
#include "pfr_dst.h"
#include "pfr_dst_list.h"
#include "pfr_peers.h"
#include "pfr_rdata.h"
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

pfr_dst_list pfrList(10);
std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>> r;
std::map<std::string, std::map<int, rt_parm *>> route;

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

    //pfr_dst_list pfrList(10);

    for(;;) {

    static int probe_id = 0;
    int ct_data = 0;

        for(int i = 0; i < t_ct; i++) {
            pthread_mutex_init(&mtxs[i], NULL);
        }

        for(std::map<int, pfr_peer>::iterator itm = m.begin(); itm != m.end(); ++itm) {
            //cout << itm->first << ":" << (itm->second).pfr_peer_get_id() << ":" << (itm->second).pfr_peer_get_pe_ip() << endl;
            int p_id = itm->first;
            itdata[ct_data] = (idata){ .mm = m, .peer_id = p_id, .probe_id = probe_id, .dst_list = &pfrList, .mtx = mtxs };
            ct_data++;
        }


        if(!fthread) {
            pthread_create(&thrdrd, NULL, readloop, NULL);
            fthread = true;
        }

        for(int i = 0; i < ct_data; i++) {
            pthread_create(&thrds[i], NULL, send_req, &itdata[i]);
        }


        for(int j = 0; j < ct_data; j++) {
            pthread_join(thrds[j], NULL);
        }
        
        req_stopped = 1;
        sleep(60);
        
        //pthread_join(thrdrd, NULL);
        
        if(send_stopped == 1) {
         print_rdata();
         pfr_calc_avg_rtt(probe_id);
         pfr_print_avg_rtt(probe_id);
         pfr_route_scan(probe_id);
         pfr_route_free(probe_id);
         pfr_route_print(probe_id);
         send_stopped = 0;
         req_stopped = 0;

         std::cout << "proc_v4_new_cnt: " << proc_v4_new_cnt << std::endl;
         std::cout << "avg_rtt_new_cnt: " << avg_rtt_new_cnt << std::endl;
         std::cout << "scan_new_cnt: " << scan_new_cnt << std::endl;
         std::cout << "del_proc_v4_new_cnt: " << del_proc_v4_new_cnt << std::endl;
         std::cout << "del_avg_rtt_new_cnt: " << del_avg_rtt_new_cnt << std::endl;
         std::cout << "del_scan_new_cnt: " << del_scan_new_cnt << std::endl;
         std::cout << "printr_new_cnt: " << printr_new_cnt << std::endl;
        }

        //pthread_mutex_unlock(&mtr);
        //if(probe_id > 3) {
         //exit(0);
        //}
        probe_id++;
        sleep(600);
    }
}
