#include "pfr_customer.h"
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

extern std::shared_ptr<spdlog::logger> syslog_logger;
extern int customer_id;

// This func converts ip[46] addr string to int
ssize_t ip4_bin(int family, std::string srcaddr) {
 ssize_t i = 0;
 const char *ip = srcaddr.c_str();
 struct sockaddr_in sa;
 inet_pton(family, ip, &(sa.sin_addr));
 i = ntohl(sa.sin_addr.s_addr);
 return i;
}

int is46_addr(std::string ip) {
 std::size_t f = ip.find(".");
 if (f != std::string::npos)
  return 4;
 return 6;
}

pfr_prefix4::pfr_prefix4(std::string sprefix) : prefix(sprefix) {
 std::string slen = prefix.substr(prefix.find('/') + 1);
 std::string slor = prefix.substr(0, prefix.find('/'));
 std::string tslor = slor;
 int prefix_len = std::stoi(slen);
 lor = ip4_bin(AF_INET, slor);
 int lor0, lor1, lor2, lor3;
 int hir0, hir1, hir2, hir3;
 int idx = 0;
 std::string shir[4];
 ssize_t pos = 0;
 std::string token;
 std::string delim = ".";
 while ((pos = tslor.find(delim)) != std::string::npos) {
  token = tslor.substr(0, pos);
  shir[idx] = token;
  tslor.erase(0, pos + delim.length());
  idx++;
 }
 shir[idx] = tslor;

 int hbits = 0;
 int oct = 0;
 std::string sshir;
 if(prefix_len >= 0 && prefix_len < 8) {
    //hbits = std::pow(2, (8 - prefix_len)) - 1;
    hbits = 255 >> 8 - (8 - prefix_len);
    oct = std::stoi(shir[0]) | hbits;
    sshir = std::to_string(oct) + ".255.255.255";
 } else if(prefix_len >= 8 && prefix_len < 16) {
    //hbits = std::pow(2, (16 - prefix_len)) - 1;
    hbits = 255 >> 8 - (16 - prefix_len);
    oct = std::stoi(shir[1]) | hbits;
    sshir = shir[0] + "." + std::to_string(oct) + ".255.255";
 } else if(prefix_len >= 16 && prefix_len < 24) {
    //hbits = std::pow(2, (24 - prefix_len)) - 1;
    hbits = 255 >> 8 - (24 - prefix_len);
    oct = std::stoi(shir[2]) | hbits;
    sshir = shir[0] + "." + shir[1] + "." + std::to_string(oct) + ".255";
 } else if(prefix_len >= 24 && prefix_len <= 32) {
    // hbits = std::pow(2, (32 - prefix_len)) - 1;
    hbits = 255 >> 8 - (32 - prefix_len);
    oct = std::stoi(shir[3]) | hbits;
    sshir = shir[0] + "." + shir[1] + "." + shir[2] + "." + std::to_string(oct);
 }
 hir = ip4_bin(AF_INET, sshir);
 syslog_logger->debug("pfr_prefix4() customer range {} {}:{}", prefix, slor, sshir);
}

std::string pfr_prefix4::get_prefix() { return prefix; }
int pfr_prefix4::get_lor() { return lor; }
int pfr_prefix4::get_hir() { return hir; }

bool pfr_prefix4::ispfxhit(std::string srcaddr4) { 
 int pfx = ip4_bin(AF_INET, srcaddr4);
 if(pfx >= lor && pfx <= hir)
    return true; 
 return false; 
}

pfr_prefix6::pfr_prefix6(std::string prefix) {}
std::string pfr_prefix6::get_prefix() { return prefix; }
int pfr_prefix6::get_lor() { return lor; }
int pfr_prefix6::get_hir() { return hir; }

bool pfr_prefix6::ispfxhit(std::string srcaddr6) { 
 int pfx = ip4_bin(AF_INET6, srcaddr6);
 if(pfx >= lor && pfx <= hir)
    return true; 
 return false; 
}
     
pfr_customer::pfr_customer() {}
pfr_customer::pfr_customer(int customer_id) {
 std::string p0, p1, p2, p3, p4;
 if(customer_id == 7) {
  p0 = "1.0.5.128/25";
  p1 = "1.0.6.0/27";
  p2 = "1.0.8.0/22";
  p3 = "1.248.0.0/14";
  p4 = "128.0.0.0/6";
 } 
 prefix4.push_back(pfr_prefix4(p0));
 prefix4.push_back(pfr_prefix4(p1));
 prefix4.push_back(pfr_prefix4(p2));
 prefix4.push_back(pfr_prefix4(p3));
 prefix4.push_back(pfr_prefix4(p4));
}

bool pfr_customer::ispfxhit(std::string srcaddr) { 
 if(is46_addr(srcaddr) == 4) {
  for(int i = 0; i < prefix4.size(); i++) {
   if(prefix4[i].ispfxhit(srcaddr))
    return true;
  }
 } else {
  for(int i = 0; i < prefix6.size(); i++) {
   if(prefix6[i].ispfxhit(srcaddr))
    return true;
  }
 }    
 return false; 
}
