#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "dynamo.h"
#include "dynamo-system.h"


static bool running;
static struct dmo_time dmo_time;


// TODO: Move this into dmo_sys
void interrupt_handler(int signal) 
{
    (void) signal;
    dmo_shutdown();
}


int dmo_startup() 
{
    running = false;
    dmo_time = {0};

    printf("Starting dynamo...\n");

    signal(SIGINT, interrupt_handler);
    dmo_sys_init();

    printf("Dynamo on!\n");

    return DMO_OK; 
}


void dmo_run() 
{
    running = true;

    while(running) {

    }
}


int dmo_shutdown()
{
    running = false;

    printf("Shutting down dynamo...\n");
    printf("Dynamo off!\n");

    exit(0);

    return DMO_OK;
}
