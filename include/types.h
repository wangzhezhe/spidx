
#ifndef SPIDX_TYPE_H
#define SPIDX_TYPE_H

#include <mercury.h>
#include <mercury_macros.h>
//#include <mercury_proc_string.h>

//refer to https://readthedocs.org/projects/mochi/downloads/pdf/latest/
//the size is the memory space for any object
typedef struct{
        size_t size;
        char *raw_obj;
}obj_t;

static inline hg_return_t hg_proc_obj_t(hg_proc_t proc, void *arg)
{
  hg_return_t ret;
  obj_t *in = (obj_t*)arg;
  ret = hg_proc_hg_size_t(proc, &in->size);
  if(ret != HG_SUCCESS) return ret;
  if (in->size) {
    switch (hg_proc_get_op(proc)) {
    case HG_ENCODE:
        ret = hg_proc_raw(proc, in->raw_obj, in->size);
        if(ret != HG_SUCCESS) return ret;
      break;
    case HG_DECODE:
      in->raw_obj = (char*)malloc(in->size);
      ret = hg_proc_raw(proc, in->raw_obj, in->size);
      if(ret != HG_SUCCESS) return ret;
      break;
    case HG_FREE:
      free(in->raw_obj);
      break;
    default:
      break;
    }
  }
  return HG_SUCCESS;
}


typedef struct obj_list {
    obj_t          value;
    struct obj_list* next;
} *obj_list_t;

static inline hg_return_t hg_proc_obj_list_t(hg_proc_t proc, void* data)
{
    hg_return_t ret;
    obj_list_t* list = (obj_list_t*)data;

    hg_size_t length = 0;
    obj_list_t tmp   = NULL;
    obj_list_t prev  = NULL;

    switch(hg_proc_get_op(proc)) {

        case HG_ENCODE:
            tmp = *list;
            // find out the length of the list
            while(tmp != NULL) {
                tmp = tmp->next;
                length += 1;
            }
            // write the length
            ret = hg_proc_hg_size_t(proc, &length);
            if(ret != HG_SUCCESS)
                break;
            // write the list
            tmp = *list;
            while(tmp != NULL) {
                ret = hg_proc_obj_t(proc, &tmp->value);
                if(ret != HG_SUCCESS)
                    break;
                tmp = tmp->next;
            }
            break;

        case HG_DECODE:
            // find out the length of the list
            ret = hg_proc_hg_size_t(proc, &length);
            if(ret != HG_SUCCESS)
                break;
            // loop and create list elements
            *list = NULL;
            while(length > 0) {
                tmp = (obj_list_t)calloc(1, sizeof(*tmp));
                if(*list == NULL) {
                    *list = tmp;
                }
                if(prev != NULL) {
                    prev->next = tmp;
                }
                ret = hg_proc_obj_t(proc, &tmp->value);
                if(ret != HG_SUCCESS)
                    break;
                prev = tmp;
                length -= 1;
            }
            break;

        case HG_FREE:
            tmp = *list;
            while(tmp != NULL) {
                prev = tmp;
                tmp  = prev->next;
                free(prev);
            }
            ret = HG_SUCCESS;
    }
    return ret;
}


MERCURY_GEN_PROC(spx_update_in_t,
        ((obj_t)(spx_key))
)


MERCURY_GEN_PROC(spx_update_out_t,
	((int64_t)(ret)))




MERCURY_GEN_PROC(spx_query_in,
        ((obj_t)(spx_key)) 
)

MERCURY_GEN_PROC(spx_query_out, 
((int32_t)(status))\
((obj_list_t)(encoded_spx_spatial_id_bundle))
)

#endif