#include <cstdlib>
#include "unp.h"

void *Calloc(size_t nuemb, size_t size) {
    void *ptr = NULL;
    if ((ptr = calloc(nuemb, size)) == NULL)
        err_sys("calloc error");
    return(ptr);
}
