#include "spx_server.h"
#include "spx_error.h"
#include "types.h"

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
    spx_provider_t p;
    hg_id_t id;
    hg_bool_t flag;

    flag = margo_is_listening(mid);
    if (flag == HG_FALSE)
    {
        fprintf(stderr, "spx_provider_register(): margo instance is not a server.");
        return SPIDX_FAILURE;
    }

    margo_provider_registered_name(mid, "spx_update", provider_id, &id, &flag);
    if (flag == HG_TRUE)
    {
        fprintf(stderr, "alpha_provider_register(): a provider with the same provider id (%d) already exists.\n", provider_id);
        return SPIDX_FAILURE;
    }

    margo_provider_registered_name(mid, "spx_query", provider_id, &id, &flag);
    if (flag == HG_TRUE)
    {
        fprintf(stderr, "alpha_provider_register(): a provider with the same provider id (%d) already exists.\n", provider_id);
        return SPIDX_FAILURE;
    }

    p = (spx_provider_t)calloc(1, sizeof(*p));
    if (p == NULL)
        return SPIDX_FAILURE;

    p->mid = mid;

    id = MARGO_REGISTER_PROVIDER(mid, "spx_query",
                                 spx_query_in, spx_query_out,
                                 spx_query_ult, provider_id, pool);
    margo_register_data(mid, id, (void *)p, NULL);
    p->update_id = id;
    /* add other RPC registration here */
    id = MARGO_REGISTER_PROVIDER(mid, "spx_update",
                                 spx_update_in, spx_update_out,
                                 spx_update_ult, provider_id, pool);
    margo_register_data(mid, id, (void *)p, NULL);
    p->query_id = id;

    margo_provider_push_finalize_callback(mid, p, &spx_finalize_provider, p);

    *provider = p;
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

static void spx_query_ult(hg_handle_t h)
{
    fprintf(stdout, "execute spx_query_ult\n");
    return;
}

static void spx_update_ult(hg_handle_t h)
{
    fprintf(stdout, "execute spx_update_ult\n");
    return;
}

DEFINE_MARGO_RPC_HANDLER(spx_query_ult)
DEFINE_MARGO_RPC_HANDLER(spx_update_ult)