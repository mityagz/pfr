#include <vector>
#include "pfr_sql.h"
#include "pfr_sql_list.h"

pfr_sql_list::pfr_sql_list() {}
void pfr_sql_list::pfr_sql_list_add(pfr_sql e) {
    pfrSqlList.push_back(e);
}
void pfr_sql_list::pfr_sql_print() {}
std::vector<pfr_sql> pfr_sql_list::get_pfr_dst_sql() { return pfrSqlList; }
std::vector<pfr_sql> pfr_sql_list::get() { return pfrSqlList; }
