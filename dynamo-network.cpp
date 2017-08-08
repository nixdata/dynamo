#include <stdio.h>
#include "dynamo.h"
#include "dynamo-network.h"
#include "./lib/netcode.io/c/netcode.h"


int dmo_net_startup()
{
    if(netcode_init() != NETCODE_OK) {
        printf("ERROR: Failed to initialize netcode.io\n");
        return 1;
    } else {
        printf("dmo_net_startup: OK!\n");
    }

    return 1;
}


int dmo_net_update(dmo_time time)
{
    return 1;
}


int dmo_net_shutdown()
{
    return 1;
}
