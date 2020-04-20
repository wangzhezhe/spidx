#include "../src/hashmodules/hash_mod.h"

/* externs pointing to api for each module */
//hilbert sfc mod map, this is initilised at the modules/sfc.c
extern spx_hash_mod_map_t hilbertsfc_mod_map;
//nonspatial key mod map
//extern struct spx_hash_mod_map nsk_mod_map;

/* table of available modules */
static spx_hash_mod_map_t *table[] =
{
        &hilbertsfc_mod_map,
        //&nsk_mod_map,
        NULL,
};

spx_hash_mod_t *spx_hash_mod_initialize(const char *name,
                                               int n_svrs, int virt_factor, int seed)
{
    spx_hash_mod_t *instance = NULL;
    int i;

    for (i = 0; table[i] != NULL; i++)
    {
        if (strcmp(name, table[i]->type) == 0)
        {
            instance = malloc(sizeof(*instance));
            if (instance)
            {
                instance = table[i]->initiate(n_svrs, virt_factor, seed);
                if (!instance)
                {
                    free(instance);
                    instance = NULL;
                }
            }
            break;
        }
    }

    return instance;
}

void spx_hash_spatial_key(spx_hash_mod_t *instance,
                          bbx_t spatial_key,
                          char *name_global_domain,
                          server_index_t *server_idxs)
{
    instance->spx_hash_spatial_key(instance, spatial_key, name_global_domain, server_idxs);
    return;
}

void spx_hash_nonspatial_key(spx_hash_mod_t *instance,
                             spx_nonskey_entry *spx_nons_key,
                             server_index_t *server_idxs)
{
    instance->spx_hash_nonspatial_key(instance, spx_nons_key, server_idxs);
    return;
}


void spx_hash_finalize(spx_hash_mod_t *instance)
{

    instance->spx_hash_finalize(instance);
    return;
}