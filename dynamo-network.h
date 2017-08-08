#pragma once

#include "dynamo-system.h"

#ifdef __cplusplus
    extern "C" {
#endif

int dmo_net_startup();
int dmo_net_update(dmo_time time);
int dmo_net_shutdown();

#ifdef __cplusplus
    }
#endif
