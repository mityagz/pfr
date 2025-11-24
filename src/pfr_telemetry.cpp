#include "pfr_telemetry.h"
#include <unistd.h>


void *telemetry_peers(void *p) {
    for(;;) {
       sleep(64);
    }
    return NULL;
}
