
#include "spx_server_inner.h"
#include "spx_error.h"
#include <abt.h>

void test_register_domain()
{
    fprintf(stdout, "---test_register_domain---\n");
    bbx_t hash_domain;
    hash_domain.m_dims = 3;
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        hash_domain.m_lb[i] = 0;
        hash_domain.m_ub[i] = 512;
    }

    int status = spx_inner_register_domain(hash_domain);
    if (status != SPIDX_SUCCESS)
    {
        fprintf(stderr, "---error: failed to register hash domain---\n");
        exit(-1);
    }

    status = spx_inner_register_domain(hash_domain);
    if (status != SPIDX_INITIALIZED)
    {
        fprintf(stderr, "---error: failed to register hash domain (only init once)---\n");
        exit(-1);
    }

    return;
}

void test_inner_put()
{
    fprintf(stdout, "---test_inner_put---\n");
    //int spx_inner_put(char encoded_nons_key[BUFLEN], bbx_t put_domain, int mem_id);
    char encoded_nons_key[BUFLEN] = "TESTKEY";
    bbx_t hash_domain;
    hash_domain.m_dims = 3;
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        hash_domain.m_lb[i] = 0;
        hash_domain.m_ub[i] = 64;
    }

    int status = spx_inner_put(encoded_nons_key, hash_domain, 1234);

    if (status != SPIDX_SUCCESS)
    {
        fprintf(stderr, "---error: failed to put hash domain 1---\n");
        exit(-1);
    }

    for (i = 0; i < 3; i++)
    {
        hash_domain.m_lb[i] = 128;
        hash_domain.m_ub[i] = 256;
    }

    status = spx_inner_put(encoded_nons_key, hash_domain, 5678);

    if (status != SPIDX_SUCCESS)
    {
        fprintf(stderr, "---error: failed to put hash domain 2---\n");
        exit(-1);
    }

    for (i = 0; i < 3; i++)
    {
        hash_domain.m_lb[i] = 68;
        hash_domain.m_ub[i] = 1024;
    }

    status = spx_inner_put(encoded_nons_key, hash_domain, 1234);

    if (status != SPIDX_OUTOFBOUND)
    {
        fprintf(stderr, "---error: failed to put hash domain test, out of bound---\n");
        exit(-1);
    }

    return;
}

void test_inner_query()
{
    fprintf(stdout, "---test_inner_query---\n");

    char encoded_nons_key[BUFLEN] = "TESTKEY";
    bbx_t query_domain;
    query_domain.m_dims = 3;
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        query_domain.m_lb[i] = 0;
        query_domain.m_ub[i] = 511;
    }
    //query the value by the spacific domain
    domain_id_boundle_t *domain_id_list = spx_inner_query(encoded_nons_key, query_domain);
    domain_id_boundle_t *elem_temp = NULL;
    LL_FOREACH(domain_id_list, elem_temp)
    {
        //check the element in the list
        fprintf(stdout, "spatial info:\n");
        printBBXinfo(elem_temp->m_spatial_key);
        fprintf(stdout, "mem_id %d\n", elem_temp->m_mem_id);
    }
    return;
}

int main()
{
    test_register_domain();

    test_inner_put();

    test_inner_query();

    return 0;
}