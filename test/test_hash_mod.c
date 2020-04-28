
#include "spx_hash_mod.h"

void test_mod_hilbertsfc()
{

    hash_specific_parameterset_t* hash_parameters = malloc(sizeof(hash_specific_parameterset_t));
    
    //assign necessary values to the the parameter set such as global domain

    spx_hash_mod_t *sfc_mode = spx_hash_mod_initialize("hilbertsfc", hash_parameters);
    bbx_t testBBX;
    
    sfc_mode->spx_hash_spatial_key(sfc_mode, testBBX, "test", NULL);

    sfc_mode->spx_hash_nonspatial_key(sfc_mode, NULL, NULL);

    sfc_mode->spx_hash_finalize(sfc_mode);

    return;
}

void test_mod_nskeymod(){
    hash_specific_parameterset_t* hash_parameters = malloc(sizeof(hash_specific_parameterset_t));
    
    //assign necessary values to the the parameter set such as global domain
    hash_parameters->total_service_number=10;

    spx_hash_mod_t *nskeymod_mode = spx_hash_mod_initialize("nskeymod", hash_parameters);
    
    //generate the nonspatial key
    int64_t key_int = 12345;
    spx_nonskey_entry *entry_nonskey = (spx_nonskey_entry *)malloc(sizeof(spx_nonskey_entry));
    entry_nonskey->type = SPX_ELEM_INT64;
    entry_nonskey->value = (void *)(&key_int);

    //init the id info
    server_index_t* server_idxs = (server_index_t*)malloc(sizeof(server_index_t));

    nskeymod_mode->spx_hash_nonspatial_key(nskeymod_mode, entry_nonskey, server_idxs);

    //check the hash results
    fprintf(stdout,"the hash value for nskeymodule is %d\n",server_idxs->serverID);

    nskeymod_mode->spx_hash_finalize(nskeymod_mode);
}

int main()
{
    test_mod_hilbertsfc();

    test_mod_nskeymod();

    return 0;
}