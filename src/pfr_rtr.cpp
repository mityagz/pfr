#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <libnetconf.h>
#include <libnetconf_ssh.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "pfr_dst.h"
#include "pfr_dst_list.h"
#include "pfr_peers.h"
#include "pfr_rdata.h"
#include "pfr_rtr.h"
#include "ping0.h"

extern std::map<std::string, std::map<int, rt_parm *>> route;
extern std::shared_ptr<spdlog::logger> syslog_logger;

extern std::string gobgp_path;

/*
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
     pfr_asbrs(std::map<int, pfr_peer> &m);
     void set_asbr(int id, std::string ip, class pfr_asbr_parm &parm);
     class pfr_asbr_parm get_asbr(std::string ip);
     class pfr_asbr_parm get_asbr(int id_peer);
     std::string get_asbr_lo(int id_peer);
};  
*/

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#define CAPABILITY_PREFIX "urn:ietf:params:netconf:"
#define ARGUMENTS "f:hl:p:vd"


void clb_print(NC_VERB_LEVEL level, const char* msg) {
    switch (level) {
        case NC_VERB_ERROR:
            fprintf(stderr, "libnetconf ERROR: %s\n", msg);
            break;
        case NC_VERB_WARNING:
            fprintf(stderr, "libnetconf WARNING: %s\n", msg);
            break;
        case NC_VERB_VERBOSE:
            fprintf(stderr, "libnetconf VERBOSE: %s\n", msg);
            break;
        case NC_VERB_DEBUG:
            fprintf(stderr, "libnetconf DEBUG: %s\n", msg);
            break;
    }
}

void clb_error_print(const char* tag,
                const char* type,
                const char* severity,
                const char* UNUSED(apptag),
                const char* UNUSED(path),
                const char* message,
                const char* UNUSED(attribute),
                const char* UNUSED(element),
                const char* UNUSED(ns),
                const char* UNUSED(sid)) {
        fprintf(stderr, "NETCONF %s: %s (%s) - %s\n", severity, tag, type, message);
}


pfr_asbr_parm::pfr_asbr_parm() {}
pfr_asbr_parm::pfr_asbr_parm(std::string peer_name, std::string ip) {
 struct nc_session *ncsession;   
 NC_VERB_LEVEL verbose = NC_VERB_WARNING;
 nc_ssh_pref(NC_SSH_AUTH_PASSWORD, -1);
 nc_ssh_pref(NC_SSH_AUTH_INTERACTIVE, -1);
 nc_ssh_pref(NC_SSH_AUTH_PUBLIC_KEYS, 3);
 nc_verbosity(verbose);
 nc_callback_print(clb_print);
 
 // config_t
 nc_callback_error_reply(clb_error_print);
 nc_set_keypair_path("/root/.ssh/id_rsa", "/root/.ssh/id_rsa.pub");

 ncsession = nc_session_connect(ip.c_str(), 0, "pfr", 0); 

 if(ncsession == NULL) {
    netconf_conn = NULL;
 }

 if(nc_session_get_status(ncsession) == NC_SESSION_STATUS_WORKING) {
    syslog_logger->debug("NC_SESSION_STATUS_WORKING");
    netconf_conn = ncsession;
 }
}

struct nc_session *pfr_asbr_parm::get_session() { return netconf_conn; }

pfr_asbrs::pfr_asbrs() {}
pfr_asbrs::pfr_asbrs(int id, std::string ip, class pfr_asbr_parm &parm) {}
pfr_asbrs::pfr_asbrs(class pfr_peers &peers) {}
pfr_asbrs::pfr_asbrs(std::map<int, pfr_peer> &p) {
 for(std::map<int, pfr_peer>::iterator itm = p.begin(); itm != p.end(); ++itm) {
  int p_id = itm->first;
  pfr_peer peer = itm->second;
  
  // map peer_id to asbr loopback
  int peer_id = peer.pfr_peer_get_id();
  std::string peer_ip = peer.pfr_peer_get_pe_ip();
  asbrIdPeer[peer_id] = peer.pfr_peer_get_pe_ip();
  auto s_peer_ip = asbrs.find(peer_ip);
  if(s_peer_ip != asbrs.end()) {
  } else {
    asbrs[peer_ip] = pfr_asbr_parm("", peer_ip);
  }
 }

 for(std::map<std::string, pfr_asbr_parm>::iterator itb = asbrs.begin(); itb != asbrs.end(); ++itb) {
  std::string ip = itb->first;
  syslog_logger->debug("ASBR Peer ip of loopback: {}", ip);
  sleep(5);
 }
}

