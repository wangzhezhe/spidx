#ifndef SPIDX_H
#define SPIDX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bbxtool.h"

struct spx_spatial_info{
    BBX* m_partition;
    int m_int;
};

// This is a data structure that can be used to decode and encode an index key
// AND map an index key into a set of responsible indexing servers
struct spx_hashing_domain{
    BBX* m_global_domain;
    spx_spatial_info* m_list_spatial_info;
};

struct spx_index_nonspatial{
    char* m_string;
    int m_int;
};

//the spatial index is represented by the bounding box
struct spx_index_spatial{
    BBX* m_bbx;
};

//the struct for the spx key
struct spx_index_key{
    spx_index_nonspatial* m_index_nonspatial;
    spx_index_spatial* m_index_spatial;
};

int spx_init(int spx_total_server_num);

int spx_create_key(spx_index_key* output_spx_key,
char* input_string,
int input_int,
BBX* input_bbx);

int spx_create_hash_dom(spx_hashing_domain* output_hash_domain,
char* spx_hash_method,
BBX*global_domain,
spx_index_nonspatial* input_spx_nonspatial_key);

int spx_update(spx_index_key* input_hash_domain, int input_int);

int spx_query(spx_index_key* input_hash_domain,
spx_spatial_info* query_list);

int spx_finalize();

#endif