#include <pthread.h>
#include <iostream>
#include <map>
#include "pfr_dst.h"
#include "pfr_dst_list.h"
#include "pfr_peers.h"

using namespace std;

int mmain() {
    std::map<int, pfr_peer> m;
    pfr_peers pp(m);
    int t_ct = pp.size();
    int m_ct = 10000;
    pthread_t thrds[t_ct];
    pthread_mutex_t mtxs[m_ct];

    pfr_dst_list pfrList(10);

    for(std::map<int, pfr_peer>::iterator itm = m.begin(); itm != m.end(); ++itm) {
        cout << itm->first << ":" << (itm->second).pfr_peer_get_id() << ":" << (itm->second).pfr_peer_get_pe_ip() << endl;
        
        pfr_dst_list::iterator it = pfrList.begin();
        pfr_dst_list::iterator ite = pfrList.end();
        while(it != ite) {
            (*it).pfr_dst_print();
            cout << it.print() << ":" << ite.print() << endl; 
            it++;
        }
    }
}
