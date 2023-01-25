#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

bool is_host_in_network(std::string netstr, int prefix, std::string hoststr) {
        char network_mask[15];

        unsigned long mask = (0xFFFFFFFF << (32 - prefix)) & 0xFFFFFFFF;
        sprintf(network_mask, "%lu.%lu.%lu.%lu", mask >> 24, (mask >> 16) & 0xFF, (mask >> 8) & 0xFF, mask & 0xFF);

        uint32_t netmask, net, host;
        inet_pton(AF_INET, network_mask, &netmask);
        inet_pton(AF_INET, netstr.c_str(), &net);
        inet_pton(AF_INET, hoststr.c_str(), &host);

        if ((net & netmask) == (host & netmask))
            return true;
        else
            return false;
}
