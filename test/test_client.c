#include <stdio.h>
#include <margo.h>
#include <spx_client.h>

//register the hash domain
void test_register(spx_provider_handle_t spx_ph)
{
    fprintf(stdout, "---test_register---\n");
    //generate the bbx
    bbx_t *bbx = (bbx_t *)malloc(sizeof(bbx_t));
    bbx->m_dims = 3;
    int i;
    for (i = 0; i < 3; i++)
    {
        bbx->m_lb[i] = 0;
        bbx->m_ub[i] = 512;
    }

    int result = spx_client_register(spx_ph, bbx);
    fprintf(stdout, "result of test_register %d\n", result);
    free(bbx);
    if (result != 0)
    {
        fprintf(stdout, "---failed for test_register---");
        exit(-1);
    }
    return;
}

//update the spx key and associated id
void test_update(spx_provider_handle_t spx_ph)
{
    fprintf(stdout, "---test_update---\n");
    //generate entry
    char *str = "teststr123";
    spx_nonskey_entry *entry_str = (spx_nonskey_entry *)malloc(sizeof(spx_nonskey_entry));
    entry_str->type = SPX_ELEM_STR;
    entry_str->value = (void *)str;

    //generate the bbx
    bbx_t *bbx = (bbx_t *)malloc(sizeof(bbx_t));
    bbx->m_dims = 3;
    int i;
    //init array
    for (i = 0; i < DEFAULT_MAX_DIM; i++)
    {

        bbx->m_lb[i] = 0;
        bbx->m_ub[i] = 0;
    }

    for (i = 0; i < 3; i++)
    {

        bbx->m_lb[i] = 0;
        bbx->m_ub[i] = 32;
    }

    HASHMETHOD hash_method = SFCGLOBAL;
    int result = spx_client_update(spx_ph, entry_str, bbx, (uint32_t)123, hash_method);
    fprintf(stdout, "result of test_update %d\n", result);


    for (i = 0; i < 3; i++)
    {

        bbx->m_lb[i] = 64;
        bbx->m_ub[i] = 128;
    }

    result = spx_client_update(spx_ph, entry_str, bbx, (uint32_t)456, hash_method);
    fprintf(stdout, "result of test_update %d\n", result);

    free(entry_str);
    free(bbx);

    if (result != 0)
    {
        fprintf(stdout, "---failed for test_update---\n");
        exit(-1);
    }
    return;
}

//query the associated id based on the spx key
void test_query(spx_provider_handle_t spx_ph)
{
    fprintf(stdout, "---test_query---\n");
    //generate entry
    char *str = "teststr123";
    spx_nonskey_entry *entry_str = (spx_nonskey_entry *)malloc(sizeof(spx_nonskey_entry));
    entry_str->type = SPX_ELEM_STR;
    entry_str->value = (void *)str;

    //generate the bbx
    bbx_t *bbx = (bbx_t *)malloc(sizeof(bbx_t));
    bbx->m_dims = 3;
    int i;
    //init array
    for (i = 0; i < DEFAULT_MAX_DIM; i++)
    {

        bbx->m_lb[i] = 0;
        bbx->m_ub[i] = 0;
    }

    for (i = 0; i < 3; i++)
    {

        bbx->m_lb[i] = 0;
        bbx->m_ub[i] = 128;
    }

    //the returned list
    spx_domain_id_entry *return_boundle_list = NULL;

    int result = spx_client_query(spx_ph, entry_str, bbx, &return_boundle_list);

    fprintf(stdout, "result of test_query %d\n", result);

    //check the results of the return_boundle_list
    if (result == 0)
    {
        //print the results
        fprintf(stdout, "check the return resutls\n");
        while (return_boundle_list != NULL)
        {
            fprintf(stdout, "id is %d\n", return_boundle_list->boundle_value->m_associated_id);
            printbbx(&(return_boundle_list->boundle_value->m_domain));
            return_boundle_list=return_boundle_list->next;
        }
    }
    fflush(stdout);
    return;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <server address>\n", argv[0]);
        exit(0);
    }

    const char *svr_addr_str = argv[1];

    margo_instance_id mid = margo_init("tcp", MARGO_CLIENT_MODE, 0, 0);

    hg_addr_t svr_addr;
    margo_addr_lookup(mid, svr_addr_str, &svr_addr);

    spx_client_t spx_clt;
    spx_provider_handle_t spx_ph;

    spx_client_init(mid, &spx_clt);

    spx_provider_handle_create(spx_clt, svr_addr, &spx_ph);

    test_register(spx_ph);

    test_update(spx_ph);

    test_query(spx_ph);

    spx_provider_handle_release(spx_ph);

    spx_client_finalize(spx_clt);

    margo_addr_free(mid, svr_addr);

    margo_finalize(mid);

    return 0;
}