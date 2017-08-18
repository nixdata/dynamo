#pragma once
#include "dynamo.h"
#include "./lib/netcode/netcode.h"

#ifdef __cplusplus
    extern "C" {
#endif


struct dmo_private_key {
    u8 bytes[NETCODE_KEY_BYTES];
};

struct dmo_server {
    // TODO: Make sure this struct is packed properly.
    u64 protocol;
    const char *address;
    struct dmo_private_key private_key;
    netcode_server_t *nio_server;
};


struct dmo_server dmo_server_create(u64 protocol, const char *address);
int dmo_server_startup(struct dmo_server server);
int dmo_server_update(struct dmo_server server, struct dmo_time time);
void dmo_server_shutdown(struct dmo_server server);

#ifdef __cplusplus
    }
#endif
