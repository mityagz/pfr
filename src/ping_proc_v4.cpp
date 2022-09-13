#include <math.h>
#include    "ping0.h"
#include    "ping_data.h"
#include    "pfr_rdata.h"
#include <climits>


extern int proc_v4_new_cnt;
extern pthread_mutex_t mtr;
//       |dst_ip               |probe_id      |peer_id     |seq_num
extern std::map<std::string, std::map<int, std::map<int, std::map<int, tparm *>>>> r;

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

void proc_v4(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv) {
    int hlen1, icmplen;
    double          rtt;
    struct ip       *ip;
    struct icmp     *icmp;
    struct timeval  *tvsend;
    ip = (struct ip *) ptr;     /* start of IP header */
    hlen1 = ip->ip_hl << 2;     /* length of IP header */
    if (ip->ip_p != IPPROTO_ICMP)
        return;             /* not ICMP */
    icmp = (struct icmp *) (ptr + hlen1);   /* start of ICMP header */
    if ( (icmplen = len - hlen1) < 8)
        return;             /* malformed packet */
    if (icmp->icmp_type == ICMP_ECHOREPLY) {
        if (icmp->icmp_id != pid)
            return;         /* not a response to our ECHO_REQUEST */
        if (icmplen < 16)
            return;         /* not enough data to use */
        //tvsend = (struct timeval *) icmp->icmp_data;
        icmp_payload *icmp_d = (icmp_payload *) icmp->icmp_data;
        tvsend = (struct timeval *) &(icmp_d->tv);
        tv_sub(tvrecv, tvsend);
        rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;

        struct in_addr src = ip->ip_src;
        struct in_addr dst = ip->ip_dst;

        char str_src[INET_ADDRSTRLEN];
        char str_dst[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(src), str_src,  INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(dst), str_dst,  INET_ADDRSTRLEN);

        uint32_t peer_id = swap_endian<uint32_t>(icmp_d->peer_id);
        uint64_t probe_id = swap_endian<uint64_t>(icmp_d->probe_id);
        uint64_t thread_id = swap_endian<uint64_t>(icmp_d->thread_id);
        uint64_t timestamp = swap_endian<uint64_t>(icmp_d->timestamp);


        //printf("%d bytes from %s: seq=%u, ttl=%d, rtt=%.3f ms\n", icmplen, Sock_ntop_host(pr->sarecv, pr->salen), icmp->icmp_seq, ip->ip_ttl, rtt);
        ///printf("%d bytes from %s: to: %s seq=%u, ttl=%d, rtt=%.3f ms peer_id=%u, probe_id=%lu, thread_id=%lu, timestamp=%lu\n", icmplen, str_src, str_dst, icmp->icmp_seq, ip->ip_ttl, rtt, peer_id, probe_id, thread_id, timestamp);
        
        //pthread_mutex_lock(&mtr);
        if(r[str_src][probe_id][peer_id][icmp->icmp_seq] == NULL) {
         r[str_src][probe_id][peer_id][icmp->icmp_seq] = new tparm(rtt, 0, 0, timestamp);
         proc_v4_new_cnt++;
        }
        //pthread_mutex_unlock(&mtr);

    } else if (verbose) {
        printf("  %d bytes from %s: type = %d, code = %d\n", icmplen, Sock_ntop_host(pr->sarecv, pr->salen), icmp->icmp_type, icmp->icmp_code);
        ;
    }
}
