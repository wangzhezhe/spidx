// the server of the spidx that expose the inner functionality 
#ifndef SPIDX_SERVER_H
#define SPIDX_SERVER_H

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <mercury.h>
#include <abt.h>
#include <margo.h>
#include "types.h"


//init the mochi service and expose the spidx RPC
int spx_server_init(margo_instance_id mid,
uint32_t global_dim, uint64_t* global_lb, uint64_t* global_ub);

//finalize the spidx service
int spx_server_finalize();

#endif