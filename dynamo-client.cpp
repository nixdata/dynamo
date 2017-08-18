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


static struct netcode_client_t *client;
static u8 packet_data[NETCODE_MAX_PACKET_SIZE];
static uint8_t private_key[NETCODE_KEY_BYTES] = { 0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea, 
                                                  0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4, 
                                                  0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
                                                  0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1 };


int dmo_client_startup()
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
    
    client = netcode_client_create("0.0.0.0", dmo_sys_time_to_double(time));
    if(!client) {
        printf("ERROR: Failed to create netcode.io client.\n");
        return DYNAMO_ERROR;
    }

    const char *server_address = SERVER_ADDRESS;

    u64 client_id = 0;
    netcode_random_bytes((u8 *)&client_id, 8);

    // TODO: This will eventually come from web service.
    u8 connect_token[NETCODE_CONNECT_TOKEN_BYTES];

    if ( netcode_generate_connect_token( 1, &server_address, TEST_CONNECT_TOKEN_EXPIRY, client_id, TEST_PROTOCOL_ID, 0, private_key, connect_token ) != NETCODE_OK ) {
        printf("ERROR: Failed to generate connect token\n");
        return DYNAMO_ERROR;
    }

    netcode_client_connect(client, connect_token);
    
    printf("packet data: ");
    for(u32 i = 0; i < NETCODE_MAX_PACKET_SIZE; ++i) {
        packet_data[i] = (u8)42;//(u8)i;
    }

    printf("Dynamo client on!\n");

    return DYNAMO_OK;
}


int dmo_client_update(dmo_time time)
{
    netcode_client_update(client, dmo_sys_time_to_double(time));

    if(netcode_client_state(client) == NETCODE_CLIENT_STATE_CONNECTED) {
        netcode_client_send_packet(client, packet_data, NETCODE_MAX_PACKET_SIZE);
    }

    while(1) {
        int packet_bytes;
        u64 packet_sequence;
        void * packet = netcode_client_receive_packet( client, &packet_bytes, &packet_sequence );
        if (!packet) {
            break;
        }

        // TODO: Only assert in debug mode.
        assert( packet_bytes == NETCODE_MAX_PACKET_SIZE );
        assert( memcmp( packet, packet_data, NETCODE_MAX_PACKET_SIZE ) == 0 );            

        netcode_client_free_packet( client, (u8 *)packet );
    }

    return DYNAMO_OK;
}




void dmo_client_shutdown()
{
    // TODO: Flip running to false

    printf("Shutting down Dynamo client...\n");
    netcode_client_destroy(client);
    netcode_term();
    printf("Dynamo client off!\n");

    exit(0);
}
