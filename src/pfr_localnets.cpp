#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include "pfr_localnets.h"

/*
 *
 * #include <string>
 * #include <vector>
 *  
 * class pfr_localnets {
 *      struct net {
 *        uint32_t net;
 *        uint32_t mask;
 *        uint32_t net_p_mask;
 *      };
 *      std::vector<net> nets;
 *         public:
 *          pfr_localnets();
 *          pfr_localnets(const pfr_localnets &);
 *          pfr_localnets(std::string localnets);
 *          ~pfr_localnets();
 *           
 *          bool is_host_in_network(std::string hoststr);
 * };
 * */

extern std::string localnets;

pfr_localnets::pfr_localnets() {}

pfr_localnets::pfr_localnets(const pfr_localnets &) {}

pfr_localnets::pfr_localnets(std::string localnets) {
    char network_mask[15];
    std::vector<std::string> parsed_nets;
    boost::split(parsed_nets, localnets, boost::is_any_of(","), boost::token_compress_on);
    for(std::vector<std::string>::iterator prefix = parsed_nets.begin(); prefix != parsed_nets.end(); ++prefix) {
        std::vector<std::string> parsed_prefix;
        boost::split(parsed_prefix, *prefix, boost::is_any_of("/"), boost::token_compress_on);
        struct net *s_net = (struct net *)malloc(sizeof(struct net));
        inet_pton(AF_INET, parsed_prefix[0].c_str(), &s_net->net);

        unsigned long mask = (0xFFFFFFFF << (32 - atoi(parsed_prefix[1].c_str())) & 0xFFFFFFFF);
        sprintf(network_mask, "%lu.%lu.%lu.%lu", mask >> 24, (mask >> 16) & 0xFF, (mask >> 8) & 0xFF, mask & 0xFF);

        inet_pton(AF_INET, network_mask, &s_net->mask);
        nets.push_back(s_net);
    }
}

pfr_localnets::~pfr_localnets() {
    for(std::vector<struct net *>::iterator n = nets.begin(); n != nets.end(); ++n) {
        free(*n);
    }
}

bool pfr_localnets::is_host_in_network(std::string hoststr) { 
    bool match = false;
    uint32_t netmask, net, host;
    inet_pton(AF_INET, hoststr.c_str(), &host);
    for(std::vector<struct net *>::iterator n = nets.begin(); n != nets.end(); ++n) {
        if (((*n)->net & (*n)->mask) == (host & (*n)->mask))
            match = true;
    }

    return match;
}
