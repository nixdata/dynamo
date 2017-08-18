#include <stdlib.h>
#include <stdio.h>
#include "dynamo.h"
#include "dynamo-server.h"
#include "./lib/netcode/netcode.h"


static u8 packet_data[NETCODE_MAX_PACKET_SIZE];



struct dmo_server dmo_server_create(u64 protocol, const char *address)
{
    struct dmo_server server = {0};
    server.protocol = protocol;
    server.address = address;
    // TODO: This should eventually come from an external service/process
    server.private_key = (struct dmo_private_key){
                          0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea, 
                          0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4, 
                          0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
                          0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1 };
    
    printf("starting server...\n");

    dmo_time time = dmo_sys_time();

    if(netcode_init() != NETCODE_OK) {
        printf("ERROR: Failed to initialize netcode.io\n");
        // TODO: Better way to handle this?
        exit(1);
    }

    netcode_log_level(NETCODE_LOG_LEVEL_INFO);

    server.nio_server = netcode_server_create(server.address, server.protocol, server.private_key.bytes, dmo_sys_time_to_double(time));
    if(!server.nio_server) {
        printf("ERROR: Failed to create netcode.io server.\n");
        // TODO: Better way to handle this?
        exit(1);
    }

    return server;
};


int dmo_server_startup(struct dmo_server server) 
{
    netcode_server_start(server.nio_server, NETCODE_MAX_CLIENTS);

    for(u32 i = 0; i < NETCODE_MAX_PACKET_SIZE; ++i) {
        packet_data[i] = (u8)i;
    }

    return DYNAMO_OK; 
}


int dmo_server_update(struct dmo_server server, dmo_time time)
{
    // Read all available packets and pop payload packets on queue. Also, send keepalive
    // packets to qualifying clients.
    netcode_server_update(server.nio_server, dmo_sys_time_to_double(time));
    
    // TODO: Sending a specific client update should be up to the simulation layer.
    if(netcode_server_client_connected(server.nio_server, 0)) { // Just send to client 0 for now.
        // Send payload packet to client.
        netcode_server_send_packet(server.nio_server, 0, packet_data, NETCODE_MAX_PACKET_SIZE);
    }

    for(int client_index = 0; client_index < NETCODE_MAX_CLIENTS; ++client_index) {
        while(1) {
            i32 packet_bytes; 
            u64 packet_sequence;
            // Process payload packet that has been popped on queue.
            u8 *packet = netcode_server_receive_packet(server.nio_server, client_index, &packet_bytes, &packet_sequence);
            if(!packet) {
                break;
            }
            
            printf("%u\n", packet[0]);

            for(int i = 0; i < NETCODE_MAX_PACKET_SIZE; ++i) {
                packet_data[i] = (u8)i;
            }
            // TODO: Only assert in debug mode.
            //assert(packet_bytes == NETCODE_MAX_PACKET_SIZE);
            //assert(memcmp(packet, packet_data, NETCODE_MAX_PACKET_SIZE) == 0);

            netcode_server_free_packet(server.nio_server, packet);
        }
    }
    return DYNAMO_OK;
}


void dmo_server_shutdown(struct dmo_server server)
{
    printf("\nshutting down server...\n");
    netcode_server_destroy(server.nio_server);
    netcode_term();
    printf("server off!\n");

    exit(0);
}
