
#include "spx_hash_mod.h"

void test_mod_sfc()
{
    spx_hash_mod_t *sfc_mode = spx_hash_mod_initialize("hilbertsfc", 1, 2, 3);
    bbx_t testBBX;
    sfc_mode->spx_hash_spatial_key(sfc_mode, testBBX, "test", NULL);

    sfc_mode->spx_hash_nonspatial_key(sfc_mode, NULL, NULL);

    sfc_mode->spx_hash_finalize(sfc_mode);

    return;
}

void main()
{
    test_mod_sfc();
}