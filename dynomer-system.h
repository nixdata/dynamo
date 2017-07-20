#pragma once
#include <cstdint>
#include <stdbool.h>
#include <time.h>


#ifdef __cplusplus
    extern "C" {
#endif


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


struct dyno_time_t {
    long seconds;
    long nanoseconds;
};


int dyno_sys_init();

struct dyno_time_t dyno_sys_hires_time();


#ifdef __cplusplus
    }
#endif