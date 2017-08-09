#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "dynamo.h"
#include "dynamo-network.h"
#include "./lib/netcode.io/c/netcode.h"


#define TEST_PROTOCOL_ID 0x1122334455667788


static bool dmo_net_started = false;
static struct netcode_server_t *server;
static u8 packet_data[NETCODE_MAX_PACKET_SIZE];
static uint8_t private_key[NETCODE_KEY_BYTES] = { 0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea, 
                                                  0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4, 
                                                  0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
                                                  0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1 };


int dmo_net_startup(struct dmo_time time)
{
    if(dmo_net_started) return DMO_OK;

    if(netcode_init() != NETCODE_OK) {
        printf("ERROR: Failed to initialize netcode.io\n");
        return DMO_ERROR;
    } else {
        printf("dmo_net_startup: OK! %ld:%ld\n", time.seconds, time.nanoseconds);
    }

    netcode_log_level(NETCODE_LOG_LEVEL_INFO);

    const char *address = "127.0.0.1:40000";
    
    f64 t = dmo_sys_time_to_double(time);
    server = netcode_server_create(address,TEST_PROTOCOL_ID, private_key, t);
    if(!server) {
        printf("ERROR: Failed to create netcode.io server.\n");
        return DMO_ERROR;
    }
    netcode_server_start(server, NETCODE_MAX_CLIENTS);

    for(int i = 0; i < NETCODE_MAX_PACKET_SIZE; ++i) {
        packet_data[i] = (u8)i;
    }

    dmo_net_started = true;

    return DMO_OK;
}


int dmo_net_update(dmo_time time)
{
    netcode_server_update(server, dmo_sys_time_to_double(time));

    if(netcode_server_client_connected(server, 0)) {
        netcode_server_send_packet(server, 0, packet_data, NETCODE_MAX_PACKET_SIZE);
    }

    for(int client_index = 0; client_index < NETCODE_MAX_CLIENTS; ++client_index) {
        while(1) {
            int packet_bytes; 
            u64 packet_sequence;
            void *packet = netcode_server_receive_packet(server, client_index, &packet_bytes, &packet_sequence);
            if(!packet) {
                break;
            }
            
            // TODO: Only assert in debug mode.
            assert(packet_bytes == NETCODE_MAX_PACKET_SIZE);
            assert(memcmp(packet, packet_data, NETCODE_MAX_PACKET_SIZE) == 0);

            netcode_server_free_packet(server, packet);
        }
    }
    return DMO_OK;
}


int dmo_net_shutdown()
{
    printf("shutting down dmo_network\n");
    netcode_server_destroy(server);
    netcode_term();
    return DMO_OK;
}
