#pragma once
#include "dynamo.h"

#ifdef __cplusplus
    extern "C" {
#endif


struct dmo_server {
    u64 protocol;
    const char *address;
};


struct dmo_server dmo_server_create(u64 protocol, const char *address);
int dmo_server_startup(struct dmo_server server);
int dmo_server_update(struct dmo_server server, struct dmo_time time);
void dmo_server_shutdown(struct dmo_server server);

#ifdef __cplusplus
    }
#endif
