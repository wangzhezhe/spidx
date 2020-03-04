#include "types.h"
#include "spx_client.h"
#include "spx_error.h"

struct spx_client
{
    margo_instance_id mid;
    hg_id_t spx_update_id;
    hg_id_t spx_query_id;
    uint64_t num_prov_hdl;
};

struct spx_provider_handle
{
    spx_client_t client;
    hg_addr_t addr;
    uint16_t provider_id;
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
    hg_id_t id;

    margo_registered_name(mid, "spx_update", &id, &flag);
    if (flag == HG_TRUE)
    {
        margo_registered_name(mid, "spx_update", &c->spx_update_id, &flag);
    }
    else
    {
        c->spx_update_id = MARGO_REGISTER(mid, "spx_update", spx_update_in, spx_update_out, NULL);
    }

    margo_registered_name(mid, "spx_query", &id, &flag);
    if (flag == HG_TRUE)
    {
        margo_registered_name(mid, "spx_query", &c->spx_query_id, &flag);
    }
    else
    {
        c->spx_query_id = MARGO_REGISTER(mid, "spx_update", spx_query_in, spx_query_out, NULL);
    }

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
    uint16_t provider_id,
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
    ph->provider_id = provider_id;
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

int spx_client_update(
    spx_provider_handle_t handle,
    spx_index_key_t spx_index_key,
    int associated_id){
    
    hg_handle_t   h;
    spx_update_in update_in_arg;
    spx_update_out update_resp;
    hg_return_t ret;

    //encode the input spatial and nonspatial key into string

    ret = margo_create(handle->client->mid, handle->addr, handle->client->spx_update_id, &h);
    if(ret != HG_SUCCESS)
        return SPIDX_FAILURE;

    ret = margo_provider_forward(handle->provider_id, h, &update_in_arg);
    if(ret != HG_SUCCESS) {
        margo_destroy(h);
        return SPIDX_FAILURE;
    }

    ret = margo_get_output(h, &update_resp);
    if(ret != HG_SUCCESS) {
        margo_destroy(h);
        return SPIDX_FAILURE;
    }

    margo_free_output(h, &update_resp);
    margo_destroy(h);
    return SPIDX_SUCCESS;

}

int spx_client_query(
    spx_provider_handle_t handle,
    bbx_t* spx_index_spatial, 
    spx_domain_id_bundle_t* bundle_list){


    hg_handle_t h;
    spx_query_in query_in_arg;
    spx_query_out query_resp;
    hg_return_t ret;

    //encode the input spatial and nonspatial key into string

    ret = margo_create(handle->client->mid, handle->addr, handle->client->spx_update_id, &h);
    if(ret != HG_SUCCESS)
        return SPIDX_FAILURE;

    ret = margo_provider_forward(handle->provider_id, h, &query_in_arg);
    if(ret != HG_SUCCESS) {
        margo_destroy(h);
        return SPIDX_FAILURE;
    }

    ret = margo_get_output(h, &query_resp);
    if(ret != HG_SUCCESS) {
        margo_destroy(h);
        return SPIDX_FAILURE;
    }

    //decode the response into the spx_partition_id_bundle_t

    margo_free_output(h, &query_resp);
    margo_destroy(h);
    return SPIDX_SUCCESS;

}

