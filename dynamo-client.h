#pragma once

#include "./lib/netcode/netcode.h"

#ifdef __cplusplus
    extern "C" {
#endif


struct dmo_client_private_key {
    u8 bytes[NETCODE_KEY_BYTES];
};


// TODO: This will eventually come from web service.
struct dmo_connect_token {
    u8 bytes[NETCODE_CONNECT_TOKEN_BYTES];
};


struct dmo_client {
    // TODO: Make sure this struct is packed properly.
    u64 protocol;
    const char *server_address;
    struct dmo_client_private_key private_key;
    struct dmo_connect_token connect_token;
    netcode_client_t *nio_client;
};


struct dmo_client dmo_client_create(u64 protocol, const char *server_address);
int dmo_client_startup(struct dmo_client client);
int dmo_client_update(struct dmo_client client, struct dmo_time time);
void dmo_client_shutdown(struct dmo_client client);


#ifdef __cplusplus
    }
#endif
