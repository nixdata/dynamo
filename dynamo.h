#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

// TODO: Consider bringnig dmo_system code here. Simplify!

#define DYNAMO_OK 0
#define DYNAMO_ERROR 1

#define NANOS_IN_SECOND 1000000000
#define SIM_UPDATE_RATE 16666666 // 60Hz
#define NET_UPDATE_RATE 16666666 // 60Hz
#define GFX_UPDATE_RATE 8333333  // 120Hz
#define FRAME_TIME_MAX 500000000


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
