#include <time.h>
#include "dynomer-system.h"

static clockid_t clock_id;
static timespec time_spec; 


int dyno_sys_init()
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


struct dyno_time_t dyno_sys_hires_time() 
{
    struct dyno_time_t dyno_time = {0, 0};
    clock_gettime(clock_id, &time_spec);
    dyno_time.seconds = time_spec.tv_sec;
    dyno_time.nanoseconds = time_spec.tv_nsec;

    return dyno_time;
}
