//refer to ch_placement
//use the factory pattern to expose generalized hash method
//the hash function can be implemented by specific hash functions
//this capability is supposed to be binded with the client

#ifndef SPX_HASH
#define SPX_HASH

#include <stdint.h>
#include "spx_common.h"
#include "../src/utils/bbxtool.h"
#include "../src/utils/uthash.h"

#ifdef __cplusplus
extern "C" {
#endif

/* maximum replication factor allowed by library */
//#define MAX_REPLICATION 5

struct spx_hash_instance;

struct spx_hash_instance* spx_hash_initialize(const char* name, 
    int n_svrs, int virt_factor, int seed);

int spx_hash_finalize();

typedef struct server_index_t{
    size_t serverID;
}server_index_t;

//input the spatial key and get the server id
server_index_t* spx_hash_spatial_key(
    bbx_t spatial_key,
    char* name_global_domain
);

//input the non spatial key and get the coresponding id
server_index_t* spx_hash_nonspatial_key(
    spx_nonskey_entry *spx_nons_key
);

//register the global domains into the spidx (there might be several global domains)
int spx_hash_global_domain_register(
    char* name_global_domain,
    bbx_t* global_domain
);

int spx_hash_get_global_domain(char *name, bbx_t *bbx);

int spx_hash_delete_global_domain_map();

typedef struct global_domain_map_t{
    char name_global_domain[256];
    bbx_t global_domain;
    UT_hash_handle hh;         /* makes this structure hashable */
}global_domain_map_t;

extern struct global_domain_map_t *g_domain_map;

#ifdef __cplusplus
}
#endif

#endif 