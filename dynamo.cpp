#include <stdlib.h>
#include <stdio.h>
#include "dynamo.h"
#include "dynamo-system.h"


static bool running;
static struct dmo_time dmo_time;


void interrupt_handler(int signal) 
{
    (void) signal;
    running = false;
}


int dmo_startup() 
{
    running = false;
    dmo_time = {0};
    printf("Starting dynamo...\n");
    dmo_sys_init();

    printf("Dynamo on!\n");

    return DMO_OK; 
}


void dmo_run() 
{
    running = true;

    while(running) {

    }

    dmo_shutdown();
}


int dmo_shutdown()
{
    printf("Shutting down dynamo...\n");
    printf("Dynamo off!\n");

    return DMO_OK;
}
