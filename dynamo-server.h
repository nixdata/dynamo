#pragma once
#pragma once
#include "dynamo.h"

#ifdef __cplusplus
    extern "C" {
#endif

int dmo_server_startup();
int dmo_server_update(struct dmo_time time);
void dmo_server_run();
void dmo_server_shutdown();

#ifdef __cplusplus
    }
#endif
