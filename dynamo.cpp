#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "dynamo.h"
#include "dynamo-system.h"


#define NANOS_IN_SECOND 1000000000
#define SIM_UPDATE_RATE 16666666 // 60Hz
#define NET_UPDATE_RATE 16666666 // 60Hz
#define GFX_UPDATE_RATE 8333333  // 120Hz
#define FRAME_TIME_MAX 500000000


static bool running = false;


// TODO: Move this into dmo_sys
void interrupt_handler(int signal) 
{
    (void) signal;
    dmo_shutdown();
}


int dmo_startup() 
{
    if(running) return DMO_OK;

    printf("Starting Dynamo...\n");

    signal(SIGINT, interrupt_handler);
    dmo_sys_init();

    printf("Dynamo on!\n");

    return DMO_OK; 
}


void dmo_run() 
{
    running = true;
    struct dmo_time base_time = dmo_sys_time();
    struct dmo_time new_time {0};
    struct dmo_time total_time = {0};
    struct dmo_time current_time = {0};
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
        
        total_time.seconds = new_time.seconds - base_time.seconds;
        total_time.nanoseconds = new_time.nanoseconds - base_time.nanoseconds;
        (void)total_time;

        current_time.seconds = new_time.seconds;
        current_time.nanoseconds = new_time.nanoseconds;

        net_time += frame_time;
        if(net_time >= NET_UPDATE_RATE) {
            // dmo_net_step();
            net_time = 0;
        }

        sim_time += frame_time;
        while(sim_time >= SIM_UPDATE_RATE) {

            //previous_state = current_state;

            // simulate current_state with SIM_UPDATE_RATE;
            // dmo_sim_step(current_state, total_time, SIM_UPDATE_RATE);
            
            sim_time -= SIM_UPDATE_RATE;
        }

        alpha_time = sim_time / SIM_UPDATE_RATE;
        (void)alpha_time;
        
        // TODO: Remove this once the client is up and running.
        gfx_time += frame_time;
        if(gfx_time >= GFX_UPDATE_RATE) {
            // dmo_state alpha_state = current_state * alpha + previous_state * (1.0 - alpha_state);
            // dmo_gfx_step(current_state);
        }
        
        new_time = dmo_sys_time();
        sleep_time = GFX_UPDATE_RATE - (new_time.nanoseconds - current_time.nanoseconds);
        if(sleep_time < GFX_UPDATE_RATE) {
            dmo_sys_sleep({0, sleep_time});
        }
    }
}


int dmo_shutdown()
{
    running = false;

    printf("\nShutting down Dynamo...\n");
    printf("Dynamo off!\n");

    exit(0);

    return DMO_OK;
}
