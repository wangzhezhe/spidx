#include "spx_server.h"
#include "spx_error.h"
#include "types.h"
#include "assert.h"
#include "spx_server_inner.h"

static void spx_finalize_provider(void *p);

DECLARE_MARGO_RPC_HANDLER(spx_register_ult);
DECLARE_MARGO_RPC_HANDLER(spx_update_ult);
DECLARE_MARGO_RPC_HANDLER(spx_query_ult);

static void spx_register_ult(hg_handle_t h);
static void spx_update_ult(hg_handle_t h);
static void spx_query_ult(hg_handle_t h);
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

    provider = (spx_provider_t *)calloc(1, sizeof(spx_provider_t));
    if (provider == NULL)
        return SPIDX_FAILURE;

    provider->mid = mid;


    hg_id_t register_id = MARGO_REGISTER(mid, "spx_register",
                                         spx_register_in_t, spx_register_out_t,
                                         spx_register_ult);

    margo_register_data(mid, register_id, (void *)provider, NULL);
    provider->register_id = register_id;

    hg_id_t update_id = MARGO_REGISTER(mid, "spx_update",
                                       spx_update_in_t, spx_update_out_t,
                                       spx_update_ult);

    margo_register_data(mid, update_id, (void *)provider, NULL);
    provider->update_id = update_id;


    hg_id_t query_id = MARGO_REGISTER(mid, "spx_query",
                                       spx_query_in_t, spx_query_out_t,
                                       spx_query_ult);
    margo_register_data(mid, query_id, (void *)provider, NULL);
    provider->query_id = query_id;

    return SPIDX_SUCCESS;
}

static void spx_finalize_provider(void *p)
{
    spx_provider_t *provider = (spx_provider_t *)p;
    margo_deregister(provider->mid, provider->update_id);
    //margo_deregister(provider->mid, provider->query_id);

    /* deregister other RPC ids ... */
    free(provider);
}

int spx_provider_destroy(
    spx_provider_t *provider)
{
    spx_finalize_provider(provider);

    return SPIDX_SUCCESS;
}

static void spx_register_ult(hg_handle_t h)
{
    hg_return_t ret;
    spx_register_in_t in;
    spx_register_out_t out;

    margo_instance_id mid = margo_hg_handle_get_instance(h);

    const struct hg_info *info = margo_get_info(h);
    spx_provider_t *provider = (spx_provider_t *)margo_registered_data(mid, info->id);

    ret = margo_get_input(h, &in);

    //check the input
    bbx_t *hash_domain = (bbx_t *)in.hash_domain.raw_obj;

    fprintf(stdout, "register following info into hash_domain");
    printbbx(hash_domain);

    //call the inner
    int status = spx_inner_register_domain(*hash_domain);
    if (status != SPIDX_SUCCESS)
    {
        fprintf(stderr, "---error: failed to register hash domain at server---\n");
    }

    out.ret = status;
    ret = margo_respond(h, &out);
    assert(ret == HG_SUCCESS);

    ret = margo_free_input(h, &in);
    assert(ret == HG_SUCCESS);
    fflush(stdout);
    return;
}

static void spx_update_ult(hg_handle_t h)
{

    hg_return_t ret;
    spx_update_in_t in;
    spx_update_out_t out;

    margo_instance_id mid = margo_hg_handle_get_instance(h);

    const struct hg_info *info = margo_get_info(h);
    spx_provider_t *provider = (spx_provider_t *)margo_registered_data(mid, info->id);

    ret = margo_get_input(h, &in);

    //check the input
    int32_t associated_id = in.associated_id;

    spx_index_key_t *server_spx_key = (spx_index_key_t *)in.spx_key.raw_obj;
    fprintf(stdout, "server get nons_key %s\nassociated_id %d\nbbx\n", server_spx_key->m_index_nonspatial, associated_id);
    printbbx(&server_spx_key->m_index_spatial);

    //call the inner
    int status = spx_inner_put(server_spx_key->m_index_nonspatial, server_spx_key->m_index_spatial, associated_id);

    if (status != SPIDX_SUCCESS)
    {
        fprintf(stderr, "---error: failed to update data into the spidx server---\n");
    }

    out.ret = status;

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

    hg_return_t ret;
    spx_query_in_t query_in;
    spx_query_out_t query_out;

    ret = margo_get_input(h, &query_in);

    spx_index_key_t *spx_index = (spx_index_key_t *)query_in.spx_key.raw_obj;

    //query the value by the spacific domain
    domain_id_boundle_t *domain_id_list = spx_inner_query(spx_index->m_index_nonspatial, spx_index->m_index_spatial);
    domain_id_boundle_t *elem_temp = NULL;
    int ifFirst=1;
    obj_list_entry_ptr prev = NULL;
    LL_FOREACH(domain_id_list, elem_temp)
    {
        //check the element in the list
        fprintf(stdout, "spatial info:\n");
        printBBXinfo(elem_temp->m_spatial_key);
        fprintf(stdout, "mem_id %d\n", elem_temp->m_mem_id);

        //copy the element

        //new element
        spx_domain_id_bundle_t* tmp = (spx_domain_id_bundle_t*)malloc(sizeof(spx_domain_id_bundle_t));
        copybbxfromBBX(&(tmp->m_domain),elem_temp->m_spatial_key);
        tmp->m_associated_id=elem_temp->m_mem_id;
        //linked it into the respond
        obj_list_entry_ptr entry_value = (obj_list_entry_ptr)malloc(sizeof(struct obj_list_entry));

        entry_value->next = NULL;
        entry_value->value.size = sizeof(spx_domain_id_bundle_t);
        entry_value->value.raw_obj = (char *)tmp;

        if (ifFirst==1)
        {
            query_out.spx_spatial_id_list=entry_value;
            prev=entry_value;
            ifFirst=0;
            continue;
        }

        prev->next = entry_value;
        prev = entry_value;
    }

    query_out.status = 0;

    ret = margo_respond(h, &query_out);
    assert(ret == HG_SUCCESS);

    ret = margo_free_input(h, &query_in);
    assert(ret == HG_SUCCESS);

    fflush(stdout);
    return;
}

DEFINE_MARGO_RPC_HANDLER(spx_query_ult)
DEFINE_MARGO_RPC_HANDLER(spx_register_ult)
DEFINE_MARGO_RPC_HANDLER(spx_update_ult)