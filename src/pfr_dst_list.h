#ifndef PFR_DST_LIST_H
#define PFR_DST_LIST_H

#include <vector>
#include <map>
#include "pfr_dst.h"
#include "pfr_sql_list.h"
#include "pfr_customer.h"

class pfr_dst_list {
    int nlist;
    std::vector<pfr_dst> pfrDstList;

  public:
    class iterator;
    friend class iterator;

    class iterator {
     int index;
     pfr_dst_list &dst_list;
     public:

      iterator(pfr_dst_list &dl) : dst_list(dl), index(0) {}
      iterator(pfr_dst_list &dl, bool) : dst_list(dl), index(dst_list.pfrDstList.size()) {}

      /*
      pfr_dst operator++(int) {
        return dst_list.pfrDstList[index++]; 
      }
      */

      bool operator!=(const iterator& rv) const { return index != rv.index; }
      int operator++(int) {
        return index++;
      }

      pfr_dst operator*() const {
        return dst_list.pfrDstList[index]; 
      }

      int print() { return index; }
    };

    pfr_dst_list();
    pfr_dst_list(int nlist);
    pfr_dst_list(int nlist, int nnlist, pfr_customer &cust);
    pfr_dst_list(int nlist, int nnlist, pfr_dst_list &prevdstList, pfr_customer &cust);
    void pfr_dst_print();
    int size();
    pfr_sql_list get_pfr_dst_sql();
    std::map<std::string, bool> pfr_dst_getmap();
    iterator begin() { return iterator(*this); }
    iterator end() { return iterator(*this, true); }
};

#endif // PFR_DST_LIST_H
