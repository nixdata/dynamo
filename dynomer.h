#pragma once

#include "platform.h"


struct dynomer_state;

struct dynomer_api {
    struct dynomer_state *(*init)();
    void (*finalize)(struct dynomer_state *state);
    void (*reload)(struct dynomer_state *state);
    void (*unload)(struct dynomer_state *state);
    bool (*step)(struct dynomer_state *state);
};

extern const struct dynomer_api DYNOMER_API;
