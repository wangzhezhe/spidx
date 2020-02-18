//send the request to spidx api to see if it handle things correactly
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <margo.h>
#include "../src/types.h"

int main(int argc, char** argv)
{

    if(argc != 2) {
        fprintf(stderr,"Usage: %s <server address>\n", argv[0]);
        exit(0);
    }
    
    margo_instance_id mid;
    mid = margo_init("tcp", MARGO_CLIENT_MODE, 0, 0);
    if(mid == MARGO_INSTANCE_NULL)
    {
        fprintf(stderr, "Error: margo_init()\n");
        return(-1);
    }


    //create instance of rpc input
    spx_update_in update_in_arg;
    update_in_arg.encoded_spx_nonspatial_key="test-encoded-spx-nonspatial-key";
    update_in_arg.encoded_spx_spatial_key="test-encoded-spx-spatial-key";
    update_in_arg.associated_id=12345;

    //create instance of rpc input
    spx_query_in query_in_arg;
    query_in_arg.encoded_spx_nonspatial_key="test-encoded-spx-nonspatial-key";
    query_in_arg.encoded_spx_spatial_key="test-encoded-spx-spatial-key";


    hg_addr_t svr_addr;
    int ret;
    ret = margo_addr_lookup(mid, argv[1], &svr_addr);
    assert(ret == HG_SUCCESS);
    fprintf(stdout,"margo_addr_lookup ok\n");


    hg_id_t update_rpc_id = MARGO_REGISTER(mid, "spx_update", spx_update_in, spx_update_out, NULL);
    hg_id_t query_rpc_id = MARGO_REGISTER(mid, "spx_query", spx_query_in, spx_query_out, NULL);
    

    //update handle
    hg_handle_t handle_u;
    ret = margo_create(mid, svr_addr, update_rpc_id, &handle_u);

    assert(ret == HG_SUCCESS);
    fprintf(stdout,"handle_u margo_create ok\n");

    ret = margo_forward(handle_u, &update_in_arg);
    assert(ret == HG_SUCCESS);
    fprintf(stdout,"handle_u margo_forward ok\n");

    spx_update_out update_resp;
    ret=margo_get_output(handle_u, &update_resp);
    assert(ret == HG_SUCCESS);
    fprintf(stdout,"handle_u margo_get_output ok\n");
    fprintf(stdout,"get the respond of the update %d\n",update_resp.status);
    ret=margo_free_output(handle_u,&update_resp);
    assert(ret == HG_SUCCESS);
    fprintf(stdout,"handle_u margo_free_output handle ok\n");
    margo_destroy(handle_u);


    //query handle
    hg_handle_t handle_q;
    ret = margo_create(mid, svr_addr, query_rpc_id, &handle_q);
    assert(ret == HG_SUCCESS);
    printf("margo_create handle_q ok\n");
    
    ret = margo_forward(handle_q, &query_in_arg);
    assert(ret == HG_SUCCESS);
    printf("margo_forward handle_q ok\n");

    spx_query_out query_resp;
    margo_get_output(handle_q, &query_resp);
    assert(ret == HG_SUCCESS);
    fprintf(stdout,"margo_get_output handle_q ok\n");
    fprintf(stdout,"get the respond of the query: status %d associated spatial and id bundle (%s)\n",query_resp.status, query_resp.encoded_spx_spatial_id_bundle);
    ret=margo_free_output(handle_q,&query_resp);
    assert(ret == HG_SUCCESS);
    fprintf(stdout,"margo_free_output handle_q ok\n");


    ret = margo_destroy(handle_q);
    assert(ret == HG_SUCCESS);
    fprintf(stdout,"margo_destroy handle_q ok\n");
    assert(ret == HG_SUCCESS);
    fprintf(stdout,"handle_q margo_free_output handle ok\n");


    ret = margo_addr_free(mid, svr_addr);
    margo_finalize(mid);

    return 0;
}