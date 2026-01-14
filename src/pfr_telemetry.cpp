#include "pfr_telemetry.h"
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include "pfr_asbrs.h"

extern std::shared_ptr<spdlog::logger> syslog_logger;

tperf_peer::tperf_peer(std::string description, int peer_id, double load, double bandwidth, double capacity, double delay, \
           double throughput, \
           double jitter, double packet_loss, double utilization) : \
           description(description), peer_id(peer_id), load(load), bandwidth(bandwidth), capacity(capacity), delay(delay), \
           throughput(throughput), \
           jitter(jitter), packet_loss(packet_loss), utilization(utilization) {
           }; //(also called load)

pfr_perf_peers_cont::pfr_perf_peers_cont() {};
pfr_perf_peers_cont::pfr_perf_peers_cont(std::string pe_ip, int peer_id, int perf_id, tperf_peer *tp) {};

void pfr_perf_peers_cont::pfr_perf_peers_cont::add(std::string pe_ip, int peer_id, int perf_id, tperf_peer *tp) {
    pthread_mutex_lock(&mtx_perf); 
    ppm[pe_ip][peer_id][perf_id] = tp;
    pthread_mutex_unlock(&mtx_perf); 
};

tperf_peer* pfr_perf_peers_cont::del(std::string pe_ip, int peer_id, int perf_id, tperf_peer *tp) {};

tperf_peer* pfr_perf_peers_cont::cleanup(std::string pe_ip, int peer_id, int perf_id, int perf_deep, tperf_peer *tp) {
    int perf_min = perf_id - perf_deep;
    if(perf_min < 0)
        return NULL;
    pthread_mutex_lock(&mtx_perf); 
    auto p = ppm[pe_ip][peer_id][perf_min];
    ppm[pe_ip][peer_id].erase(perf_min);
    pthread_mutex_unlock(&mtx_perf); 
    return p;
};

tperf_peer* pfr_perf_peers_cont::get(std::string pe_ip, int peer_id, int perf_id, tperf_peer *tp) {
    pthread_mutex_lock(&mtx_perf); 
    auto p = ppm[pe_ip][peer_id][perf_id];
    pthread_mutex_unlock(&mtx_perf); 
    return p;
}

void pfr_perf_peers_cont::log() {
    pthread_mutex_lock(&mtx_perf); 
    for(std::map<std::string, std::map<int, std::map<int, tperf_peer *>>>::iterator it0 = ppm.begin(); it0 != ppm.end(); it0++) {
        std::string dst_ip = it0->first;
        for(std::map<int, std::map<int, tperf_peer *>>::iterator it1 = ppm[dst_ip].begin(); it1 != ppm[dst_ip].end(); it1++) {
            int peer_id = it1->first;
            for(std::map<int, tperf_peer *>::iterator it2 = ppm[dst_ip][peer_id].begin(); it2 != ppm[dst_ip][peer_id].end(); it2++) {
                int perf_id = it2->first;
                auto tp = it2->second;
                syslog_logger->debug("pfr_perf_peers_cont(): probe_id : {} : {} : {}", dst_ip, peer_id, perf_id);
            }
        }
    }
    pthread_mutex_unlock(&mtx_perf); 
}

void *telemetry_peers(void *p) {
    for(;;) {
       sleep(64);
    }
    return NULL;
}

void *stream_peers(void *p) {
    for(;;) {
       sleep(64);
    }
    return NULL;
}

void *performance_peers(void *p) {
    pthread_t t = pthread_self();
    perf_peers_input *ppi = (perf_peers_input *)p;
    std::string pe_ip = ppi->pe_ip;
    auto asbr_peers = ppi->p;
    auto nconf = ppi->parm;
    auto pppc = ppi->pppc;
    int perf_id = 0;
    //auto syslog_logger = ppi->syslog_logger;
        syslog_logger->debug("--------------------------------------------------");
        syslog_logger->debug("- tid: {}", t);
        syslog_logger->debug("- perf br: {}", pe_ip);
        syslog_logger->debug("--------------------------------------------------");
    
        struct timeval tv;
        int retval;
        tv.tv_sec = 64;
        tv.tv_usec = 0;
        sigset_t set;
        int s;

        /* Block SIGQUIT and SIGUSR1; other threads created by main()
           will inherit a copy of the signal mask. */

       sigemptyset(&set);
       sigaddset(&set, SIGQUIT);
       sigaddset(&set, SIGUSR1);
       sigaddset(&set, SIGUSR2);
       sigaddset(&set, SIGINT);
       s = pthread_sigmask(SIG_BLOCK, &set, NULL);
                                                                                   
        //auto nconn = nconf->get_asbr(pe_ip);
        auto nconn = nconf->get_asbr(pe_ip);
    
    for(;;) {
        syslog_logger->debug("--------------------------------------------------");
        syslog_logger->debug("tid: {}", t);
        syslog_logger->debug("perf br: {} : {}", pe_ip, (void *)nconn.get_session());
        auto peers = (*asbr_peers)[pe_ip];
        // map peer_id to asbr loopback
        for(std::map<int, pfr_peer>::iterator itp = peers.begin(); itp != peers.end(); ++itp) {
            int p_id = itp->first;
            auto peer = itp->second;
            auto *tp = new tperf_peer(pe_ip, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            pppc->add(pe_ip, peer.pfr_peer_get_id(), perf_id, tp);
            syslog_logger->debug("pfr_asbr_peers(): {} : {} : {} : {} : {} : {}", pe_ip, perf_id, peer.pfr_peer_get_id(), peer.get_interface_name(), peer.get_interface_unit(), (void *) tp);
            syslog_logger->debug("-------------------------------------------------------------------");
            auto p = pppc->cleanup(pe_ip, peer.pfr_peer_get_id(), perf_id, 2, NULL);
            delete p;
        }
        tv.tv_sec = 64;
        tv.tv_usec = 0;
        do {
            retval = select(1, NULL, NULL, NULL, &tv);
            if(retval != -1) break;
            sleep(1);
        } while(retval == -1);
        perf_id++;
        syslog_logger->debug("pfr_asbr_peers() retval: {}", retval);
    }
    return NULL;
}
