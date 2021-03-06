#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include "dynamo.h"
#include "dynamo-server.h"


static struct dmo_server server;
bool running;


void interrupt_handler(int signal) 
{
    (void) signal;
    dmo_server_shutdown(server);
}


void run() 
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
            dmo_server_update(server, current_time);
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


int main(int argc, char *argv[])
{
    if(argc != 3) {
        printf("usage: server <protocol id> <address:port>\n");
        return EXIT_FAILURE;
    }

    u64 protocol = atoll(argv[1]);
    const char *address = argv[2];
    server = dmo_server_create(protocol, address);
    signal(SIGINT, interrupt_handler);
    dmo_server_startup(server);
    run();

    return EXIT_SUCCESS;
}
