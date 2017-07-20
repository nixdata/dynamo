#pragma once


#ifdef __cplusplus
    extern "C" {
#endif


struct dmo_state_t {
    int int_state;
    double double_state;
    void *void_state;
};


struct dmo_api_t {
    struct dmo_state_t *(*init)();
    void (*finalize)(struct dmo_state_t *state);
    void (*reload)(struct dmo_state_t *state);
    void (*unload)(struct dmo_state_t *state);
    bool (*step)(struct dmo_state_t *state);
};


extern const struct dmo_api_t DYNAMO_API;


#ifdef __cplusplus
    }
#endif

