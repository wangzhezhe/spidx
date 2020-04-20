#ifndef SPX_HASH_DOMAIN
#define SPX_HASH_DOMAIN

#include <stdint.h>
#include "spx_common.h"
#include "../src/utils/bbxtool.h"
#include "../src/utils/uthash.h"

#ifdef __cplusplus
extern "C" {
#endif

//this data structure aims to store different hash domains
typedef struct global_domain_map_t{
    char name_global_domain[256];
    bbx_t global_domain;
    UT_hash_handle hh;         /* makes this structure hashable */
}global_domain_map_t;

extern struct global_domain_map_t *g_domain_map;

//register the global domains into the spidx (there might be several global domains)
int spx_hash_global_domain_register(
    char* name_global_domain,
    bbx_t* global_domain
);

int spx_hash_get_global_domain(char *name, bbx_t *bbx);

int spx_hash_delete_global_domain_map();

global_domain_map_t* spx_hash_get_domain_map();

#ifdef __cplusplus
}
#endif

#endif 