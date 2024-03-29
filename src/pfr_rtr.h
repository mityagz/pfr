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
#include "grpc/pfr_gobgp_grpc.h"

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
        ~pfr_asbrs();
        pfr_asbrs(int id, std::string ip, class pfr_asbr_parm &parm);
        pfr_asbrs(class pfr_peers &peers);
        pfr_asbrs(std::map<int, pfr_peer> &mm);

        void set_asbr(int id, std::string ip, class pfr_asbr_parm &parm);
        class pfr_asbr_parm get_asbr(std::string ip);
        class pfr_asbr_parm get_asbr(int id_peer);
        std::string get_asbr_lo(int id_peer);
};

void pfr_route_update(int probe_id, pfr_dst_list &dstList, gobgp_grpc &grpcc, std::map<int, pfr_peer> &p, pfr_asbrs &br);
void pfr_routes_man(int probe_id, std::map<int, pfr_peer> &mm, gobgp_grpc &grpcc, pfr_asbrs &bbr, std::map<std::string, std::map<int, rt_parm *>> &rroute);
//void pfr_routes_man(int probe_id);
void pfr_create_set_jrouter_rt(std::map<int, pfr_peer> &mm, gobgp_grpc &grpcc, pfr_asbrs &bbr, int probe_id, int prev, int curr, std::string dst_ip);
void pfr_delete_set_jrouter_rt(std::map<int, pfr_peer> &mm, gobgp_grpc &grpcc, pfr_asbrs &bbr, int probe_id, int prev_peer_id, int curr_peer_id, std::string dst_ip);
void pfr_create_xml_jrouter_rt(std::string rt, struct nc_session * ncs);
void pfr_delete(int probe_id, gobgp_grpc &grpcc, std::map<int, pfr_peer> &p, pfr_asbrs &br); // delete dst_ip who didn't answer 3 times
void pfr_delete_all(int probe_id, gobgp_grpc &grpcc, std::map<int, pfr_peer> &p, pfr_asbrs &br); // delete all dst_ip (graceful shutdown)
void pfr_delete_r_route(std::map<int, pfr_peer> &p, gobgp_grpc &grpcc, pfr_asbrs &br, std::string dst_ip, int peer_id); // delete dst_ip subfunction
#endif // PFR_RTR_H
