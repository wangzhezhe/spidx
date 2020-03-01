#include <stdio.h>
#include <stdlib.h>
#include "spx_server_inner.h"
#include "spx_error.h"
#include "spx_common.h"
#include "../src/utils/bbxtool.h"
#include <abt.h>
#include <abt-io.h>

spx_server_inner_t *spx_server_inner = NULL;
static ABT_mutex spx_inner_mtx = ABT_MUTEX_NULL; // mutex for the hash map of the server inner

//register the hash domain associated with current server
int spx_inner_register_domain(bbx_t hash_domain)
{
    //check if the spx_server_inner is NULL
    if (spx_server_inner != NULL)
    {
        return SPIDX_INITIALIZED;
    }

    //else, init the spx_server_inner and init the hash domain
    spx_server_inner = (spx_server_inner_t *)malloc(sizeof(struct spx_server_inner_t));
    //init the hash domain
    spx_server_inner->spx_hash_domain = initBBX(hash_domain.m_dims, hash_domain.m_lb, hash_domain.m_ub);
    spx_server_inner->m_key_id_map = NULL; /* important! initialize hash map to NULL */

    //init the ABT mutex
    int ret = ABT_init(0, NULL); /* XXX: argc/argv not currently used by ABT ... */
    if (ret != 0)
    {
        fprintf(stderr, "failed to init the abt\n");
        return SPIDX_INITABT;
    }
    ret = ABT_mutex_create(&spx_inner_mtx);
    if (ret != 0)
    {
        fprintf(stderr, "failed to init the abt mutex\n");
        return SPIDX_INITABT;
    }

    return SPIDX_SUCCESS;
}

//put the associated value into the domain
int spx_inner_put(char encoded_nons_key[BUFLEN], bbx_t put_domain, int mem_id)
{

    if (spx_server_inner == NULL)
    {
        return SPIDX_NOTINITILIZED;
    }

    //check wether the put_domain is out of the bbx
    BBX *bbx = initBBX(put_domain.m_dims, put_domain.m_lb, put_domain.m_ub);
    int status = ifOutofBBX(spx_server_inner->spx_hash_domain, bbx);
    if (status == 0)
    {
        deleteBBX(bbx);
        return SPIDX_OUTOFBOUND;
    }

    //query the Map according to the encoded nons_key
    key_id_boundle_t *key_id_boundle = NULL;

    ABT_mutex_lock(spx_inner_mtx);
    HASH_FIND_STR(spx_server_inner->m_key_id_map, encoded_nons_key, key_id_boundle);
    ABT_mutex_unlock(spx_inner_mtx);

    //if not exist, init the list and add the datastructure into the map
    if (key_id_boundle == NULL)
    {
        key_id_boundle = (key_id_boundle_t *)malloc(sizeof(key_id_boundle_t));
        key_id_boundle->m_domain_id_list = NULL; /* important! initialize list to NULL */
        strncpy(key_id_boundle->m_nons_key, encoded_nons_key, BUFLEN);

        ABT_mutex_lock(spx_inner_mtx);
        HASH_ADD_STR(spx_server_inner->m_key_id_map, m_nons_key, key_id_boundle);
        ABT_mutex_unlock(spx_inner_mtx);
    }

    //generate the domain_boundle element and insert into the list
    domain_id_boundle_t *domain_id_boundle = (domain_id_boundle_t *)malloc(sizeof(domain_id_boundle_t));

    //assign the key and the id
    domain_id_boundle->m_spatial_key = bbx;
    domain_id_boundle->m_mem_id = mem_id;
    LL_PREPEND(key_id_boundle->m_domain_id_list, domain_id_boundle);

    return SPIDX_SUCCESS;
}

//query the associated value by the spx key
//return a linked list, every element is a domain_id bundle
domain_id_boundle_t *spx_inner_query(char encoded_nons_key[BUFLEN], bbx_t query_domain)
{
    //query the map
    key_id_boundle_t *key_id_boundle = NULL;

    ABT_mutex_lock(spx_inner_mtx);
    HASH_FIND_STR(spx_server_inner->m_key_id_map, encoded_nons_key, key_id_boundle);
    ABT_mutex_unlock(spx_inner_mtx);
    //if not exist, return NULL
    if (key_id_boundle == NULL)
    {
        return NULL;
    }

    //if exist, return the header position of the list
    domain_id_boundle_t *domain_header = key_id_boundle->m_domain_id_list;
    domain_id_boundle_t *elem_temp = NULL;

    //generate the list for output
    domain_id_boundle_t *output_boundle_id = NULL;

    //go through the list, compare elem with the query domain, get the overlapped region
    LL_FOREACH(domain_header, elem_temp)
    {
        //allocate space for the element in list
        domain_id_boundle_t *domain_id_boundle = (domain_id_boundle_t *)malloc(sizeof(domain_id_boundle_t));

        //if there is overlapping compared with the query domain
        BBX *query_bbx = initBBX(query_domain.m_dims, query_domain.m_lb, query_domain.m_ub);
        BBX *overlap_bbx = getOverlapBBX(elem_temp->m_spatial_key, query_bbx);

        if (overlap_bbx != NULL)
        {
            domain_id_boundle->m_mem_id = elem_temp->m_mem_id;
            domain_id_boundle->m_spatial_key = overlap_bbx;
            LL_PREPEND(output_boundle_id, domain_id_boundle);
        }

        deleteBBX(query_bbx);
    }

    return output_boundle_id;
}

//TODO finalize the server inner, free the data and finalize the abt io, 