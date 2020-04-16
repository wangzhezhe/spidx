#include "../src/utils/uthash.h"
#include <spx_hash.h>

//init the global domain
global_domain_map_t *g_domain_map = NULL;

//register the global domain into the hashtable
int spx_hash_global_domain_register(
    char *name_global_domain,
    bbx_t *global_domain)
{

    global_domain_map_t *temp = NULL;

    HASH_FIND_STR(g_domain_map, name_global_domain, temp);

    //if the name exist, return -1
    if (temp != NULL)
    {
        fprintf(stderr, "---the name_global_domain (%s) exist---\n", name_global_domain);
        return -1;
    }

    global_domain_map_t *new_domain = malloc(sizeof(global_domain_map_t));
    strcpy(new_domain->name_global_domain, name_global_domain);
    new_domain->global_domain.m_dims = global_domain->m_dims;
    int i;
    for (i = 0; i < global_domain->m_dims; i++)
    {
        new_domain->global_domain.m_lb[i] = global_domain->m_lb[i];
        new_domain->global_domain.m_ub[i] = global_domain->m_ub[i];
    }

    HASH_ADD_STR(g_domain_map, name_global_domain, new_domain);

    return 0;
}

int spx_hash_get_global_domain(char *name, bbx_t *bbx)
{

    global_domain_map_t *retieved_domain = NULL;
    HASH_FIND_STR(g_domain_map, name, retieved_domain);
    if (retieved_domain == NULL)
    {
        fprintf(stdout, "---the global_domain with name (%s) not exist---\n", name);
        return -1;
    }

    //copy the indexed info
    bbx->m_dims = retieved_domain->global_domain.m_dims;
    int i;
    for (i = 0; i < bbx->m_dims; i++)
    {
        bbx->m_lb[i] = retieved_domain->global_domain.m_lb[i];
        bbx->m_ub[i] = retieved_domain->global_domain.m_ub[i];
    }

    return 0;
}

int spx_hash_delete_global_domain_map()
{
    struct global_domain_map_t *current, *tmp;

    HASH_ITER(hh, g_domain_map, current, tmp)
    {
        HASH_DEL(g_domain_map, current); /* delete; users advances to next */
        free(current);            /* optional- if you want to free  */
    }
    return 0;
}