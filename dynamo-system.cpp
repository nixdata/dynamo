#include <time.h>
#include <stdio.h>
#include "dynamo-system.h"


struct dmo_time dmo_sys_time() 
{
    struct dmo_time dmo_time = {0, 0};
    struct timespec time_spec = {0}; 
    clock_gettime(CLOCK_MONOTONIC, &time_spec);
    dmo_time.seconds = time_spec.tv_sec;
    dmo_time.nanoseconds = time_spec.tv_nsec;

    return dmo_time;
}


double dmo_sys_time_to_double(struct dmo_time dmo_time)
{
    return (double)dmo_time.seconds + ((double)dmo_time.nanoseconds / 1000000000);
}


void dmo_sys_sleep(dmo_time sleep_time)
{
    struct timespec ts;
    ts.tv_sec = sleep_time.seconds;
    ts.tv_nsec = sleep_time.nanoseconds;
    nanosleep(&ts, NULL);
}
