#ifndef PFR_SQL_H
#define PFR_SQL_H
#include <string>

class pfr_sql {
    int id;
    std::string ipv4_dst_address;
    std::string ipv6_dst_address;
    std::string description;

  public:
    pfr_sql(int id, std::string ipv4_dst_address, std::string ipv6_dst_address , std::string description);
    int pfr_sql_get_id();
    std::string pfr_sql_get_ipv4();
    std::string pfr_sql_get_ipv6();
    std::string pfr_sql_get_descr();
};

#endif // PFR_DST_H
