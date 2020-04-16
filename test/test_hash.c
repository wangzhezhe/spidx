
#include <spx_hash.h>

void test_register_domain()
{

    char *name1 = "domain1";
    bbx_t bbx;
    bbx.m_dims=3;
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        bbx.m_lb[i] = 10;
        bbx.m_ub[i] = 20;
    }

    int rvalue = spx_hash_global_domain_register(name1, &bbx);
    if (rvalue != 0)
    {
        fprintf(stderr, "---error: failed to test gloabl hash domain case0---\n");
        exit(-1);
    }

    rvalue = spx_hash_global_domain_register(name1, &bbx);
    if (rvalue != -1)
    {
        fprintf(stderr, "---error: failed to test gloabl hash domain case1---\n");
        exit(-1);
    }

    char *name2 = "domain2";
    rvalue = spx_hash_global_domain_register(name2, &bbx);
    if (rvalue != 0)
    {
        fprintf(stderr, "---error: failed to test gloabl hash domain case0---\n");
        exit(-1);
    }

    spx_hash_delete_global_domain_map();
}

void test_retrieve_domain()
{
    char *name1 = "domain1";
    bbx_t bbx;
    bbx.m_dims = 3;
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        bbx.m_lb[i] = 10;
        bbx.m_ub[i] = 20;
    }
    int rvalue = spx_hash_global_domain_register(name1, &bbx);

    //retrieve
    bbx_t *output_global_domain = malloc(sizeof(bbx_t));

    rvalue = spx_hash_get_global_domain(name1,output_global_domain);

    if (rvalue!=0)
    {
        fprintf(stderr, "---error: failed to test gloabl hash domain retrieve case0---\n");
        exit(-1);
    }

    //check results

    if (output_global_domain->m_dims != 3)
    {

        fprintf(stderr, "---error: failed to test gloabl hash domain retrieve results dim %d ---\n", output_global_domain->m_dims);
        exit(-1);
    }
    for (i = 0; i < 3; i++)
    {
        if (bbx.m_lb[i] != output_global_domain->m_lb[i] || bbx.m_ub[i] != output_global_domain->m_ub[i])
        {
            fprintf(stderr, "---error: failed to test gloabl hash domain retrieve value---\n");
            exit(-1);
        }
    }

    char *name2 = "domain2";
        rvalue = spx_hash_get_global_domain(name2,output_global_domain);


    if (rvalue != -1)
    {
        fprintf(stderr, "---error: failed to test gloabl hash domain retrieve case1---\n");
        exit(-1);
    }
    
   free(output_global_domain);
}

int main()
{
    test_register_domain();

    test_retrieve_domain();
}