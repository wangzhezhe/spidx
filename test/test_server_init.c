
//test if the server can be started normally
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <mercury.h>
#include <abt.h>
#include <margo.h>
#include "../src/spx_server.h"
#include "mpi.h"
#include <ssg.h>
#include <ssg-mpi.h>

int main(int argc, char **argv){


    MPI_Init(&argc, &argv);
    //create the margo instance
    margo_instance_id mid;
    mid = margo_init("tcp", MARGO_SERVER_MODE, 0, -1);
    if(mid == MARGO_INSTANCE_NULL)
    {
        fprintf(stderr, "Error: margo_init()\n");
        return(-1);
    }

    //call the spx init function
    uint64_t global_lb [9] ={0,0,0};
    uint64_t global_ub [9] ={9,9,9};
    uint32_t global_dim = 3;
    //assume MPI_init is called before ssg_init
    ssg_init();
    int status = spx_server_init(mid, 3 , global_lb, global_ub);
    if(status!=0){
        fprintf(stderr, "failed to init spx server\n");
        return(-1);
    }

    printf("ok to init the spidx server and start listen to the request\n");

    //print the address
    hg_addr_t my_address;
    margo_addr_self(mid, &my_address);
    char addr_str[128];
    size_t addr_str_size = 128;
    margo_addr_to_string(mid, addr_str, &addr_str_size, my_address);
    margo_addr_free(mid,my_address);
    printf("Server running at address %s\n", addr_str);


    //margo_wait_for_finalize is supposed to be called by another function
    spx_server_finalize(mid);

    MPI_Finalize();

}