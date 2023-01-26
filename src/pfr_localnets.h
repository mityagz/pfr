#ifndef PFR_LOCALNETS_H
#define PFR_LOCALNETS_H

#include <string>
#include <vector>

class pfr_localnets {
    struct net {
     uint32_t net;
     uint32_t mask;
     uint32_t net_p_mask;
    };
    std::vector<net *> nets;
  public:
    pfr_localnets();
    pfr_localnets(const pfr_localnets &);
    pfr_localnets(std::string localnets);
    ~pfr_localnets();
 
    bool is_host_in_network(std::string hoststr);
};

#endif
