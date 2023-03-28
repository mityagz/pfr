#include <cstring>
#include <climits>
#include <iostream>
#include <map>
#include <malloc.h>
#include <sys/time.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ping0.h"
#include "ping_data.h"

struct proto    proto_v4 = { proc_v4, NULL, NULL, NULL, NULL, 0, IPPROTO_ICMP };

#ifdef  IPV6
struct proto    proto_v6 = { proc_v6, send_v6, init_v6, NULL, NULL, 0, IPPROTO_ICMPV6 };
#endif

extern int pfr_ping_req;
extern std::string src_addr;
extern int usleep_between_echo;
extern int max_load;
extern int df;
extern int dscp;

//icmp_payload icmp_d;
//int datalen = 56;

template <typename T>
T swap_endian(T u) {
static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");
union {
    T u;
    unsigned char u8[sizeof(T)];
} source, dest;

source.u = u;
    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];
    
    return dest.u;
}



int set_dscp(int fd, uint8_t dscp) {
 int val;
 bool success;
               
 if (dscp > 63) {
  return EINVAL;
 }
                    
 success = false;
 val = dscp << 2;
 if (setsockopt(fd, IPPROTO_IP, IP_TOS, &val, sizeof val)) {
 } else {
   success = true;
 }
 if (setsockopt(fd, IPPROTO_IPV6, IPV6_TCLASS, &val, sizeof val)) {
 } else {
     success = true;
 }
 if (!success) {
   return ENOPROTOOPT;
 }
                              
 return 0;
}

