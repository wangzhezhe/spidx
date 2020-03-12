#include "types.h"
#include "spx_client.h"
#include "spx_error.h"
#include <string.h>

struct spx_client
{
    margo_instance_id mid;
    hg_id_t spx_update_id;
    hg_id_t spx_query_id;
    hg_id_t spx_register_id;

    uint64_t num_prov_hdl;
};

struct spx_provider_handle
{
    spx_client_t client;
    hg_addr_t addr;
    uint64_t refcount;
};

int spx_client_init(margo_instance_id mid, spx_client_t *client)
{
    int ret = SPIDX_SUCCESS;

    spx_client_t c = (spx_client_t)calloc(1, sizeof(*c));
    if (!c)
        return SPIDX_SUCCESS;

    c->mid = mid;

    hg_bool_t flag;
    hg_id_t update_id;

    c->spx_register_id = MARGO_REGISTER(mid, "spx_register", spx_register_in_t, spx_register_out_t, NULL);
    c->spx_update_id = MARGO_REGISTER(mid, "spx_update", spx_update_in_t, spx_update_out_t, NULL);
    c->spx_query_id = MARGO_REGISTER(mid, "spx_query", spx_query_in_t, spx_query_out_t, NULL);

    *client = c;
    return SPIDX_SUCCESS;
}

int spx_client_finalize(spx_client_t client)
{
    if (client->num_prov_hdl != 0)
    {
        fprintf(stderr,
                "Warning: %d provider handles not released when spx_client_finalize was called\n",
                client->num_prov_hdl);
    }
    free(client);
    return SPIDX_SUCCESS;
}

int spx_provider_handle_create(
    spx_client_t client,
    hg_addr_t addr,
    spx_provider_handle_t *handle)
{
    if (client == SPIDX_CLIENT_NULL)
        return SPIDX_FAILURE;

    spx_provider_handle_t ph =
        (spx_provider_handle_t)calloc(1, sizeof(*ph));

    if (!ph)
        return SPIDX_FAILURE;

    hg_return_t ret = margo_addr_dup(client->mid, addr, &(ph->addr));
    if (ret != HG_SUCCESS)
    {
        free(ph);
        return SPIDX_FAILURE;
    }

    ph->client = client;
    ph->refcount = 1;

    client->num_prov_hdl += 1;

    *handle = ph;
    return SPIDX_SUCCESS;
}

int spx_provider_handle_ref_incr(
    spx_provider_handle_t handle)
{
    if (handle == SPIDX_PROVIDER_HANDLE_NULL)
        return SPIDX_FAILURE;
    handle->refcount += 1;
    return SPIDX_SUCCESS;
}

int spx_provider_handle_release(spx_provider_handle_t handle)
{
    if (handle == SPIDX_PROVIDER_HANDLE_NULL)
        return SPIDX_FAILURE;
    handle->refcount -= 1;
    if (handle->refcount == 0)
    {
        margo_addr_free(handle->client->mid, handle->addr);
        handle->client->num_prov_hdl -= 1;
        free(handle);
    }
    return SPIDX_SUCCESS;
}

int spx_client_register(
    spx_provider_handle_t handle,
    bbx_t *hash_domain)
{
    hg_handle_t h;
    spx_register_in_t in;
    spx_register_out_t out;
    hg_return_t ret;

    in.hash_domain.size = sizeof(bbx_t);
    in.hash_domain.raw_obj = (char *)hash_domain;

    ret = margo_create(handle->client->mid, handle->addr, handle->client->spx_register_id, &h);
    if (ret != HG_SUCCESS)
        return SPIDX_FAILURE;

    ret = margo_forward(h, &in);

    if (ret != HG_SUCCESS)
    {
        margo_destroy(h);
        return SPIDX_FAILURE;
    }

    ret = margo_get_output(h, &out);

    if (ret != HG_SUCCESS)
    {
        margo_destroy(h);
        return SPIDX_FAILURE;
    }

    int32_t status = out.ret;
    //the input should not be freed
    margo_free_output(h, &out);
    margo_destroy(h);
    return status;
}

int spx_client_update(
    spx_provider_handle_t handle,
    spx_nonskey_entry *spx_nons_key,
    bbx_t *spx_spatial_key,
    int32_t associated_id,
    HASHMETHOD method)
{

    hg_handle_t h;
    spx_update_in_t update_in_arg;
    hg_return_t ret;

    //generate the key
    spx_index_key_t *client_spx_key = (spx_index_key_t *)calloc(1, sizeof(spx_index_key_t));

    enocde_nonspatial_key(spx_nons_key, client_spx_key->m_index_nonspatial);

    client_spx_key->m_index_spatial.m_dims = spx_spatial_key->m_dims;
    int i;
    for (i = 0; i < DEFAULT_MAX_DIM; i++)
    {
        client_spx_key->m_index_spatial.m_lb[i] = spx_spatial_key->m_lb[i];
        client_spx_key->m_index_spatial.m_ub[i] = spx_spatial_key->m_ub[i];
    }

    //fill the obj
    update_in_arg.spx_key.size = sizeof(spx_index_key_t);
    update_in_arg.spx_key.raw_obj = (char *)client_spx_key;
    update_in_arg.associated_id = associated_id;

    ret = margo_create(handle->client->mid, handle->addr, handle->client->spx_update_id, &h);
    if (ret != HG_SUCCESS)
        return SPIDX_FAILURE;

    ret = margo_forward(h, &update_in_arg);

    if (ret != HG_SUCCESS)
    {
        margo_destroy(h);
        return SPIDX_FAILURE;
    }

    spx_update_out_t resp;

    ret = margo_get_output(h, &resp);

    if (ret != HG_SUCCESS)
    {
        free(client_spx_key);
        margo_destroy(h);
        return SPIDX_FAILURE;
    }

    int32_t status = resp.ret;

    //the input should not be freed
    margo_free_output(h, &resp);
    free(client_spx_key);
    margo_destroy(h);

    return status;
}

int spx_client_query(
    spx_provider_handle_t handle,
    bbx_t *spx_index_spatial,
    spx_domain_id_bundle_t *bundle_list)
{

    hg_handle_t h;
    spx_query_in_t query_in_arg;
    spx_query_out_t query_resp;
    hg_return_t ret;

    //encode the input spatial and nonspatial key into string

    ret = margo_create(handle->client->mid, handle->addr, handle->client->spx_update_id, &h);
    if (ret != HG_SUCCESS)
        return SPIDX_FAILURE;

    ret = margo_forward(h, &query_in_arg);
    if (ret != HG_SUCCESS)
    {
        margo_destroy(h);
        return SPIDX_FAILURE;
    }

    ret = margo_get_output(h, &query_resp);
    if (ret != HG_SUCCESS)
    {
        margo_destroy(h);
        return SPIDX_FAILURE;
    }

    //decode the response into the spx_partition_id_bundle_t

    margo_free_output(h, &query_resp);
    margo_destroy(h);
    return SPIDX_SUCCESS;
}