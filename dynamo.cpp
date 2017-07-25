#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "dynamo.h"
#include "dynamo-system.h"


#define NANOS_IN_SECOND 1000000000
#define FIXED_TIME_LIMIT 16666666 // 60Hz
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
    long accum_time = 0;
    long frame_time = 0;
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

        accum_time += frame_time;

        if(accum_time >= FIXED_TIME_LIMIT) {
            
            // dmo_net_step();

            while(accum_time >= FIXED_TIME_LIMIT) {

                //previous_state = current_state;

                // simulate current_state with FIXED_TIME_LIMIT;
                // dmo_sim_step(current_state, total_time, FIXED_TIME_LIMIT);
                
                accum_time -= FIXED_TIME_LIMIT;
            }
        }

        alpha_time = accum_time / FIXED_TIME_LIMIT;
        (void)alpha_time;
        
        // dmo_state alpha_state = current_state * alpha + previous_state * (1.0 - alpha_state);
        // dmo_gfx_step(current_state);
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