pfr_asbrs::~pfr_asbrs() {
 for(std::map<std::string, pfr_asbr_parm>::iterator itb = asbrs.begin(); itb != asbrs.end(); ++itb) {
  std::string ip = itb->first;
  pfr_asbr_parm parm = itb->second;
  struct nc_session * s = parm.get_session();
  nc_session_free(s);
 }
}

void pfr_asbrs::set_asbr(int id, std::string ip, class pfr_asbr_parm &parm) {}
class pfr_asbr_parm pfr_asbrs::get_asbr(std::string ip) { return asbrs[ip]; }
class pfr_asbr_parm pfr_asbrs::get_asbr(int id_peer) {}
std::string pfr_asbrs::get_asbr_lo(int id_peer) { return asbrIdPeer[id_peer]; }

// https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
std::string goexec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (pipe) {
        syslog_logger->debug("goexec: {}", cmd);
    }
    if (!pipe) throw std::runtime_error("popen() failed!");
       try {
            while (fgets(buffer, sizeof buffer, pipe) != NULL) {
               result += buffer;
            }
            syslog_logger->debug("goexec result: {}", result);
       } catch (...) {
            pclose(pipe);
            throw;
       }
    pclose(pipe);
    return result;
}

// set of method for manipulation of routes on juniper routers
// we are going to use netconf and ephemerial db
void pfr_routes_man(int probe_id, std::map<int, pfr_peer> &mm, \
                    pfr_asbrs &br, std::map<std::string, std::map<int, rt_parm *>> &rroute) {
 for(std::map<std::string, std::map<int, rt_parm *>>::iterator it0 = rroute.begin(); it0 != rroute.end(); it0++) {
        std::string dst_ip = it0->first;
        int prev_peer_id = rroute[it0->first][probe_id]->get_prev_peer();
        int prev_min = rroute[it0->first][probe_id]->get_pmin_rtt();
        int curr_peer_id = rroute[it0->first][probe_id]->get_curr_peer();
        int cmin_rtt = rroute[it0->first][probe_id]->get_cmin_rtt();
        pfr_create_set_jrouter_rt(mm, br, probe_id, prev_peer_id, curr_peer_id, dst_ip);
 } 
 //
 // get routes from devices?
 // 
 //pfr_get_jrouter_rt(probe_id, prev, curr);
 //pfr_send_jrouter_rt(probe_id, prev, curr, dst_ip);
}

// get route conf from device to create route_out[][] structure if pfrd was reloaded/restarteed
void pfr_get_jrouter_rt() {}
// get route conf from device to return map of strings
std::map<std::string, std::string> pfr_get_jrouter_rt1() {}

std::vector<std::string> pfr_update_jrouter_rt() { std::vector<std::string> vs; return vs; }

int pfr_set_jrouter_rt() { return 0; }

