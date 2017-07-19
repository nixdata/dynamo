#include <stdlib.h>
#include <stdio.h>
#include "dynomer.h"


using namespace dynomer;


static dynomer_state_t * dynomer_init()
{
    dynomer_state_t *state = (dynomer_state_t *)malloc(sizeof(dynomer_state_t)); // Swap with your allocator.

    state->int_state = 0;
    state->double_state = 0.0f;
    state->void_state = NULL;

    return state;
}


static bool dynomer_step(dynomer_state_t *state) 
{
    platform::dyno_time_t t = platform::get_high_res_time();
    printf("\033c");
    printf("%ld:%ld\n", t.seconds, t.nanoseconds);
    return true;
}


static void dynomer_unload(dynomer_state_t *state)
{
}


static void dynomer_reload(dynomer_state_t *state)
{
}


static void dynomer_finalize(dynomer_state_t *state)
{
    free(state); // Swap with your free function.
}


const dynomer_api_t DYNOMER_API = {
    dynomer_init,
    dynomer_finalize,
    dynomer_reload,
    dynomer_unload,
    dynomer_step
};
