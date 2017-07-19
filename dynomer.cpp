#include <stdlib.h>
#include <stdio.h>
#include "dynomer.h"


struct dynomer_state {
    int int_state;
    double double_state;
    void *void_state;
};


static struct dynomer_state * dynomer_init()
{
    struct dynomer_state *state = (struct dynomer_state *)malloc(sizeof(dynomer_state)); // Swap with your allocator.

    state->int_state = 0;
    state->double_state = 0.0f;
    state->void_state = NULL;

    return state;
}


static bool dynomer_step(struct dynomer_state *state) 
{
    //printf("\e[1;1H\e[2J"); // clear terminal screen
    using namespace dynomer;
    using namespace platform;
    struct dyno_time_t t = get_high_res_time();
    //printf("\033c");
    printf("%ld:%ld\n", t.seconds, t.nanoseconds);
    return true;
}


static void dynomer_unload(struct dynomer_state *state)
{
}


static void dynomer_reload(struct dynomer_state *state)
{
}


static void dynomer_finalize(struct dynomer_state *state)
{
    free(state); // Swap with your free function.
}


const struct dynomer_api DYNOMER_API = {
    dynomer_init,
    dynomer_finalize,
    dynomer_reload,
    dynomer_unload,
    dynomer_step
};
