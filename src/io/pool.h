#ifndef HCB_POOL
#define HCB_POOL

#ifdef __cplusplus
extern "C" {
#endif

#include "../error/error.h"

typedef struct hcb_pool_config hcb_pool_config_t;

hcb_pool_config_t *hcb_new_pool_config(const int write_timeout, const int read_timeout);

typedef struct hcb_pool hcb_pool_t;

void hcb_pool_new_connection(hcb_pool_t *pool, const int conn_fd);

hcb_pool_t *hcb_new_pool(hcb_pool_config_t *cfg);

hcb_error_t *hcb_pool_get_error(hcb_pool_t *pool);

hcb_pool_t *hcb_pool_free(hcb_pool_t *pool);

#ifdef __cplusplus
}
#endif
#endif
