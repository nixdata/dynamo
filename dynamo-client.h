#pragma once

#include "./lib/netcode/netcdoe.h"

#ifdef __cplusplus
    extern "C" {
#endif

struct dmo_client {
    u64 protocol;
    const char *server_address;
};


struct dmo_client dmo_client_create(u64 protocol, const char *server_address);
int dmo_client_startup(struct dmo_client client);
int dmo_client_update(struct dmo_client client, struct dmo_time time);
void dmo_client_shutdown(struct dmo_client client);


#ifdef __cplusplus
    }
#endif
