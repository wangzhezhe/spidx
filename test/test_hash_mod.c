
#include "spx_hash_mod.h"

void test_mod_sfc()
{

    hash_specific_parameterset_t* hash_parameters = malloc(sizeof(hash_specific_parameterset_t));
    
    //assign necessary values to the the parameter set
    spx_hash_mod_t *sfc_mode = spx_hash_mod_initialize("hilbertsfc", hash_parameters);
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