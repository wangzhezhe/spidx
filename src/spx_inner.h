// the inner data structure of the spidx
#ifndef SPIDX_INNER_H
#define SPIDX_INNER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils/bbxtool.h"
#include "utils/list.h"
#include "spx_common.h"


// This is a data structure that can be used to decode and encode an index key
// AND map an index key into a set of responsible indexing servers
struct spx_hashing_domain{
    BBX* m_global_domain;
    //every element in this array is the structure of spx_spatial_info
    struct list_head m_list_spatial_info;
};

int spx_inner_create_key(spx_index_key* output_spx_key,
char* input_string,
int input_int,
BBX* input_bbx);

int spx_inner_create_hash_dom(spx_hashing_domain* output_hash_domain,
char* spx_hash_method,
BBX*global_domain,
spx_index_nonspatial* input_spx_nonspatial_key);

int spx_innter_update(spx_index_key* input_hash_domain, int input_int);

int spx_query(spx_index_key* input_hash_domain,
spx_partition_id_bundle* query_list);

#endif