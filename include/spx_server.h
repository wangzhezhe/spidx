#ifndef __SPIDX_SERVER_H
#define __SPIDX_SERVER_H

#include <margo.h>
#include <spx_common.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPIDX_ABT_POOL_DEFAULT ABT_POOL_NULL

typedef struct spx_provider* spx_provider_t;
#define SPIDX_PROVIDER_NULL ((spx_provider_t)NULL)
#define SPIDX_PROVIDER_IGNORE ((spx_provider_t*)NULL)

/**
 * @brief Creates a new SPIDX provider. If SPIDX_PROVIDER_IGNORE
 * is passed as last argument, the provider will be automatically
 * destroyed when calling :code:`margo_finalize`.
 *
 * @param[in] mid Margo instance
 * @param[in] provider_id provider id
 * @param[in] pool Argobots pool
 * @param[out] provider provider handle
 *
 * @return SPIDX_SUCCESS or error code defined in spx_common.h
 */
int spx_provider_register(
        margo_instance_id mid,
        uint16_t provider_id,
        ABT_pool pool,
        spx_provider_t* provider);

/**
 * @brief Destroys the SPIDX provider and deregisters its RPC.
 *
 * @param[in] provider SPIDX provider
 *
 * @return SPIDX_SUCCESS or error code defined in spx_common.h
 */
int spx_provider_destroy(
        spx_provider_t provider);

#ifdef __cplusplus
}
#endif

#endif