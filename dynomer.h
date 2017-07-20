#pragma once
#include "system.h"


#ifdef __cplusplus
    extern "C" {
#endif


struct dyno_state_t {
    int int_state;
    double double_state;
    void *void_state;
};


struct dyno_api_t {
    struct dyno_state_t *(*init)();
    void (*finalize)(struct dyno_state_t *state);
    void (*reload)(struct dyno_state_t *state);
    void (*unload)(struct dyno_state_t *state);
    bool (*step)(struct dyno_state_t *state);
};


extern const struct dyno_api_t DYNOMER_API;


#ifdef __cplusplus
    }
#endif

