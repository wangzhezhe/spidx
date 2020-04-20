#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "hash_mod.h"

//the declaration of the function instead of the pointer to the function
static spx_hash_mod_t* spx_hash_mod_hilbertsfc_init(hash_specific_parameterset_t* hash_parameters);


static void spx_hash_mod_hilbertsfc_spatial(spx_hash_mod_t *mod,
                                        bbx_t spatial_key,
                                        char *name_global_domain,
                                        server_index_t *server_idxs);

static void spx_hash_mod_hilbertsfc_nons(spx_hash_mod_t *mod,
                                     spx_nonskey_entry *spx_nons_key,
                                     server_index_t *server_idxs);

static void spx_hash_mod_hilbertsfc_finalize(spx_hash_mod_t *mod);

//the element used for indexing
spx_hash_mod_map_t hilbertsfc_mod_map =
{
        .type = "hilbertsfc",
        .initiate = spx_hash_mod_hilbertsfc_init,
};

static spx_hash_mod_t *spx_hash_mod_hilbertsfc_init(hash_specific_parameterset_t* hash_parameters)
{
    fprintf(stdout, "call the spx_hash_mod_hilbertsfc\n");
    spx_hash_mod_t *mod_hilbertsfc = malloc(sizeof(spx_hash_mod_t));

    mod_hilbertsfc->spx_hash_spatial_key = spx_hash_mod_hilbertsfc_spatial;
    mod_hilbertsfc->spx_hash_nonspatial_key = spx_hash_mod_hilbertsfc_nons;
    mod_hilbertsfc->spx_hash_finalize = spx_hash_mod_hilbertsfc_finalize;

    return mod_hilbertsfc;
}

static void spx_hash_mod_hilbertsfc_spatial(spx_hash_mod_t *mod,
                                        bbx_t spatial_key,
                                        char *name_global_domain,
                                        server_index_t *server_idxs)
{
    fprintf(stdout, "todo call the spx_hash_mod_hilbertsfc_spatial\n");
    return;
}

static void spx_hash_mod_hilbertsfc_nons(spx_hash_mod_t *mod,
                                     spx_nonskey_entry *spx_nons_key,
                                     server_index_t *server_idxs)
{
    fprintf(stdout, "todo call the spx_hash_mod_hilbertsfc_nons\n");
    return;
}

static void spx_hash_mod_hilbertsfc_finalize(spx_hash_mod_t *mod)
{
    fprintf(stdout, "todo call the spx_hash_mod_hilbertsfc_finalize\n");
    return;
}