void pfr_create_set_jrouter_rt(std::map<int, pfr_peer> &mm, pfr_asbrs &br, int probe_id, int prev_peer_id, int curr_peer_id, std::string dst_ip) {
   syslog_logger->debug("ppfr_create_set_jrouter_rt(): peer_id: {} : {} : {}", probe_id, prev_peer_id, curr_peer_id);
   if(probe_id > 0) {
    int id = (mm[curr_peer_id]).pfr_peer_get_id();
    std::string ipasbr = (mm[curr_peer_id]).pfr_peer_get_pe_ip();
    std::string nameasbr = (mm[curr_peer_id]).get_pe_name();
    std::string nh = (mm[curr_peer_id]).get_ipv4_peer_address();
    std::string comm = (mm[curr_peer_id]).get_peer_community();
        if(probe_id > 1 && prev_peer_id != 0) {
            int pid = (mm[prev_peer_id]).pfr_peer_get_id();
            std::string pipasbr = (mm[prev_peer_id]).pfr_peer_get_pe_ip();
            std::string pnameasbr = (mm[prev_peer_id]).get_pe_name();
            std::string pnh = (mm[prev_peer_id]).get_ipv4_peer_address();
            std::string pcomm = (mm[prev_peer_id]).get_peer_community();
            std::string prts = "del routing-instances i routing-options static route " + dst_ip \
                             + "/32 next-hop " + pnh + " community 3333:10000 tag " + std::to_string(pid);
            std::string exec_prts = gobgp_path + " global rib del -a ipv4 " + dst_ip + "/32 community " + pcomm;         
            goexec(exec_prts.c_str());
            std::string plo0asbr = br.get_asbr_lo(prev_peer_id);
            pfr_asbr_parm pp = br.get_asbr(plo0asbr);
            struct nc_session *pncs = pp.get_session();

            syslog_logger->debug("ppfr_create_set_jrouter_rt(): {}", plo0asbr);
            syslog_logger->debug("ppfr_create_set_jrouter_rt(): {}", prts);
        } 
        std::string rts = "set routing-instances i routing-options static route " + dst_ip \
                        + "/32 next-hop " + nh + " community 3333:10000 tag " + std::to_string(id);
        std::string exec_rts = gobgp_path + " global rib add -a ipv4 " + dst_ip + "/32 community " + comm;         
        goexec(exec_rts.c_str());
    std::string lo0asbr = br.get_asbr_lo(curr_peer_id);
    pfr_asbr_parm p = br.get_asbr(lo0asbr);
    struct nc_session *ncs = p.get_session();
    syslog_logger->debug("ppfr_create_set_jrouter_rt(): {}", lo0asbr);
    syslog_logger->debug("ppfr_create_set_jrouter_rt(): {}", rts);
    syslog_logger->debug("-------------------------------------------------------------------");
    pfr_create_xml_jrouter_rt(rts, ncs);
   }
}

void pfr_delete_set_jrouter_rt(std::map<int, pfr_peer> &mm, pfr_asbrs &br, int probe_id, int prev_peer_id, int curr_peer_id, std::string dst_ip) {
   syslog_logger->debug("ppfr_delete_set_jrouter_rt(): probe_id: {} : dst_ip {} : {} : {}", probe_id, dst_ip, prev_peer_id, curr_peer_id);
   prev_peer_id = curr_peer_id;
            int pid = (mm[prev_peer_id]).pfr_peer_get_id();
            std::string pipasbr = (mm[prev_peer_id]).pfr_peer_get_pe_ip();
            std::string pnameasbr = (mm[prev_peer_id]).get_pe_name();
            std::string pnh = (mm[prev_peer_id]).get_ipv4_peer_address();
            std::string pcomm = (mm[prev_peer_id]).get_peer_community();
            std::string prts = "del routing-instances i routing-options static route " + dst_ip \
                             + "/32 next-hop " + pnh + " community 3333:10000 tag " + std::to_string(pid);
            std::string exec_prts = gobgp_path + " global rib del -a ipv4 " + dst_ip + "/32 community " + pcomm;         
            goexec(exec_prts.c_str());
            std::string plo0asbr = br.get_asbr_lo(prev_peer_id);
            pfr_asbr_parm pp = br.get_asbr(plo0asbr);
            struct nc_session *pncs = pp.get_session();
            syslog_logger->debug("-------------------------------------------------------------------");
            syslog_logger->debug("ppfr_delete_set_jrouter_rt(): probe_id: {} : {} : {}", probe_id, dst_ip, plo0asbr);
            syslog_logger->debug("ppfr_delete_set_jrouter_rt(): {} {}", dst_ip, plo0asbr);
            syslog_logger->debug("ppfr_delete_set_jrouter_rt(): {}", prts);
            syslog_logger->debug("-------------------------------------------------------------------");
} 

void pfr_create_xml_jrouter_rt(std::string rt, struct nc_session *ncs) {
// const char *rr0 = "<rpc> \
//  <load-configuration action=\"set\"> \
//  <configuration-set>";
//  //set system host-name PE0
//  const char *rr2 = "</configuration-set> \
//  </load-configuration> \
//  </rpc>";
}
