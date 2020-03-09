#include <assert.h>
#include <stdio.h>
#include <margo.h>
#include <spx_server.h>

margo_instance_id gloabl_mid;

int main(int argc, char **argv)
{
    margo_instance_id gloabl_mid = margo_init("na+sm", MARGO_SERVER_MODE, 0, 0);
    assert(gloabl_mid);

    hg_addr_t my_address;
    margo_addr_self(gloabl_mid, &my_address);
    char addr_str[128];
    size_t addr_str_size = 128;
    margo_addr_to_string(gloabl_mid, addr_str, &addr_str_size, my_address);
    margo_addr_free(gloabl_mid, my_address);

    int a = sizeof(size_t);
    int b = sizeof(hg_size_t);
    fprintf(stdout, "%d %d\n", a, b);
    fprintf(stdout, "test server\n");
    fprintf(stdout, "Server running at address %s\n", addr_str);
    fflush(stdout);
    spx_provider_t* spx_prov = NULL;
    spx_provider_register(gloabl_mid, spx_prov);
    margo_wait_for_finalize(gloabl_mid);

    return 0;
}