#ifndef PFR_DST_LIST_H
#define PFR_DST_LIST_H

#include <vector>
#include "pfr_dst.h"
#include "pfr_sql_list.h"

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

    pfr_dst_list(int nlist);
    pfr_dst_list(int nlist, int nnlist);
    void pfr_dst_print();
    pfr_sql_list get_pfr_dst_sql();
    iterator begin() { return iterator(*this); }
    iterator end() { return iterator(*this, true); }
};

#endif // PFR_DST_LIST_H
