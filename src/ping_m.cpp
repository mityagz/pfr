#include    "ping.h"
#include    "ping_data.h"

//void send_v4() {
//}

//struct proto    proto_v4 = { proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_ICMP };
//struct proto    proto_v4 = { proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_ICMP };

#ifdef  IPV6
struct proto    proto_v6 = { proc_v6, send_v6, init_v6, NULL, NULL, 0, IPPROTO_ICMPV6 };
#endif

icmp_payload icmp_d;

int datalen = 56;       /* data that goes with ICMP echo request */

int mmain(int argc, char **argv) {
    int c;
    struct addrinfo *ai;
    char *h;
    char *hosts[4];

    hosts[0] = "10.229.4.0";
    hosts[1] = "10.229.6.0";
    hosts[2] = "10.229.132.0";
    hosts[3] = "10.229.134.0";

    opterr = 0;     ///* don't want getopt() writing to stderr */
    for(int i = 0; i < 4; i++) {
        host = *(hosts + i);
        pid = getpid() & 0xffff;    ///* ICMP ID field is 16 bits */
        Signal(SIGALRM, sig_alrm);
        ai = Host_serv(host, NULL, 0, 0);
        h = Sock_ntop_host(ai->ai_addr, ai->ai_addrlen);
        
        printf("PING %s (%s): %d data bytes\n", ai->ai_canonname ? ai->ai_canonname : h, h, datalen);
        ///* 4initialize according to protocol */
        if (ai->ai_family == AF_INET) {
            ; //pr = &proto_v4;
#ifdef  IPV6
        } else if (ai->ai_family == AF_INET6) {
            pr = &proto_v6;
            if (IN6_IS_ADDR_V4MAPPED(&(((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr)))
                err_quit("cannot ping IPv4-mapped IPv6 address");
#endif
        } else
            err_quit("unknown address family %d", ai->ai_family);

        pr->sasend = ai->ai_addr;
        pr->sarecv = (sockaddr*)Calloc(1, ai->ai_addrlen);
        pr->salen = ai->ai_addrlen;
        
        for(int j = 0; j < 4; j++) {
            readloop();
        }
    }
        exit(0);
}
