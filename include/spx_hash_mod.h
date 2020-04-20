//refer to ch_placement
//use the factory pattern to expose generalized hash method
//the hash function can be implemented by specific hash functions
//this capability is supposed to be binded with the client

#ifndef SPX_HASH_MOD
#define SPX_HASH_MOD

#include "../src/hashmodules/hash_mod.h"

#ifdef __cplusplus
extern "C" {
#endif


spx_hash_mod_t *spx_hash_mod_initialize(const char *name,
                                               int n_svrs, int virt_factor, int seed);

void spx_hash_spatial_key(spx_hash_mod_t *instance,
                          bbx_t spatial_key,
                          char *name_global_domain,
                          server_index_t *server_idxs);

void spx_hash_nonspatial_key(spx_hash_mod_t *instance,
                             spx_nonskey_entry *spx_nons_key,
                             server_index_t *server_idxs);

void spx_hash_finalize(spx_hash_mod_t *instance);


#ifdef __cplusplus
}
#endif

#endif 