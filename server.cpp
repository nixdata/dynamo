#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include "dynamo.h"
#include "dynamo-system.h"


const char *DYNAMO_LIBRARY = "./libdynamo.so";


int quit = 0;


struct dmo_t {
    void *handle;
    ino_t id;
    struct dmo_api_t api;
    struct dmo_state_t *state;
};


static void dmo_load(dmo_t *dmo)
{
    struct stat attr;
    if((stat(DYNAMO_LIBRARY, &attr) == 0) && (dmo->id != attr.st_ino)) {
        if(dmo->handle) {
            dmo->api.unload(dmo->state);
            dlclose(dmo->handle);
        }

        void *handle = dlopen(DYNAMO_LIBRARY, RTLD_NOW);
        if(handle) {
            dmo->handle = handle;
            dmo->id = attr.st_ino;
            const struct dmo_api_t *api = (const struct dmo_api_t *)dlsym(dmo->handle, "DYNAMO_API");
            if(api != NULL) {
                dmo->api = *api;
                if(dmo->state == NULL) {
                    dmo->state = dmo->api.init();
                }
                dmo->api.reload(dmo->state);
            } else {
                dlclose(dmo->handle);
                dmo->handle = NULL;
                dmo->id = 0;
            }
        } else {
            dmo->handle = NULL;
            dmo->id = 0;
        }
    }
}


static void dmo_unload(dmo_t *dmo) 
{
    if(dmo->handle) {
        dmo->api.finalize(dmo->state);
        dmo->state = NULL;
        dlclose(dmo->handle);
        dmo->handle = NULL;
        dmo->id = 0;
    }
}


int main(int argc, char *argv[])
{
    dmo_t dmo = {0};

    while(!quit) {
        dmo_load(&dmo);
        if(dmo.handle) {
            if(!dmo.api.step(dmo.state)) {
                break;
            }
        }

        usleep(100000);
    }

    dmo_unload(&dmo);
}
