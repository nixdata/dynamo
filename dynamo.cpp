#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "dynamo.h"
#include "dynamo-system.h"


#define FIXED_TIME_LIMIT 16666666
#define FRAME_TIME_CLAMP 500000000


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
    struct dmo_time total_time = {0};
    struct dmo_time current_time = {0};
    long accum_time = 0;
    long frame_time = 0;
    long fixed_time = FIXED_TIME_LIMIT;

    // struct dmo_state previous_state;
    // struct dmo_state current_state;

    while(running) {

        dmo_time new_time = dmo_sys_time();

        frame_time = new_time.nanoseconds - current_time.nanoseconds;
        if(frame_time > FRAME_TIME_CLAMP) {
            frame_time = FRAME_TIME_CLAMP;
        }
        
        total_time.seconds = new_time.seconds - base_time.seconds;
        total_time.nanoseconds = new_time.nanoseconds - base_time.nanoseconds;
        (void)total_time;

        current_time.seconds = new_time.seconds;
        current_time.nanoseconds = new_time.nanoseconds;

        accum_time += frame_time;

        while(accum_time >= fixed_time) {

            //previous_state = current_state;

            // simulate current_state with fixed_time;
            // dmo_sim_step(current_state, total_time, fixed_time);
            
            accum_time -= fixed_time;
        }

        const double alpha_time = accum_time / fixed_time;
        (void)alpha_time;
        
        // dmo_state alpha_state = current_state * alpha + previous_state * (1.0 - alpha_state);
        // dmo_gfx_render(current_state);
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
