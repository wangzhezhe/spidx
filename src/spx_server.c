#include "spx_server.h"
#include "spx_error.h"
#include "types.h"
#include "assert.h"

struct spx_provider
{
    margo_instance_id mid;
    hg_id_t update_id;
    hg_id_t query_id;
    /* other provider-specific data */
};

static void spx_finalize_provider(void *p);

DECLARE_MARGO_RPC_HANDLER(spx_update_ult);
DECLARE_MARGO_RPC_HANDLER(spx_query_ult);

static void spx_update_ult(hg_handle_t h);
static void spx_query_ult(hg_handle_t h);
/* add other RPC declarations here */

int spx_provider_register(
    margo_instance_id mid,
    uint16_t provider_id,
    ABT_pool pool,
    spx_provider_t *provider)
{
    hg_id_t update_id;
    hg_id_t query_id;

    hg_bool_t flag;

    flag = margo_is_listening(mid);
    if (flag == HG_FALSE)
    {
        fprintf(stderr, "spx_provider_register(): margo instance is not a server.");
        return SPIDX_FAILURE;
    }

    margo_provider_registered_name(mid, "spx_update", provider_id, &update_id, &flag);
    if (flag == HG_TRUE)
    {
        fprintf(stderr, "spx_provider_register(): a provider with the same provider id (%d) already exists.\n", provider_id);
        return SPIDX_FAILURE;
    }

    margo_provider_registered_name(mid, "spx_query", provider_id, &query_id, &flag);
    if (flag == HG_TRUE)
    {
        fprintf(stderr, "spx_provider_register(): a provider with the same provider id (%d) already exists.\n", provider_id);
        return SPIDX_FAILURE;
    }

    spx_provider_t server;

    server = (spx_provider_t)calloc(1, sizeof(*server));
    if (server == NULL)
        return SPIDX_FAILURE;

    server->mid = mid;

    query_id = MARGO_REGISTER_PROVIDER(mid, "spx_query",
                                       spx_query_in, spx_query_out,
                                       spx_query_ult, provider_id, pool);
    margo_register_data(mid, query_id, (void *)server, NULL);
    server->query_id = query_id;

    /* add other RPC registration here */
    update_id = MARGO_REGISTER_PROVIDER(mid, "spx_update",
                                        spx_update_in_t, spx_update_out_t,
                                        spx_update_ult, provider_id, pool);
    margo_register_data(mid, update_id, (void *)server, NULL);
    server->update_id = update_id;

    margo_provider_push_finalize_callback(mid, server, &spx_finalize_provider, server);

    *provider = server;
    return SPIDX_SUCCESS;
}

static void spx_finalize_provider(void *p)
{
    spx_provider_t provider = (spx_provider_t)p;
    margo_deregister(provider->mid, provider->update_id);
    margo_deregister(provider->mid, provider->query_id);

    /* deregister other RPC ids ... */
    free(provider);
}

int spx_provider_destroy(
    spx_provider_t provider)
{
    /* pop the finalize callback */
    margo_provider_pop_finalize_callback(provider->mid, provider);
    /* call the callback */
    spx_finalize_provider(provider);

    return SPIDX_SUCCESS;
}

static void spx_update_ult(hg_handle_t h)
{
    fprintf(stdout, "execute spx_update_ult\n");

    hg_return_t ret;
    spx_update_in_t in;
    spx_update_out_t out;

    margo_instance_id mid = margo_hg_handle_get_instance(h);

    const struct hg_info *info = margo_get_info(h);
    spx_provider_t provider = (spx_provider_t)margo_registered_data(mid, info->id);

    ret = margo_get_input(h, &in);

    fprintf(stdout, "debug size %d\n", in.spx_key.size);

    //check the input
    //int32_t associated_id = in.associated_id;
    int32_t associated_id = 0;

    spx_index_key_t *server_spx_key = (spx_index_key_t *)in.spx_key.raw_obj;
    fprintf(stdout, "server get nons_key %s\nassociated_id %d\nbbx\n", server_spx_key->m_index_nonspatial, associated_id);
    printbbx(&server_spx_key->m_index_spatial);
    
    out.ret=321123;
    
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

DEFINE_MARGO_RPC_HANDLER(spx_query_ult)
DEFINE_MARGO_RPC_HANDLER(spx_update_ult)