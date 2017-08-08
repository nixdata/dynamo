#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

#define DMO_OK 0
#define DMO_ERROR 1


int dmo_startup();
void dmo_run();
int dmo_shutdown();

#ifdef __cplusplus
    }
#endif
