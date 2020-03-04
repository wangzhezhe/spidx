#ifndef __SPIDX_SERVER_INNER_H
#define __SPIDX_SERVER_INNER_H

#ifdef __cplusplus
extern "C" {
#endif


#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */


#include "spx_common.h"

#include "../src/utils/bbxtool.h"
#include "../src/utils/uthash.h"
#include "../src/utils/utlist.h"



typedef struct domain_id_boundle_t{
    BBX* m_spatial_key;
    //this is the mem_id inserted by the user
    //char m_mem_id[BUFLEN];
    int m_mem_id;
    struct domain_id_boundle_t *next, *prev;
}domain_id_boundle_t;

typedef struct key_id_boundle_t{
    char m_nons_key[BUFLEN];
    domain_id_boundle_t* m_domain_id_list;
    UT_hash_handle hh;         /* makes this structure hashable */
}key_id_boundle_t;

typedef struct spx_server_inner_t{ 
    //The map that store the spx_key and the associated id
    key_id_boundle_t *m_key_id_map;
    //the partition associated with the current server based on SFC
    BBX* spx_hash_domain;
}spx_server_inner_t;

extern spx_server_inner_t * spx_server_inner;

//register the hash domain associated with current server
int spx_inner_register_domain(bbx_t hash_domain);

//put the associated value into the domain
int spx_inner_put(char encoded_nons_key[BUFLEN], bbx_t put_domain, int mem_id);

//query the associated value by the spx key
//return a linked list, every element is a domain_id bundle
domain_id_boundle_t* spx_inner_query(char encoded_nons_key[BUFLEN], bbx_t query_domain);

#ifdef __cplusplus
}
#endif

#endif