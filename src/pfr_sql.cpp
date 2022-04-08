#include <string>
#include "pfr_sql.h"


pfr_sql::pfr_sql(int id, std::string ipv4_dst_address, std::string ipv6_dst_address , std::string description) {
    this->id = id;
    this->ipv4_dst_address = ipv4_dst_address;
    this->ipv6_dst_address = ipv6_dst_address;
    this->description = description;
}

int pfr_sql::pfr_sql_get_id() { return id;}
std::string pfr_sql::pfr_sql_get_ipv4() { return ipv4_dst_address; }
std::string pfr_sql::pfr_sql_get_ipv6() { return ipv6_dst_address; }
std::string pfr_sql::pfr_sql_get_descr() { return description; }
