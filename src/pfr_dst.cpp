#include <iostream>
#include <string>
#include "pfr_dst.h"

pfr_dst::pfr_dst(int id, std::string ipv4_dst_address, std::string ipv6_dst_address , std::string description) {
        this->id = id;
        this->ipv4_dst_address = ipv4_dst_address;
        this->ipv6_dst_address = ipv6_dst_address;
        this->description = description;
}

int pfr_dst::get_id() {
    return id;
}

std::string pfr_dst::get_ipv4() {
    return ipv4_dst_address;
}

void pfr_dst::pfr_dst_print() {
        std::cout << id << ':';
        std::cout << ipv4_dst_address << ':';
        std::cout << ipv6_dst_address << ':';
        std::cout << description << '\n';

}
