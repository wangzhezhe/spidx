
#ifndef SPX_HASH_MODULES
#define SPX_HASH_MODULES

#include <stdint.h>
#include "spx_common.h"
#include "../src/utils/bbxtool.h"


#ifdef __cplusplus
extern "C" {
#endif


//the id for server index
typedef struct server_index_t{
    size_t serverID;
}server_index_t;

//this is a generalized struct applied for different modules
typedef struct spx_hash_mod
{
    void (*spx_hash_spatial_key)(struct spx_hash_mod *mod, 
    bbx_t spatial_key,
    char* name_global_domain,
    server_index_t* server_idxs);

    void (*spx_hash_nonspatial_key)(struct spx_hash_mod *mod, 
    spx_nonskey_entry *spx_nons_key,
    server_index_t* server_idxs);
    
    void (*spx_hash_finalize)(struct spx_hash_mod *mod);

}spx_hash_mod_t;

//different module can be registered into the map
//the key of the map if the module name, the value of the map is the spx_hash_mod
//that wrap different hash functions
typedef struct spx_hash_mod_map
{
    char* type;
    spx_hash_mod_t* (*initiate)(int n_svrs, int virt_factor, int seed);
}spx_hash_mod_map_t;



#ifdef __cplusplus
}
#endif

#endif 