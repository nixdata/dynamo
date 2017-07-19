#pragma once

#include "platform.h"

struct dynomer_state_t {
    int int_state;
    double double_state;
    void *void_state;
};


struct dynomer_api_t {
    dynomer_state_t *(*init)();
    void (*finalize)(dynomer_state_t *state);
    void (*reload)(dynomer_state_t *state);
    void (*unload)(dynomer_state_t *state);
    bool (*step)(dynomer_state_t *state);
};


extern const dynomer_api_t DYNOMER_API;
