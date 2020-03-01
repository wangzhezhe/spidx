#include "spx_server.h"
#include <mpi.h>
#include <ssg.h>
#include <ssg-mpi.h>

//for testing
DECLARE_MARGO_RPC_HANDLER(hello_world)

//margo_init operation is executed by the service that call spidx
int spx_server_init(margo_instance_id mid,
uint32_t global_dim, uint64_t* global_lb, uint64_t* global_ub)
{
    /* register RPC */
    MARGO_REGISTER(mid, "spx_update", spx_update_in, spx_update_out, spx_handle_update);
    MARGO_REGISTER(mid, "spx_query", spx_query_in, spx_query_out, spx_handle_query);
    hg_id_t rpc_id = MARGO_REGISTER(mid, "hello", void, void, hello_world);
    margo_registered_disable_response(mid, rpc_id, HG_TRUE);
    
    /**
     * SSG group creation and state query
    
    ssg_group_id_t gid = ssg_group_create_mpi(mid, "spidx_group", MPI_COMM_WORLD, NULL, NULL, NULL);
    assert(gid != SSG_GROUP_ID_INVALID);

    int self_rank = ssg_get_group_self_rank(gid);
    int group_size = ssg_get_group_size(gid);

    fprintf(stdout, "self rank is %d, group size is %d\n",self_rank,group_size);
    **/
    //TODO init the global domain and init the DHT (global domain and the number of the server)

    return 0;
}

//finalize the spidx service
int spx_server_finalize(margo_instance_id mid)
{
    margo_wait_for_finalize(mid);
    return 0;
}

static void spx_handle_update(hg_handle_t h)
{
    spx_update_in in;
    spx_update_out out;

    int ret = margo_get_input(h, &in);
    assert(ret == HG_SUCCESS);
    
   
    fprintf(stdout,"spidx handle the spx_handle_update\n");
    fprintf(stdout,"input nonspatial key (%s), spatial key (%s), id (%d)\n", 
    in.encoded_spx_nonspatial_key, in.encoded_spx_spatial_key, in.associated_id);

    //TODO
    out.status = 777;
    
    ret = margo_respond(h, &out);
    assert(ret == HG_SUCCESS);
}

static void spx_handle_query(hg_handle_t h)
{
    spx_query_in in;
    spx_query_out out;

    int ret = margo_get_input(h, &in);
    assert(ret == HG_SUCCESS);

    fprintf(stdout,"spidx handle the spx_handle_query\n");
    fprintf(stdout,"input encoded_spx_nonspatial_key (%s), encoded_spx_spatial_key (%s)\n", 
    in.encoded_spx_nonspatial_key, in.encoded_spx_spatial_key);

    //TODO
    out.status=888;
    out.encoded_spx_spatial_id_bundle="test-encoded-spatial-id-boundle";
    ret = margo_respond(h, &out);
    assert(ret == HG_SUCCESS);

}

static void hello_world(hg_handle_t h)
{
    hg_return_t ret;
    printf("Hello World!\n");
}


DEFINE_MARGO_RPC_HANDLER(hello_world)
DEFINE_MARGO_RPC_HANDLER(spx_handle_update)
DEFINE_MARGO_RPC_HANDLER(spx_handle_query)