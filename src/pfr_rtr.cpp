#include <map>
#include <string>
#include <iostream>
#include <unistd.h>
#include <libnetconf.h>
#include <libnetconf_ssh.h>
#include "pfr_dst.h"
#include "pfr_dst_list.h"
#include "pfr_peers.h"
#include "pfr_rdata.h"
#include "pfr_rtr.h"
#include "ping0.h"

extern std::map<std::string, std::map<int, rt_parm *>> route;

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

/*
char * pw(const char *u, const char *h) { 
     char *pass = "pp";
     char *p = (char *)malloc(strlen(pass));
     strcpy(p, pass);
     return p; 
}

char * pw0(const char *username, const char *hostname, const char *priv_key_file) {  
     char *pass = "pp";
     char *p = (char *)malloc(strlen(pass));
     strcpy(p, pass);
     return priv_key_file; 
}
*/

pfr_asbr_parm::pfr_asbr_parm() {}
pfr_asbr_parm::pfr_asbr_parm(std::string peer_name, std::string ip) {
 struct nc_session *ncsession;   
 NC_VERB_LEVEL verbose = NC_VERB_WARNING;
 nc_ssh_pref(NC_SSH_AUTH_PASSWORD, -1);
 nc_ssh_pref(NC_SSH_AUTH_INTERACTIVE, -1);
 nc_ssh_pref(NC_SSH_AUTH_PUBLIC_KEYS, 3);
 nc_verbosity(verbose);
 nc_callback_print(clb_print);
 nc_callback_error_reply(clb_error_print);
 nc_set_keypair_path("/root/.ssh/id_rsa", "/root/.ssh/id_rsa.pub");

 //char *(*func)(const char *username, const char *hostname) = pw;
 //char *(*func)(const char *username, const char *hostname, const char *priv_key_file) = pw0;   

 //void nc_callback_sshauth_passphrase  (   char *(*)(const char *username, const char *hostname, const char *priv_key_file)    func    )   
 //void nc_callback_sshauth_password    (   char *(*)(const char *username, const char *hostname)   func    )   

 //nc_callback_sshauth_password(func);   
 //nc_callback_sshauth_passphrase(func);   
 ncsession = nc_session_connect(ip.c_str(), 0, "pfr", 0); 

 if(ncsession == NULL) {
   printf("ncsession is NULL\n");
 }

 if(nc_session_get_status(ncsession) == NC_SESSION_STATUS_WORKING) {
    printf("NC_SESSION_STATUS_WORKING\n");
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
  std::cout << "ASBR Peer Id: " << peer.pfr_peer_get_id() << std::endl;
  std::cout << "ASBR Peer ip: " << peer.pfr_peer_get_pe_ip() << std::endl;
  //std::cout << "ASBR Peer name: " << peer.pfr_peer_get_pe_name() << std::endl;
  
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
  std::cout << "ASBR Peer ip of loopback: " << ip << std::endl;
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

void set_asbr(int id, std::string ip, class pfr_asbr_parm &parm) {}
class pfr_asbr_parm get_asbr(std::string ip) {}
class pfr_asbr_parm get_asbr(int id_peer) {}
std::string pfr_asbrs::get_asbr_lo(int id_peer) { return asbrIdPeer[id_peer]; }
