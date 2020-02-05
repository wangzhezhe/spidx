
#ifndef SPIDX_TYPE_H
#define SPIDX_TYPE_H

#include <mercury.h>
#include <mercury_macros.h>
#include <mercury_proc_string.h>


MERCURY_GEN_PROC(spx_update_in,
        ((hg_string_t)(variable_name))\
        ((uint32_t)(version))\
        ((hg_string_t)(encoded_spx_spatial_key))\
        ((uint32_t)(associated_id))
)

MERCURY_GEN_PROC(spx_update_out, 
((int32_t)(status))
)

MERCURY_GEN_PROC(spx_query_in,
        ((hg_string_t)(variable_name))\
        ((uint32_t)(version))\
        ((hg_string_t)(encoded_spx_spatial_key))  
)

MERCURY_GEN_PROC(spx_query_out, 
((int32_t)(status))\
((hg_string_t)(encoded_spx_spatial_id_bundle))
)

#endif