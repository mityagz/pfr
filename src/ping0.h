#ifndef PING_H
#define PING_H

#include "unp.h"
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <cstddef>
#include <map>
#include "pfr_peers.h"
#include "pfr_dst_list.h"

#define BUFSIZE     1500

/* globals */
extern char     sendbuf[BUFSIZE];

typedef struct {
    std::map<int, pfr_peer> mm;
    int peer_id;
    int probe_id;
    pfr_dst_list *dst_list;
    pthread_mutex_t *mtx;
} idata;

extern int      datalen;           /* # bytes of data following ICMP header */
extern char    *host;
extern int      nsent;             /* add 1 for each sendto() */
extern pid_t    pid;               /* our PID */
//extern int      sockfd;
extern int      verbose;

/* function prototypes */
void     init_v6(void);
void     proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
void     proc_v6(char *, ssize_t, struct msghdr *, struct timeval *);
void     send_v4(idata *, pthread_t);
//void     send_v4(void);
void     send_v6(void);
void     readloop(void);
void     sig_alrm(int);
void     tv_sub(struct timeval *, struct timeval *);

extern struct proto {
    void   (*fproc)(char *, ssize_t, struct msghdr *, struct timeval *);
    void   (*fsend)(void);
    void   (*finit)(void);
    struct sockaddr  *sasend;   /* sockaddr{} for send, from getaddrinfo */
    struct sockaddr  *sarecv; /* sockaddr{} for receiving */
    socklen_t       salen;      /* length of sockaddr{}s */
    int           icmpproto;  /* IPPROTO_xxx value for ICMP */
} *pr;

#ifdef  IPV6

#include    <netinet/ip6.h>
#include    <netinet/icmp6.h>

#endif

#endif
