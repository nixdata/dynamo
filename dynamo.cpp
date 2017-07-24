#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "dynamo.h"
#include "dynamo-system.h"


static bool running;
static struct dmo_time current_time;
static struct dmo_time total_time;
static struct dmo_time delta_time;
static struct dmo_time accum_time;


// TODO: Move this into dmo_sys
void interrupt_handler(int signal) 
{
    (void) signal;
    dmo_shutdown();
}


int dmo_startup() 
{
    running = false;

    printf("Starting Dynamo...\n");

    signal(SIGINT, interrupt_handler);
    dmo_sys_init();

    printf("Dynamo on!\n");

    return DMO_OK; 
}


void dmo_run() 
{
    running = true;
    current_time = {0};
    total_time = {0};
    delta_time = {0};
    accum_time = {0};

    // struct dmo_state previous_state;
    // struct dmo_state current_state;

    while(running) {
        dmo_time new_time = dmo_sys_time();
        dmo_time frame_time = {0};

        frame_time.seconds = new_time.seconds - current_time.seconds;
        frame_time.nanoseconds = new_time.nanoseconds - current_time.nanoseconds;
        // TODO: Figure out frame_time clamp below.
        //if(frame_time > some_clamped_value) { frame_time = some_clamped_value; }

        current_time.seconds = new_time.seconds;
        current_time.nanoseconds = new_time.nanoseconds;

        accum_time.seconds += frame_time.seconds;
        accum_time.nanoseconds += frame_time.nanoseconds;

        while((accum_time.seconds >= delta_time.seconds) && (accum_time.nanoseconds >= delta_time.nanoseconds)) {

            //previous_state = current_state;

            // simulate current_state with delta_time;
            // dmo_sim_step(current_state, total_time, delta_time);

            total_time.seconds += delta_time.seconds;
            total_time.nanoseconds += delta_time.nanoseconds;
            accum_time.seconds -= delta_time.seconds;
            accum_time.nanoseconds -= delta_time.nanoseconds;
        }

        const double alpha_seconds = accum_time.seconds / delta_time.seconds;
        const double alpha_nanoseconds = accum_time.nanoseconds / delta_time.nanoseconds;
        (void)alpha_seconds; 
        (void)alpha_nanoseconds;
        
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
