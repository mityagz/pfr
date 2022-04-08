#ifndef PFR_SQL_LIST_H
#define PFR_SQL_LIST_H

#include <vector>
#include "pfr_sql.h"

class pfr_sql_list {
    int nlist;
    std::vector<pfr_sql> pfrSqlList;

  public:
    pfr_sql_list();
    void pfr_sql_list_add(pfr_sql);
    void pfr_sql_print();
    std::vector<pfr_sql> get_pfr_dst_sql();
    std::vector<pfr_sql> get();
};

#endif // PFR_DST_LIST_H
