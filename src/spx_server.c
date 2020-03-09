#include "spx_server.h"
#include "spx_error.h"
#include "types.h"
#include "assert.h"

static void spx_finalize_provider(void *p);

DECLARE_MARGO_RPC_HANDLER(spx_update_ult);
//DECLARE_MARGO_RPC_HANDLER(spx_query_ult);

static void spx_update_ult(hg_handle_t h);
//static void spx_query_ult(hg_handle_t h);
/* add other RPC declarations here */

int spx_provider_register(
    margo_instance_id mid,
    spx_provider_t *provider)
{

    hg_bool_t flag;
    
    flag = margo_is_listening(mid);
    if (flag == HG_FALSE)
    {
        fprintf(stderr, "spx_provider_register(): margo instance is not a server.");
        return SPIDX_FAILURE;
    }

    provider = (spx_provider_t*)calloc(1, sizeof(spx_provider_t));
    if (provider == NULL)
        return SPIDX_FAILURE;

    provider->mid = mid;

/*
    hg_id_t query_id = MARGO_REGISTER(mid, "spx_query",
                                       spx_query_in, spx_query_out,
                                       spx_query_ult);
    margo_register_data(mid, query_id, (void *)provider, NULL);
    provider->query_id = query_id;

*/  
    hg_id_t update_id = MARGO_REGISTER(mid, "spx_update",
                                        spx_update_in_t, spx_update_out_t,
                                        spx_update_ult);

    margo_register_data(mid, update_id, (void *)provider, NULL);
    provider->update_id = update_id;


    return SPIDX_SUCCESS;
}

static void spx_finalize_provider(void *p)
{
    spx_provider_t * provider = (spx_provider_t *)p;
    margo_deregister(provider->mid, provider->update_id);
    //margo_deregister(provider->mid, provider->query_id);

    /* deregister other RPC ids ... */
    free(provider);
}

int spx_provider_destroy(
    spx_provider_t * provider)
{
    spx_finalize_provider(provider);

    return SPIDX_SUCCESS;
}

static void spx_update_ult(hg_handle_t h)
{

    hg_return_t ret;
    spx_update_in_t in;
    spx_update_out_t out;

    margo_instance_id mid = margo_hg_handle_get_instance(h);

    const struct hg_info *info = margo_get_info(h);
    spx_provider_t* provider = (spx_provider_t*)margo_registered_data(mid, info->id);

    ret = margo_get_input(h, &in);


    //check the input
    int32_t associated_id = in.associated_id;

    spx_index_key_t *server_spx_key = (spx_index_key_t *)in.spx_key.raw_obj;
    fprintf(stdout, "server get nons_key %s\nassociated_id %d\nbbx\n", server_spx_key->m_index_nonspatial, associated_id);
    printbbx(&server_spx_key->m_index_spatial);
    

    //call the inner 



    out.ret=0;
    
    ret = margo_respond(h, &out);
    assert(ret == HG_SUCCESS);

    ret = margo_free_input(h, &in);
    assert(ret == HG_SUCCESS);

    fflush(stdout);

    return;
}

static void spx_query_ult(hg_handle_t h)
{
    fprintf(stdout, "execute spx_query_ult\n");
    return;
}

//DEFINE_MARGO_RPC_HANDLER(spx_query_ult)
DEFINE_MARGO_RPC_HANDLER(spx_update_ult)