#include <time.h>
#include "dynamo-system.h"


static clockid_t clock_id;
static timespec time_spec; 


int dmo_sys_init()
{
    if(clock_getcpuclockid(0, &clock_id) != 0) {
        // TODO: How are you dealing with runtime errors?
        return 1;
    }

    time_spec = {0};

    if(clock_gettime(clock_id, &time_spec) != 0) {
        return 1;
    }

    return 0;
}


struct dmo_time_t dmo_sys_time() 
{
    struct dmo_time_t dmo_time = {0, 0};
    clock_gettime(clock_id, &time_spec);
    dmo_time.seconds = time_spec.tv_sec;
    dmo_time.nanoseconds = time_spec.tv_nsec;

    return dmo_time;
}
