#include "common.h"
#include "icmp.h"
#include <iostream>

struct proto proto_v4 = { proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_ICMP };

void pfr_pinger() {

 //struct proto proto_v4 = { proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_ICMP };
 
 int ret, rc;
 socklen_t slen, clen;
 int fd, select_fd, bkp_select_fd, recalc_fds, select_num;
 fd_set read_descs, bkp_read_descs;
 struct timeval tv;
 int yes = 1;

}

void pfr_peer_init(struct bgp_peer *peer) {
}

int main(int argc, char **argv) {
 pfr_pinger();
}
