
//the data structure shared by both client and server
#ifndef SPIDX_COMMON_H
#define SPIDX_COMMON_H

#include <stdint.h>

#define DEFAULT_MAX_DIM 9

typedef enum {
    SPX_ELEM_INT,
    SPX_ELEM_STR,
} spx_elem_t;

//the general structure to represent the key in different type
typedef struct spx_index_entry{
    spx_elem_t type;
    void *value;
}spx_index_entry;

typedef struct bbx_t{
    uint16_t m_dims;
    uint64_t m_lb[DEFAULT_MAX_DIM];
    uint64_t m_ub[DEFAULT_MAX_DIM];
}bbx_t;

typedef struct spx_index_nonspatial_t{
    spx_index_entry m_entry;
}spx_index_nonspatial_t;

//the spatial part of the spidx is represented by the bounding box
typedef struct spx_index_spatial_t{
    bbx_t m_bbx;
}spx_index_spatial_t;

//one parition is bined with one id 
//this informaiton is basic unit updated by the update API
//and returned by query API
typedef struct spx_partition_id_bundle_t{
    bbx_t m_partition;
    int m_associated_id;
}spx_partition_id_bundle_t;

//the struct for the spx key
typedef struct spx_index_key_t{
    spx_index_nonspatial_t m_index_nonspatial;
    spx_index_spatial_t m_index_spatial;
}spx_index_key_t;

#endif