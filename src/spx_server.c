#include "spx_server.h"

static void spx_handle_update(hg_handle_t h);
DECLARE_MARGO_RPC_HANDLER(spx_handle_update)

static void spx_handle_query(hg_handle_t h);
DECLARE_MARGO_RPC_HANDLER(spx_handle_query)

static void spx_handle_update(hg_handle_t h)
{
    spx_update_in in;
    spx_update_out out;
    //TODO
}

static void spx_handle_query(hg_handle_t h)
{
    spx_query_in in;
    spx_query_out out;
    //TODO
}

//margo_init operation is executed by the service that call spidx
int spx_init(margo_instance_id mid,
uint32_t global_dim, uint64_t* global_lb, uint64_t* global_ub)
{
    /* register RPC */
    MARGO_REGISTER(mid, "spx_update", spx_update_in, spx_update_out, spx_handle_update);
    MARGO_REGISTER(mid, "spx_query", spx_query_in, spx_query_out, spx_handle_query);
    
    //TODO init global domain
    return 0;
}

//finalize the spidx service
int spx_finalize(margo_instance_id mid)
{
    margo_wait_for_finalize(mid);
    return 0;
}

DEFINE_MARGO_RPC_HANDLER(spx_handle_update)
DEFINE_MARGO_RPC_HANDLER(spx_handle_query)