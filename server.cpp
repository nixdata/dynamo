#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include "dynamo.h"
#include "./lib/netcode/netcode.h"


#define TEST_PROTOCOL_ID 0x1122334455667788
#define SERVER_ADDRESS "127.0.0.1:40000"


static bool running = false;
static struct netcode_server_t *server;
static u8 packet_data[NETCODE_MAX_PACKET_SIZE];
static uint8_t private_key[NETCODE_KEY_BYTES] = { 0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea, 
                                                  0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4, 
                                                  0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
                                                  0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1 };

void interrupt_handler(int signal); 


int dmo_server_startup() 
{
    if(running) return DYNAMO_OK;

    printf("Starting Dynamo server...\n");

    signal(SIGINT, interrupt_handler);

    dmo_time time = dmo_sys_time();

    if(netcode_init() != NETCODE_OK) {
        printf("ERROR: Failed to initialize netcode.io\n");
        return DYNAMO_ERROR;
    } else {
        printf("Dynamo server started!\n");
    }

    netcode_log_level(NETCODE_LOG_LEVEL_INFO);

    const char *address = SERVER_ADDRESS;
    
    server = netcode_server_create(address,TEST_PROTOCOL_ID, private_key, dmo_sys_time_to_double(time));
    if(!server) {
        printf("ERROR: Failed to create netcode.io server.\n");
        return DYNAMO_ERROR;
    }
    netcode_server_start(server, NETCODE_MAX_CLIENTS);

    for(u32 i = 0; i < NETCODE_MAX_PACKET_SIZE; ++i) {
        packet_data[i] = (u8)i;
    }

    printf("Dynamo server on!\n");

    return DYNAMO_OK; 
}


int dmo_server_update(dmo_time time)
{
    netcode_server_update(server, dmo_sys_time_to_double(time));

    if(netcode_server_client_connected(server, 0)) {
        //netcode_server_send_packet(server, 0, packet_data, NETCODE_MAX_PACKET_SIZE);
    }

    for(int client_index = 0; client_index < NETCODE_MAX_CLIENTS; ++client_index) {
        while(1) {
            i32 packet_bytes; 
            u64 packet_sequence;
            u8 *packet = netcode_server_receive_packet(server, client_index, &packet_bytes, &packet_sequence);
            if(!packet) {
                break;
            }
            
            printf("first byte: %u\n", packet[0]);

            for(int i = 0; i < NETCODE_MAX_PACKET_SIZE; ++i) {
                packet_data[i] = (u8)i;
            }
            // TODO: Only assert in debug mode.
            //assert(packet_bytes == NETCODE_MAX_PACKET_SIZE);
            //assert(memcmp(packet, packet_data, NETCODE_MAX_PACKET_SIZE) == 0);

            netcode_server_free_packet(server, packet);
        }
    }
    return DYNAMO_OK;
}


void dmo_server_run() 
{
    running = true;
    struct dmo_time new_time {0};
    struct dmo_time current_time = dmo_sys_time();
    long net_time = 0;
    long sim_time = 0;
    long gfx_time = 0;
    long frame_time = 0;
    long sleep_time = 0;
    double alpha_time = 0.0;
    
    // struct dmo_state previous_state;
    // struct dmo_state current_state;

    while(running) {
        
        new_time = dmo_sys_time();
        
        struct dmo_time delta_time = {0};
        delta_time.seconds = new_time.seconds - current_time.seconds;
        delta_time.nanoseconds = new_time.nanoseconds - current_time.nanoseconds;
        frame_time = (delta_time.seconds * NANOS_IN_SECOND) + delta_time.nanoseconds;
        if(frame_time > FRAME_TIME_MAX) {
            frame_time = FRAME_TIME_MAX;
        }
        
        current_time.seconds = new_time.seconds;
        current_time.nanoseconds = new_time.nanoseconds;

        net_time += frame_time;
        if(net_time >= NET_UPDATE_RATE) {
            dmo_server_update(current_time);
            net_time = 0;
        }

        sim_time += frame_time;
        while(sim_time >= SIM_UPDATE_RATE) {
            

            //previous_state = current_state;

            // simulate current_state with SIM_UPDATE_RATE;
            // dmo_sim_update(current_state, total_time, SIM_UPDATE_RATE);
            
            sim_time -= SIM_UPDATE_RATE;
        }
        
        // TODO: Ripped from the gaffer. Personally unverified.
        alpha_time = sim_time / SIM_UPDATE_RATE;
        (void)alpha_time;
        
        // TODO: Remove this once the client is up and running.
        gfx_time += frame_time;
        if(gfx_time >= GFX_UPDATE_RATE) {
            // dmo_state alpha_state = current_state * alpha + previous_state * (1.0 - alpha_state);
            // dmo_gfx_update(current_state);
        }
            
        new_time = dmo_sys_time();
        sleep_time = GFX_UPDATE_RATE - (new_time.nanoseconds - current_time.nanoseconds);
        if(sleep_time >= 0) {
            dmo_sys_sleep({0, sleep_time});
        } else {
            // TODO: Test and handle this condition.
            printf("halp! I'm running behind!\n");
        }
    }
}




void dmo_server_shutdown()
{
    running = false;

    printf("\nShutting down Dynamo server...\n");
    netcode_server_destroy(server);
    netcode_term();
    printf("Dynamo server off!\n");

    exit(0);
}


void interrupt_handler(int signal) 
{
    (void) signal;
    dmo_server_shutdown();
}


int main(int argc, char *argv[])
{
    dmo_server_startup();
    dmo_server_run();

    return EXIT_SUCCESS;
}
