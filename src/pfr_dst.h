#ifndef PFR_DST_H
#define PFR_DST_H
#include <string>

class pfr_dst {
    int id;
    std::string ipv4_dst_address;
    std::string ipv6_dst_address;
    std::string description;

  public:
    pfr_dst(int id, std::string ipv4_dst_address, std::string ipv6_dst_address , std::string description);
    int get_id();
    std::string get_ipv4();
    void pfr_dst_print();
};

#endif // PFR_DST_H
