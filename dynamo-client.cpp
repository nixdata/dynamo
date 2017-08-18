#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "dynamo.h"
#include "dynamo-client.h"
#include "lib/netcode/netcode.h"

#define TEST_CONNECT_TOKEN_EXPIRY 30


static u8 packet_data[NETCODE_MAX_PACKET_SIZE];


// TODO: Not happy with the way client_create and client_startup logic is separated.
struct dmo_client dmo_client_create(u64 protocol, const char *server_address)
{
    struct dmo_client client = {0};
    client.protocol = protocol;
    client.server_address = server_address;
    // TODO: This should eventually come from an external service/process
    client.private_key = (struct dmo_client_private_key){
                          0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea, 
                          0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4, 
                          0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
                          0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1 };
    
    dmo_time time = dmo_sys_time();

    if(netcode_init() != NETCODE_OK)
    {
        printf("ERROR: Failed to initialize netcode.io\n");
        // TODO: Better way to handle this?
        exit(1);
    }

    netcode_log_level(NETCODE_LOG_LEVEL_INFO);
    
    client.nio_client = netcode_client_create("0.0.0.0", dmo_sys_time_to_double(time));
    if(!client.nio_client) {
        printf("ERROR: Failed to create netcode.io client.\n");
        // TODO: Better way to handle this?
        exit(1);
    }
    
    u64 client_id = 0;
    netcode_random_bytes((u8 *)&client_id, 8);

    if ( netcode_generate_connect_token( 1, &client.server_address, TEST_CONNECT_TOKEN_EXPIRY, client_id, client.protocol, 0, client.private_key.bytes, client.connect_token.bytes ) != NETCODE_OK ) {
        printf("ERROR: Failed to generate connect token\n");
        // TODO: Better way to handle this?
        exit(1);
    }

    return client;
};


int dmo_client_startup(struct dmo_client client)
{
    netcode_client_connect(client.nio_client, client.connect_token.bytes);
    
    printf("packet data: ");
    for(u32 i = 0; i < NETCODE_MAX_PACKET_SIZE; ++i) {
        packet_data[i] = (u8)42;//(u8)i;
    }

    return DYNAMO_OK;
}


int dmo_client_update(struct dmo_client client, dmo_time time)
{
    netcode_client_update(client.nio_client, dmo_sys_time_to_double(time));

    if(netcode_client_state(client.nio_client) == NETCODE_CLIENT_STATE_CONNECTED) {
        netcode_client_send_packet(client.nio_client, packet_data, NETCODE_MAX_PACKET_SIZE);
    }

    while(1) {
        int packet_bytes;
        u64 packet_sequence;
        void * packet = netcode_client_receive_packet( client.nio_client, &packet_bytes, &packet_sequence );
        if (!packet) {
            break;
        }

        // TODO: Only assert in debug mode.
        assert( packet_bytes == NETCODE_MAX_PACKET_SIZE );
        assert( memcmp( packet, packet_data, NETCODE_MAX_PACKET_SIZE ) == 0 );            

        netcode_client_free_packet( client.nio_client, (u8 *)packet );
    }

    return DYNAMO_OK;
}




void dmo_client_shutdown(struct dmo_client client)
{
    printf("shutting down dynamo client...\n");
    netcode_client_destroy(client.nio_client);
    netcode_term();
    printf("dynamo client off!\n");

    exit(0);
}
