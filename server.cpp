#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include "dynomer.h"


const char *DYNOMER_LIBRARY = "./lib-dynomer.so";


int quit = 0;


struct dynomer_t {
    void *handle;
    ino_t id;
    struct dynomer_api api;
    struct dynomer_state *state;
};


static void dynomer_load(struct dynomer_t *dynomer)
{
    struct stat attr;
    if((stat(DYNOMER_LIBRARY, &attr) == 0) && (dynomer->id != attr.st_ino)) {
        if(dynomer->handle) {
            dynomer->api.unload(dynomer->state);
            dlclose(dynomer->handle);
        }

        void *handle = dlopen(DYNOMER_LIBRARY, RTLD_NOW);
        if(handle) {
            dynomer->handle = handle;
            dynomer->id = attr.st_ino;
            const struct dynomer_api *api = (const struct dynomer_api *)dlsym(dynomer->handle, "DYNOMER_API");
            if(api != NULL) {
                dynomer->api = *api;
                if(dynomer->state == NULL) {
                    dynomer->state = dynomer->api.init();
                }
                dynomer->api.reload(dynomer->state);
            } else {
                dlclose(dynomer->handle);
                dynomer->handle = NULL;
                dynomer->id = 0;
            }
        } else {
            dynomer->handle = NULL;
            dynomer->id = 0;
        }
    }
}


static void dynomer_unload(struct dynomer_t *dynomer) 
{
    if(dynomer->handle) {
        dynomer->api.finalize(dynomer->state);
        dynomer->state = NULL;
        dlclose(dynomer->handle);
        dynomer->handle = NULL;
        dynomer->id = 0;
    }
}


int main(int argc, char *argv[])
{
    struct dynomer_t dynomer = {0};
    dynomer::platform::init();

    while(!quit) {
        dynomer_load(&dynomer);
        if(dynomer.handle) {
            if(!dynomer.api.step(dynomer.state)) {
                break;
            }
        }

        usleep(100000);
    }

    dynomer_unload(&dynomer);
}
