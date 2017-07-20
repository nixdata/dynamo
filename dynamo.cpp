#include <stdlib.h>
#include <stdio.h>
#include "dynamo.h"
#include "dynamo-system.h"


static struct dmo_state_t * dmo_init()
{
    // TODO: Swap with your allocator.
    struct dmo_state_t *state = (struct dmo_state_t *)malloc(sizeof(struct dmo_state_t));

    state->int_state = 0;
    state->double_state = 0.0f;
    state->void_state = NULL;

    return state;
}


static bool dmo_step(struct dmo_state_t *state) 
{
    struct dmo_time_t t = dmo_sys_time();
    printf("\033c");
    printf("%ld:%ld\n", t.seconds, t.nanoseconds);
    return true;
}


static void dmo_unload(struct dmo_state_t *state)
{
}


static void dmo_reload(struct dmo_state_t *state)
{
}


static void dmo_finalize(struct dmo_state_t *state)
{
    free(state); // Swap with your free function.
}


const struct dmo_api_t DYNAMO_API = {
    dmo_init,
    dmo_finalize,
    dmo_reload,
    dmo_unload,
    dmo_step
};
