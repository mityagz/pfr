#include <pthread.h>
#include <iostream>
#include <map>
#include <vector>
#include <unistd.h>
#include "pfr_dst.h"
#include "pfr_dst_list.h"
#include "pfr_peers.h"
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

void testdata();

pfr_dst_list pfrList(10);

int main() {
    //main loop 
    //data for thread peer_id, probe_id, thread_id, timestamp
    idata itdata[999];
    std::map<int, pfr_peer> m;
    pfr_peers pp(m);
    int t_ct = pp.size();
    int m_ct = 10000;
    pthread_t thrds[t_ct];
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

        for(int i = 0; i < 2; i++) {
        //for(int i = 0; i < ct_data; i++) {
            pthread_create(&thrds[i], NULL, send_req, &itdata[i]);
        }

        probe_id++;

        for(int j = 0; j < 2; j++) {
        //for(int j = 0; j < ct_data; j++) {
            pthread_join(thrds[j], NULL);
        }
        testdata();
        sleep(60);
    }
}
