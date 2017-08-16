#pragma once
#include "dynamo-system.h"

#ifdef __cplusplus
    extern "C" {
#endif

#define DYNAMO_OK 0
#define DYNAMO_ERROR 1

#define NANOS_IN_SECOND 1000000000
#define SIM_UPDATE_RATE 16666666 // 60Hz
#define NET_UPDATE_RATE 16666666 // 60Hz
#define GFX_UPDATE_RATE 8333333  // 120Hz
#define FRAME_TIME_MAX 500000000

#ifdef __cplusplus
    }
#endif
