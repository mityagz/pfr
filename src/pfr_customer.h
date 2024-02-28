#ifndef PFR_CUSTOMER_H
#define PFR_CUSTOMER_H
#include <string>
#include <vector>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


ssize_t ip4_bin(int family, std::string ip4); 
int is46_addr(std::string ip); 

class pfr_prefix4 {
    std::string prefix;
    int lor;
    int hir;
 public:
    pfr_prefix4(std::string prefix);
    std::string get_prefix();
    int get_lor();
    int get_hir();
    bool ispfxhit(std::string srcaddr4);
};
 
class pfr_prefix6 {
    std::string prefix;
    int lor;
    int hir;
 public:
    pfr_prefix6(std::string prefix);
    std::string get_prefix();
    int get_lor();
    int get_hir();
    bool ispfxhit(std::string srcaddr6);
}; 
    
class pfr_customer {
    int customer_id;
    int autnum;
    std::string asset;
    std::vector<pfr_prefix4> prefix4;
    std::vector<pfr_prefix6> prefix6;
 public:
    pfr_customer();
    pfr_customer(int customer_id);
    bool ispfxhit(std::string srcaddr);
}; 

#endif // PFR_CUSTOMER_H
