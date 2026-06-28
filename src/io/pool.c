#include "pool.h"
#include "../error/error.h"
#include "conn.h"

#include <stdlib.h>

#define HCB_POOL_SIZE 32

// TODO add pool size variable to a pool config then make pool array as custom index based struct;
struct hcb_pool_config {
	int write_timeout;
	int read_timeout;
};

struct hcb_pool {
	hcb_conn_t *conn[HCB_POOL_SIZE];
	hcb_error_t *err;
	hcb_pool_config_t *cfg;
};

hcb_pool_t *hcb_pool_new(hcb_pool_config_t *cfg) {
	hcb_pool_t *ret;
	ret = malloc(sizeof(*ret));
	ret->err = hcb_new_error("Pool");
	ret->cfg = cfg;
	for (int i = 0; i < HCB_POOL_SIZE; i++) {
		ret->conn[i] = hcb_new_conn();
	}
	return ret;
}

hcb_pool_t *hcb_pool_free(hcb_pool_t *pool) {
	if (pool != NULL) {
		for (int i = 0; i < HCB_POOL_SIZE; i++) {
			if (pool->conn[i] != NULL) {
				pool->conn[i] = hcb_conn_free(pool->conn[i]);
			}
		}
		if (pool->err != NULL) {
			pool->err = hcb_error_free(pool->err);
		}
	}
	free(pool);
	return NULL;
}
