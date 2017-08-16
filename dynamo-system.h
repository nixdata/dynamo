#pragma once
#include <cstdint>
#include <stdbool.h>
#include <time.h>
#include <signal.h>


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


struct dmo_time {
    long seconds;
    long nanoseconds;
};


struct dmo_time dmo_sys_time();
double dmo_sys_time_to_double(struct dmo_time dmo_time);
void dmo_sys_sleep(struct dmo_time);
void dmo_sys_interrupt_handler(int signal);

#ifdef __cplusplus
    }
#endif
