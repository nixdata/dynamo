#pragma once

#include <cstdint>
#include <stdbool.h>
#include <time.h>

#define i8              int8_t
#define u8              uint8_t
#define i16             int16_t
#define u16             uint16_t
#define i32             int32_t
#define u32             uint32_t
#define i64             int64_t
#define u64             uint64_t
#define f32             float
#define f64             double


static clockid_t clock_id;
static timespec time_spec; 


namespace dynomer {
namespace platform {


struct dyno_time_t {
    long seconds;
    long nanoseconds;
};


int init() 
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

dyno_time_t get_high_res_time() 
{
    dyno_time_t dyno_time = {0, 0};
    clock_gettime(clock_id, &time_spec);
    dyno_time.seconds = time_spec.tv_sec;
    dyno_time.nanoseconds = time_spec.tv_nsec;

    return dyno_time;
}

}
}
