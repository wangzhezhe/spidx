#ifndef SPIDX_CLIENT_H
#define SPIDX_CLIENT_H

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <mercury.h>
#include <abt.h>
#include <margo.h>
#include "types.h"
#include "spx_common.h"
#include "spx_error.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct spx_client* spx_client_t;
#define SPX_CLIENT_NULL ((spx_client_t)NULL)
typedef struct spx_provider_handle *spx_provider_handle_t;
#define SPX_PROVIDER_HANDLE_NULL ((spx_provider_handle_t)NULL)

//init the mochi service and expose the spidx RPC
/**
* @brief Creates a SPIDX client.
*
* @param[in] mid Margo instance
* @param[out] client spidx client
*
* @return SPIDX_SUCCESS or error code defined in spidx_error.h
*/
int spx_client_init(margo_instance_id mid, spidx_client_t* client);

/**
 * @brief Finalizes a SPIDX client.
 *
 * @param[in] client SPIDX client to finalize
 *
 * @return SPIDX_SUCCESS or error code defined in spidx_error.h
 */
int spx_client_finalize(spx_client_t client);


/**
 * @brief Creates a SPIDX provider handle.
 *
 * @param[in] client SPIDX client responsible for the provider handle
 * @param[in] addr Mercury address of the provider
 * @param[in] provider_id id of the provider
 * @param[in] handle provider handle
 *
 * @return SPIDX_SUCCESS or error code defined in spidx_error.h
 */
int spx_provider_handle_create(
        spx_client_t client,
        hg_addr_t addr,
        uint16_t provider_id,
        spx_provider_handle_t* handle);

/**
 * @brief Increments the reference counter of a provider handle.
 *
 * @param handle provider handle
 *
 * @return ALPHA_SUCCESS or error code defined in spidx_error.h
 */
int spx_provider_handle_ref_incr(
        spx_provider_handle_t handle);


/**
 * @brief Releases the provider handle. This will decrement the
 * reference counter, and free the provider handle if the reference
 * counter reaches 0.
 *
 * @param[in] handle provider handle to release.
 *
 * @return SPIDX_SUCCESS or error code defined in spidx_error.h
 */
int spx_provider_handle_release(spx_provider_handle_t handle);


/**
 * @brief Update the hash domain, this hash domain is labeld by the spx_index_key
 *
 * @param[in] handle provide handle.
 * @param[in] spx_index_key contains the spx_index_nons_key (a generalized structure supporting multiple types of the non-spatial key)
 * and the spx_index_spatial (the spatial information that contains the lower bound and the upper bound)
 * the spidx service will forward the update request to suitable service based on DHT and SFC
 * @param[in] is the id associated with the spx_index_key 
 *
 * @return SPIDX_SUCCESS or error code defined in spidx_error.h
 */
int spx_client_update(
    alpha_provider_handle_t handle,
    spx_index_key_t spx_index_key,
    char associated_id;
);


/**
 * @brief query the id related with the hash domain
 *
 * @param[in] spx_index_spatial is spatial information that containes the lower bound and the upper bound
 * the spidx service will forward the update request to suitable service based on DHT and SFC
 * @param[out] list_of_spatial_id_bundle is a list, every element in this list is a boundle 
 * that contains spatial information (bounding box) and an key, this key is setted by spx_update
 * 
 * @return SPIDX_SUCCESS or error code defined in spidx_error.h
 */
int spx_client_query(
    alpha_provider_handle_t handle,
    spx_index_spatial_t spx_index_spatial, 
    spx_partition_id_bundle_t* list_of_spatial_id_bundle);

#endif