void send_v4(idata *pin, pthread_t pt) {
    //pthread_detach(pthread_self());

    pthread_t thId = pt;

    int len;
    struct icmp *icmp;

    int size;
    char    recvbuf[BUFSIZE];
    char    controlbuf[BUFSIZE];
    struct msghdr   msg;
    struct iovec    iov;
    ssize_t n;
    struct timeval  tval;
    struct addrinfo *ai;
    char *h;
    int sockwr;

    sigset_t set, orig; 
    sigemptyset(&set);

    char *host0 = "10.229.4.0";
    opterr = 0;     // don't want getopt() writing to stderr

    //ai = Host_serv(host0, NULL, 0, 0);
    //h = Sock_ntop_host(ai->ai_addr, ai->ai_addrlen);
    //
    //struct addrinfo hints, *ai;
    struct addrinfo hints;
    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_CANONNAME;  /* always return canonical name */
    hints.ai_family = 0;       /* 0, AF_INET, AF_INET6, etc. */
    hints.ai_socktype = 0;   /* 0, SOCK_STREAM, SOCK_DGRAM, etc. */
    getaddrinfo(host0, NULL, &hints, &ai);

    if (ai->ai_family == AF_INET) {
                    pr = &proto_v4;
#ifdef  IPV6
    } else if (ai->ai_family == AF_INET6) {
                    pr = &proto_v6;
                    if (IN6_IS_ADDR_V4MAPPED(&(((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr)))
                        err_quit("cannot ping IPv4-mapped IPv6 address");
#endif
    } else
                err_quit("unknown address family %d", ai->ai_family);
    pr->sasend = ai->ai_addr;
    //pr->sarecv = (sockaddr*)Calloc(1, ai->ai_addrlen);
    pr->salen = ai->ai_addrlen;

    sockwr = Socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);

    //
    /* bind to an address */
    struct sockaddr_in serveraddr;
    //int s;
    memset(&serveraddr, 0x00, sizeof(struct sockaddr_in));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(0);
    serveraddr.sin_addr.s_addr = inet_addr(src_addr.c_str()); // config_t

    int rc = bind(sockwr, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    setuid(getuid());               // don't need special permissions any more
    if (pr->finit)
        (*pr->finit)();
    size = 60 * 1024;               // OK if setsockopt fails
    setsockopt(sockwr, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    if(df) {
#if  defined(__FreeBSD__)
        int val = 1;
        setsockopt(sockwr, IPPROTO_IP, IP_DONTFRAG, &val, sizeof(val));
#else
        int val = IP_PMTUDISC_DO;
        setsockopt(sockwr, IPPROTO_IP, IP_MTU_DISCOVER, &val, sizeof(val));
#endif
    }

    if(dscp) {
     set_dscp(sockwr, dscp);
    }

    pid = getpid() & 0xffff;    // ICMP ID field is 16 bits 

    idata *in = (idata*)pin;
    auto k = in->mm.find(in->peer_id);
    
    freeaddrinfo(ai);

    /*
    pthread_mutex_lock(&in->mtx[0]);
    std::cout << "Peer_id: " << k->first << std::endl;
    std::cout << k->first << ":" << (k->second).pfr_peer_get_id() << ":" << (k->second).pfr_peer_get_pe_ip() << std::endl;
    std::cout << "Peer_id: " << in->peer_id << ": Probe_id: " << in->probe_id << ":" << ":" << std::endl;
    std::cout << "ThreadId: " << thId << std::endl;
    std::cout << in->peer_id << ":" << in->probe_id << ":" << ":" << std::endl;
    pthread_mutex_unlock(&in->mtx[0]);
    */

    pfr_dst_list::iterator it = in->dst_list->begin();
    pfr_dst_list::iterator ite = in->dst_list->end();


    while(it != ite) {
        /*
        pthread_mutex_lock(&in->mtx[0]);
        std::cout << (*it).get_id() << ":ext:" << (*it).get_ipv4() << std::endl;
        pthread_mutex_unlock(&in->mtx[0]);
        */
        int hid = (*it).get_id();
        std::string hip = (*it).get_ipv4();

        //ai = Host_serv(((*it).get_ipv4()).std::string::c_str(), NULL, 0, 0);
        //h = Sock_ntop_host(ai->ai_addr, ai->ai_addrlen);
        getaddrinfo(((*it).get_ipv4()).std::string::c_str(), NULL, &hints, &ai);

        pr->sasend = ai->ai_addr;
        //pr->sarecv = (sockaddr*)Calloc(1, ai->ai_addrlen);
        pr->salen = ai->ai_addrlen;


        icmp = (struct icmp *) sendbuf;
        icmp->icmp_type = ICMP_ECHO;
        icmp->icmp_code = 0;
        icmp->icmp_id = pid;
        //icmp->icmp_seq = nsent++;
        //memset(icmp->icmp_data, 0xa5, datalen); // fill with pattern
        //Gettimeofday((struct timeval *) icmp->icmp_data, NULL);
    
        // struct icmp_d fills 
       //
        for(int seq = 0; seq < pfr_ping_req; seq++) {
            icmp->icmp_seq = seq;
            //icmp_payload *icmp_d = (icmp_payload *)malloc(sizeof(icmp_payload));
            icmp_payload *icmp_d = (icmp_payload *)malloc(sizeof(icmp_payload) + max_load);
            struct timeval  *stv = (struct timeval *)malloc(sizeof(struct timeval));
            if(max_load > 0)
             memset(icmp_d, 0xa5, sizeof(icmp_payload) + max_load); // fill with pattern
            //Gettimeofday((struct timeval *) icmp_d->tv, NULL);
            //Gettimeofday((struct timeval *) stv, NULL);
            Gettimeofday((struct timeval *) &(icmp_d->tv), NULL);
            //icmp_d->tv = *stv;

            icmp_d->peer_id = swap_endian<uint32_t>(in->peer_id);
            icmp_d->probe_id = swap_endian<uint64_t>(in->probe_id);
            icmp_d->thread_id = swap_endian<uint64_t>((uint64_t)thId);
            icmp_d->timestamp = swap_endian<uint64_t>((unsigned long)time(NULL));


            //memcpy(sendbuf + 8, icmp_d, sizeof(icmp_payload));
            memcpy(sendbuf + 8, icmp_d, sizeof(icmp_payload) + max_load);

            //len = 8 + sizeof(icmp_payload);      // checksum ICMP header and data
            len = 8 + sizeof(icmp_payload) + max_load;      // checksum ICMP header and data
            icmp->icmp_cksum = 0;
            icmp->icmp_cksum = in_cksum((u_short *) icmp, len);

            pthread_mutex_lock(&in->mtx[0]);
            // by mitya 15.07.2022
            ///std::cout << "Peer_id:" << in->peer_id << " Probe_id:" << in->probe_id << " sockwr:" << sockwr << " id:" << (*it).get_id() << " seq:" << seq << " thid:" << thId << ":" << (*it).get_ipv4(); // << std::endl;
            pthread_mutex_unlock(&in->mtx[0]);
            
            /*
            sigaddset(&set, SIGINT);
            sigaddset(&set, SIGKILL);
            for (int s = SIGRTMIN; s <= SIGRTMAX-4; s++) {
                    sigaddset(&set, s);
            }
            sigemptyset(&orig);
            pthread_sigmask(SIG_BLOCK, &set, &orig);
            */

            int ret = 0;
            if((ret = sendto(sockwr, sendbuf, len, 0, pr->sasend, pr->salen)) == -1) {
                pthread_mutex_lock(&in->mtx[0]);
                // by mitya 16.07.2022
                ///std::cout << "ThreadId: " << thId << std::endl;
                ///std::cout << "ret: " << ret << std::endl;
                ///std::cout << "EXIT(1)" << std::endl;
                pthread_mutex_unlock(&in->mtx[0]);
                
                //exit(1);
            }
            //pthread_sigmask(SIG_SETMASK, &orig, 0);

                pthread_mutex_lock(&in->mtx[0]);
                // by mitya 15.07.2022
                ///std::cout << ":ret: " << ret << std::endl;
                pthread_mutex_unlock(&in->mtx[0]);
            //Sendto(sockwr, sendbuf, len, 0, pr->sasend, pr->salen);
            free(stv);
            free(icmp_d);
            //usleep(3000);
            //usleep(2500); //config_t
            usleep(usleep_between_echo); //config_t
      } 
      it++;
       //usleep(20000);
      //usleep(2000);
      //usleep(200);
      //usleep(20);
      freeaddrinfo(ai);
    }
    close(sockwr);
    /*
    pthread_mutex_lock(&in->mtx[0]);
    std::cout << "ThreadId: " << thId << std::endl;
    std::cout << in->peer_id << ":" << in->probe_id << ":" << ":" << std::endl;
    pthread_mutex_unlock(&in->mtx[0]);
    */
    return;
}
