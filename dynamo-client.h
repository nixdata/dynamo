#pragma once

#ifdef __cplusplus
    extern "C" {
#endif


int dmo_client_startup();
int dmo_client_update(struct dmo_time time);
void dmo_client_shutdown();


#ifdef __cplusplus
    }
#endif
