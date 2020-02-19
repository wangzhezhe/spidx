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
int spx_init(margo_instance_id mid);

//register the initial partition into current server
int spx_register_hash_domain(uint32_t global_dim, uint64_t* global_lb, uint64_t* global_ub);

//update the spidx key
int spx_update(spx_index_key_t spx_index_key, spx_index_spatial_t spx_index_spatial);

//query the spidx key
int spx_query(spx_index_spatial_t spx_index_spatial, spx_partition_id_bundle_t* list_of_spatial_id_bundle);

//finalize the spidx service
int spx_finalize();

#endif