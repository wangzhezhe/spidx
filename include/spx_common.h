
//the data structure shared by both client and server
#ifndef SPIDX_COMMON_H
#define SPIDX_COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <margo.h>

#define DEFAULT_MAX_DIM 9
#define BUFLEN 256

typedef struct spx_provider
{
    margo_instance_id mid;
    hg_id_t update_id;
    hg_id_t query_id;
    /* other provider-specific data */
}spx_provider_t;

typedef enum {
    SFCGLOBAL,
    VERSION,
    VARNAME,
    RBGLOBAL, //recursive bisection of global dimensions
    INTERLEAVING
}HASHMETHOD;

typedef enum
{
    SPX_ELEM_INT,
    SPX_ELEM_STR,
    SPX_ELEM_INTSTR,
} spx_elem_t;

//the general structure to represent the key in different type
typedef struct spx_nonskey_entry
{
    spx_elem_t type;
    void *value;
} spx_nonskey_entry;

typedef struct bbx_t
{
    uint16_t m_dims;
    uint64_t m_lb[DEFAULT_MAX_DIM];
    uint64_t m_ub[DEFAULT_MAX_DIM];
} bbx_t;

static void copybbx(bbx_t *destbbx, bbx_t *srcbbx)
{
    destbbx->m_dims = srcbbx->m_dims;
    int i;
    for (i = 0; i < DEFAULT_MAX_DIM; i++)
    {
        destbbx->m_lb[i] = srcbbx->m_lb[i];
        destbbx->m_ub[i] = srcbbx->m_ub[i];
    }
    return;
}
static void printbbx(bbx_t *bbx)
{
    fprintf(stdout, "dims %d\n", bbx->m_dims);
    int i;
    fprintf(stdout, "m_lb\n");

    for (i = 0; i < bbx->m_dims; i++)
    {
        fprintf(stdout, "%d,", bbx->m_lb[i]);
    }

    fprintf(stdout, "\nm_ub\n");

    for (i = 0; i < bbx->m_dims; i++)
    {
        fprintf(stdout, "%d,", bbx->m_ub[i]);
    }

    fprintf(stdout, "\n");
    return;
}

//the struct for the spx key
typedef struct spx_index_key_t
{
    char m_index_nonspatial[BUFLEN];
    bbx_t m_index_spatial;
} spx_index_key_t;

//one parition is bined with one id
//this informaiton is basic unit updated by the update API
//and returned by query API
typedef struct spx_domain_id_bundle_t
{
    bbx_t m_domain;
    int m_associated_id;
} spx_domain_id_bundle_t;

static void enocde_nonspatial_key(spx_nonskey_entry *nonskey_entry, char* encoded_buffer)
{
    char buffer[BUFLEN];
    if (nonskey_entry->type == SPX_ELEM_INT)
    {
        sprintf(buffer, "%d", *(int *)nonskey_entry->value);
    }
    else if (nonskey_entry->type == SPX_ELEM_STR)
    {
        sprintf(buffer, "%s", (char *)nonskey_entry->value);
    }
    else if (nonskey_entry->type == SPX_ELEM_INTSTR)
    {
        sprintf(buffer, "%s", (char *)nonskey_entry->value);
    }
    else
    {
        fprintf(stderr, "unsuported type %d\n", nonskey_entry->type);
    }
    strcpy(encoded_buffer, buffer);
    return;
}

static void encode_spatial_key(bbx_t *bbx, char encoded_buffer[BUFLEN])
{
    //size of bbx is 152 on 64 bit CPU
    memcpy(encoded_buffer, (char *)bbx, sizeof(bbx_t));
    return;
}

static void decode_domain_id_bundle(char *encoded_domain_id_bundle)
{
    //return an array of listed boundle
    //the first 16 bit is the length of the array
    int headerLen = 4 + 4;
    char arrayLen[headerLen];
    memcpy(arrayLen, encoded_domain_id_bundle, headerLen);
    int len = atoi(arrayLen);

    fprintf(stdout, "domain_id_bundle array length:%d\n", len);

    spx_domain_id_bundle_t *temp_elem = (spx_domain_id_bundle_t *)((char *)encoded_domain_id_bundle + headerLen);
    int i;
    for (i = 0; i < len; i++)
    {
        //print domain
        printbbx(&temp_elem->m_domain);
        //print id
        fprintf(stdout, "associated id :%d\n", temp_elem->m_associated_id);
        temp_elem++;
    }

    return;
}

#endif