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
        ~pfr_asbrs();
        pfr_asbrs(int id, std::string ip, class pfr_asbr_parm &parm);
        pfr_asbrs(class pfr_peers &peers);
        pfr_asbrs(std::map<int, pfr_peer> &mm);

        void set_asbr(int id, std::string ip, class pfr_asbr_parm &parm);
        class pfr_asbr_parm get_asbr(std::string ip);
        class pfr_asbr_parm get_asbr(int id_peer);
        std::string get_asbr_lo(int id_peer);
};

#endif // PFR_RTR_H
