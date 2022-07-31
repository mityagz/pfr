#ifndef PFR_RTR_H
#define PFR_RTR_H
#include <string>
#include <map>
#include <libnetconf.h>
#include <libnetconf_ssh.h>
#include "pfr_peers.h"
#include <unistd.h>
#include "pfr_dst.h"
#include "pfr_dst_list.h"
#include "pfr_peers.h"
//#include "pfr_rdata.h"
#include "ping0.h"

class pfr_asbr_parm {
    private:
        std::string name;
        std::string ip;
        struct nc_session *netconf_conn;
    public:
        pfr_asbr_parm();
        pfr_asbr_parm(std::string name, std::string ip);
        struct nc_session * get_session();
        void set_session();
};
 
class pfr_asbrs {
    private:
        std::map<std::string, class pfr_asbr_parm> asbrs;
        std::map<int, std::string> asbrIdPeer;
        class pfr_peers p;
    public:
        pfr_asbrs();
        pfr_asbrs(int id, std::string ip, class pfr_asbr_parm &parm);
        pfr_asbrs(class pfr_peers &peers);
        pfr_asbrs(std::map<int, pfr_peer> &mm);
        //pfr_asbrs(std::map<int, int> &mm);
        //pfr_asbrs(int a, int b);

        void set_asbr(int id, std::string ip, class pfr_asbr_parm &parm);
        class pfr_asbr_parm get_asbr(std::string ip);
        class pfr_asbr_parm get_asbr(int id_peer);
        std::string get_asbr_lo(int id_peer);
};

/*
   conn_hash[peer_id] = conn_descripter
   int pfr_update_jrouter_conf(probe_id) {
        for dst_ip
          if:
           route[dst_ip].[probe_id].[prev_peer_id] != [curr_peer_id]
           route_pack[prev_peer_id][curr_peer_id].push_back(dst_ip)
           go to prev delete && go to curr add dst_ip, all in the ephhemerial db
  } 
*/

#endif // PFR_RTR_H
