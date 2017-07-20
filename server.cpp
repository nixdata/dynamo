#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include "dynomer.h"


const char *DYNOMER_LIBRARY = "./libdynomer.so";


int quit = 0;


struct dyno_t {
    void *handle;
    ino_t id;
    struct dyno_api_t api;
    struct dyno_state_t *state;
};


static void dyno_load(dyno_t *dyno)
{
    struct stat attr;
    if((stat(DYNOMER_LIBRARY, &attr) == 0) && (dyno->id != attr.st_ino)) {
        if(dyno->handle) {
            dyno->api.unload(dyno->state);
            dlclose(dyno->handle);
        }

        void *handle = dlopen(DYNOMER_LIBRARY, RTLD_NOW);
        if(handle) {
            dyno->handle = handle;
            dyno->id = attr.st_ino;
            const struct dyno_api_t *api = (const struct dyno_api_t *)dlsym(dyno->handle, "DYNOMER_API");
            if(api != NULL) {
                dyno->api = *api;
                if(dyno->state == NULL) {
                    dyno->state = dyno->api.init();
                }
                dyno->api.reload(dyno->state);
            } else {
                dlclose(dyno->handle);
                dyno->handle = NULL;
                dyno->id = 0;
            }
        } else {
            dyno->handle = NULL;
            dyno->id = 0;
        }
    }
}


static void dyno_unload(dyno_t *dyno) 
{
    if(dyno->handle) {
        dyno->api.finalize(dyno->state);
        dyno->state = NULL;
        dlclose(dyno->handle);
        dyno->handle = NULL;
        dyno->id = 0;
    }
}


int main(int argc, char *argv[])
{
    dyno_t dyno = {0};
    dyno_sys_init();

    while(!quit) {
        dyno_load(&dyno);
        if(dyno.handle) {
            if(!dyno.api.step(dyno.state)) {
                break;
            }
        }

        usleep(100000);
    }

    dyno_unload(&dyno);
}
