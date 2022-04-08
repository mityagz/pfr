#include "common.h"
#include <iostream>

void pfr_daemon() {
 int ret, rc;
 socklen_t slen, clen;
 int fd, select_fd, bkp_select_fd, recalc_fds, select_num;
 fd_set read_descs, bkp_read_descs;
 struct timeval tv;
 int yes = 1;

 int fdmax = 0;
 fdmax = config.bmp_sock;

 rc = listen(config.bmp_sock, 1);
 if (rc < 0) {
     printf("Listen an error: %s\n", strerror(errno));
     exit(1);
 }

 
 tv.tv_sec = 5;
 tv.tv_usec = 0;

 FD_ZERO(&bkp_read_descs);
 FD_ZERO(&read_descs);
 FD_SET(config.bmp_sock, &bkp_read_descs);
 struct timeval dump_refresh_timeout, *drt_ptr;

 char buf[256];
 int nbytes;
 int inet_addrstrlen = 256;
 char ipbuff[inet_addrstrlen];


 for(;;) {
    select_again:

    memcpy(&read_descs, &bkp_read_descs, sizeof(bkp_read_descs));
    select_num = select(fdmax + 1, &read_descs, NULL, NULL, NULL);
        if(FD_ISSET(config.bmp_sock, &read_descs)) {
            fd = accept(config.bmp_sock, (struct sockaddr *) &client, &clen);
            printf("New connection is from %s:%d on socket: %d\n", \
                    inet_ntop(AF_INET, &(((struct sockaddr_in *) &client))->sin_addr, ipbuff, sizeof(ipbuff)), \
                    ntohs(((struct sockaddr_in *) &client)->sin_port), fd);
            for(peer_id = 0; peer_id < config.bmp_daemon_max_peers; peer_id++) {
                if ((bmp_peers + peer_id)->self.fd < 0) {
                     (bmp_peers + peer_id)->self.fd = fd;
                    break;
                }
            }
            //if (peer_id == FD_SETSIZE)
                //err_quit("too many clients");
            FD_SET(fd, &bkp_read_descs);
            if(fd > fdmax)
                fdmax = fd;

            if (--select_num <= 0)
                continue;

        }
    int sockfd;

    for(peer_id = 0; peer_id < config.bmp_daemon_max_peers; peer_id++) {
        if ((sockfd = (bmp_peers + peer_id)->self.fd) < 0) 
            continue;
                peer = &((bmp_peers + peer_id)->self);
                memset(buf, 0, sizeof(buf));
                if (FD_ISSET(sockfd, &read_descs)) {
                    printf("get data from fd: %d\n", sockfd);
                 if (!peer->buf.exp_len) {
                    if(peer->buf.base == NULL)
                        bmp_peer_init(peer);
                    nbytes = recv(sockfd, &peer->buf.base[peer->buf.cur_len], (BMP_CMN_HDRLEN - peer->buf.cur_len), 0);
                    printf("TRAP0: %d\n", nbytes);
                  if (nbytes > 0) {
                    printf("Data rcvd: %s\n", peer->buf.base);
                    peer->buf.cur_len += nbytes;
                    printf("cur len: %d\n", peer->buf.cur_len);
                      if (peer->buf.cur_len  == BMP_CMN_HDRLEN) {
                          struct bmp_common_hdr *bhdr = (struct bmp_common_hdr *) peer->buf.base;
                          if (bhdr->version != BMP_V3 && bhdr->version != BMP_V4) {
                              //Log(LOG_INFO, "INFO ( %s/%s ): [%s] packet discarded: unknown BMP version: %u (1)\n",
                              //config.name, bmp_misc_db->log_str, peer->addr_str, bhdr->version);
                              peer->msglen = 0;
                              peer->buf.cur_len = 0;
                              peer->buf.exp_len = 0;
                              nbytes = -1;
                          } else {
                              peer->buf.exp_len = ntohl(bhdr->len);
                              /* commit */
                              if (peer->buf.cur_len == peer->buf.exp_len) {
                                  peer->msglen = peer->buf.exp_len;
                                  peer->buf.cur_len = 0;
                                  peer->buf.exp_len = 0;
                              }
                          }  
                      } else {
                              goto select_again;
                      }
                  } 
                 }
                 printf("TRAP3: %d\n", peer->buf.exp_len);
                  if (peer->buf.exp_len) {
                    //int sink_mode = FALSE;
                        if (peer->buf.exp_len <= peer->buf.tot_len) { 
                            nbytes = recv(peer->fd, &peer->buf.base[peer->buf.cur_len], (peer->buf.exp_len - peer->buf.cur_len), 0);
                        } else {
                            //  nbytes = recv(peer->fd, peer->buf.base, MIN(peer->buf.tot_len, (peer->buf.exp_len - peer->buf.cur_len)), 0);
                            nbytes = recv(peer->fd, peer->buf.base,  (peer->buf.exp_len - peer->buf.cur_len), 0);
                            //sink_mode = TRUE;
                            //Log(LOG_WARNING, "WARN ( %s/%s ): [%s] long BMP message received: len=%u buf=%u. Sinking.\n",
                            //    config.name, bmp_misc_db->log_str, peer->addr_str, peer->buf.exp_len, BGP_BUFFER_SIZE);
                        }

                        if (nbytes > 0) {
                            peer->buf.cur_len += ret;
                            /* commit */
                            if (peer->buf.cur_len == peer->buf.exp_len) {
                                peer->msglen = peer->buf.exp_len;
                                peer->buf.cur_len = 0;
                                peer->buf.exp_len = 0;
                            } else {
                                goto select_again;
                            }
                        }
                  } 
                  if (nbytes <= 0) {
                        //Log(LOG_INFO, "INFO ( %s/%s ): [%s] BMP connection reset by peer (%d).\n", 
                        //    config.name, bmp_misc_db->log_str, peer->addr_str, errno);
                        FD_CLR(peer->fd, &bkp_read_descs);
                        bmp_peer_close(peer);
                        //recalc_fds = TRUE;
                        goto select_again;
                  }
                 }
        }
        //bmp_process_packet(peer->buf.base, peer->msglen, bmpp, &do_term);
    }
 }

void pfr_peer_init(struct bgp_peer *peer) {
    peer->buf.base = (char *)malloc(BMP_RECV_BUF);
}

void pfr_peer_close(struct bgp_peer *peer) {
    close(peer->fd);
    free(peer->buf.base);

    peer->fd = -1;
    peer->buf.base = NULL;
    peer->buf.tot_len = 0;
    peer->buf.cur_len = 0;
    peer->buf.exp_len = 0;
}   

void main(int argc, char **argv) {
 pfr_daemon();
}
