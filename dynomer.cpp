#include <stdlib.h>
#include <stdio.h>
#include "dynomer.h"


static struct dyno_state_t * dyno_init()
{
    // TODO: Swap with your allocator.
    struct dyno_state_t *state = (struct dyno_state_t *)malloc(sizeof(struct dyno_state_t));

    state->int_state = 0;
    state->double_state = 0.0f;
    state->void_state = NULL;

    return state;
}


static bool dyno_step(struct dyno_state_t *state) 
{
    struct dyno_time_t t = dyno_sys_hr_time();
    printf("\033c");
    printf("%ld:%ld\n", t.seconds, t.nanoseconds);
    return true;
}


static void dyno_unload(struct dyno_state_t *state)
{
}


static void dyno_reload(struct dyno_state_t *state)
{
}


static void dyno_finalize(struct dyno_state_t *state)
{
    free(state); // Swap with your free function.
}


const struct dyno_api_t DYNOMER_API = {
    dyno_init,
    dyno_finalize,
    dyno_reload,
    dyno_unload,
    dyno_step
};
