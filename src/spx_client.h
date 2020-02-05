//spidex client to recieve the API
#ifndef SPIDX_CLIENT_H
#define SPIDX_CLIENT_H

#include "spx_common.h"

//encode the spidx key
//return value is a encoded string
char* spx_client_encode_spatital_key(uint32_t dim, uint64_t* lb, uint64_t* ub);

//decode the respond of spidx query 
//return value is a list of spx_spatial_info
spx_partition_id_bundle_list* spx_client_decode_query_respond(char *encoded_spx_partition_id_bundle_list);

int spx_client_update(char* variable_name, 
uint32_t version, 
char*encode_spatital_key,
uint32_t associated_id);

spx_partition_id_bundle_list* spx_client_query(char* variable_name, 
uint32_t version, 
char*encode_spatital_key);

#endif