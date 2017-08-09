#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

#define DMO_OK 0
#define DMO_ERROR 1

#define NANOS_IN_SECOND 1000000000
#define SIM_UPDATE_RATE 16666666 // 60Hz
#define NET_UPDATE_RATE 16666666 // 60Hz
#define GFX_UPDATE_RATE 8333333  // 120Hz
#define FRAME_TIME_MAX 500000000

int dmo_startup();
void dmo_run();
int dmo_shutdown();

#ifdef __cplusplus
    }
#endif
