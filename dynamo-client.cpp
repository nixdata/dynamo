#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "dynamo.h"
#include "dynamo-client.h"
#include "lib/netcode/netcode.h"

#define TEST_PROTOCOL_ID 0x1122334455667788
#define TEST_CONNECT_TOKEN_EXPIRY 30
#define SERVER_ADDRESS "127.0.0.1:40000"


static struct netcode_client_t *nio_client;
static u8 packet_data[NETCODE_MAX_PACKET_SIZE];
static uint8_t private_key[NETCODE_KEY_BYTES] = { 0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea, 
                                                  0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4, 
                                                  0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
                                                  0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1 };


struct dmo_client dmo_client_create(u64 protocol, const char *server_address)
{
    struct dmo_client client = {0};
    client.protocol = protocol;
    client.server_address = server_address;
    return client;
};


int dmo_client_startup(struct dmo_client client)
{
    // TODO: Check to make sure client isn't running already.

    printf("Starting Dynamo client...\n");


    dmo_time time = dmo_sys_time();

    if(netcode_init() != NETCODE_OK)
    {
        printf("ERROR: Failed to initialize netcode.io\n");
        return DYNAMO_ERROR;
    } else {
        printf("Dynamo client started!\n");
    }

    netcode_log_level(NETCODE_LOG_LEVEL_INFO);
    
    nio_client = netcode_client_create("0.0.0.0", dmo_sys_time_to_double(time));
    if(!nio_client) {
        printf("ERROR: Failed to create netcode.io client.\n");
        return DYNAMO_ERROR;
    }

    u64 client_id = 0;
    netcode_random_bytes((u8 *)&client_id, 8);

    // TODO: This will eventually come from web service.
    u8 connect_token[NETCODE_CONNECT_TOKEN_BYTES];

    if ( netcode_generate_connect_token( 1, &client.server_address, TEST_CONNECT_TOKEN_EXPIRY, client_id, client.protocol, 0, private_key, connect_token ) != NETCODE_OK ) {
        printf("ERROR: Failed to generate connect token\n");
        return DYNAMO_ERROR;
    }

    netcode_client_connect(nio_client, connect_token);
    
    printf("packet data: ");
    for(u32 i = 0; i < NETCODE_MAX_PACKET_SIZE; ++i) {
        packet_data[i] = (u8)42;//(u8)i;
    }

    printf("Dynamo client on!\n");

    return DYNAMO_OK;
}


int dmo_client_update(struct dmo_client client, dmo_time time)
{
    netcode_client_update(nio_client, dmo_sys_time_to_double(time));

    if(netcode_client_state(nio_client) == NETCODE_CLIENT_STATE_CONNECTED) {
        netcode_client_send_packet(nio_client, packet_data, NETCODE_MAX_PACKET_SIZE);
    }

    while(1) {
        int packet_bytes;
        u64 packet_sequence;
        void * packet = netcode_client_receive_packet( nio_client, &packet_bytes, &packet_sequence );
        if (!packet) {
            break;
        }

        // TODO: Only assert in debug mode.
        assert( packet_bytes == NETCODE_MAX_PACKET_SIZE );
        assert( memcmp( packet, packet_data, NETCODE_MAX_PACKET_SIZE ) == 0 );            

        netcode_client_free_packet( nio_client, (u8 *)packet );
    }

    return DYNAMO_OK;
}




void dmo_client_shutdown(struct dmo_client client)
{
    // TODO: Flip running to false

    printf("Shutting down Dynamo client...\n");
    netcode_client_destroy(nio_client);
    netcode_term();
    printf("Dynamo client off!\n");

    exit(0);
}
