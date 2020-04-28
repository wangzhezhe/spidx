#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "hash_mod.h"

//the declaration of the function instead of the pointer to the function
static spx_hash_mod_t *spx_hash_mod_nskeymod_init(hash_specific_parameterset_t *hash_parameters);

static void spx_hash_mod_nskeymod_spatial(spx_hash_mod_t *mod,
                                          bbx_t spatial_key,
                                          char *name_global_domain,
                                          server_index_t *server_idxs);

static void spx_hash_mod_nskeymod_nons(spx_hash_mod_t *mod,
                                       spx_nonskey_entry *spx_nons_key,
                                       server_index_t *server_idxs);

static void spx_hash_mod_nskeymod_finalize(spx_hash_mod_t *mod);

//the element used for indexing
spx_hash_mod_map_t nskeymod_mod_map =
    {
        .type = "nskeymod",
        .initiate = spx_hash_mod_nskeymod_init,
};

int nskeymod_total_server_num = 0;

//extract necessary information from the hash_parameters
static spx_hash_mod_t *spx_hash_mod_nskeymod_init(hash_specific_parameterset_t *hash_parameters)
{
    fprintf(stdout, "call the spx_hash_mod_nskeymod\n");
    spx_hash_mod_t *mod_nskeymod = malloc(sizeof(spx_hash_mod_t));

    mod_nskeymod->spx_hash_spatial_key = spx_hash_mod_nskeymod_spatial;
    mod_nskeymod->spx_hash_nonspatial_key = spx_hash_mod_nskeymod_nons;
    mod_nskeymod->spx_hash_finalize = spx_hash_mod_nskeymod_finalize;

    nskeymod_total_server_num = hash_parameters->total_service_number;

    return mod_nskeymod;
}

static void spx_hash_mod_nskeymod_spatial(spx_hash_mod_t *mod,
                                          bbx_t spatial_key,
                                          char *name_global_domain,
                                          server_index_t *server_idxs)
{
    fprintf(stdout, "do not support spx_hash_mod_nskeymod_spatial\n");
    return;
}

static void spx_hash_mod_nskeymod_nons(spx_hash_mod_t *mod,
                                       spx_nonskey_entry *spx_nons_key,
                                       server_index_t *server_idxs)
{
    //transfer the nonskey into the string and hash it to the id
    if (spx_nons_key == NULL)
    {
        fprintf(stderr, "spx_nons_key should not be null\n");
        return;
    }
    size_t server_id = 0;
    if (spx_nons_key->type == SPX_ELEM_STR)
    {
    }
    else if (spx_nons_key->type == SPX_ELEM_INT64)
    {
        if (nskeymod_total_server_num == 0)
        {
            fprintf(stderr, "nskeymod_total_server_num should larger than 0\n");
            return;
        }
        server_id = *(int64_t*)spx_nons_key->value;

        server_id = server_id % nskeymod_total_server_num;
    }
    else
    {
        fprintf(stdout, "unsupported element type\n");
    }

    server_idxs->serverID = server_id;

    return;
}

static void spx_hash_mod_nskeymod_finalize(spx_hash_mod_t *mod)
{
    fprintf(stdout, "todo call the spx_hash_mod_nskeymod_finalize\n");
    return;
}
