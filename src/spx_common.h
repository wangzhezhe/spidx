
//the data structure shared by both client and server
#include SPIDX_COMMON_H
#define  SPIDX_COMMON_H

#define DEFAULT_MAX_DIM 9

struct BBX{
    uint64_t m_lb[DEFAULT_MAX_DIM];
    uint64_t m_ub[DEFAULT_MAX_DIM];
};

//one parition is bined with one id 
//this informaiton is basic unit updated by the update API
//and returned by query API
struct spx_partition_id_bundle{
    BBX m_partition;
    int m_associated_id;
};

//the non spatial part of the spx key
struct spx_index_nonspatial{
    char m_string[256];
};

//the spatial part of the spidx is represented by the bounding box
struct spx_index_spatial{
    BBX m_bbx;
};

//the struct for the spx key
struct spx_index_key{
    spx_index_nonspatial m_index_nonspatial;
    spx_index_spatial m_index_spatial;
};
#endif