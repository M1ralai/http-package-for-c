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

hcb_pool_config_t *hcb_new_pool_config(int write_timeout, int read_timeout) {
	hcb_pool_config_t *ret;
	ret = malloc(sizeof(*ret));
	ret->write_timeout = write_timeout;
	ret->read_timeout = read_timeout;
	return ret;
}

struct hcb_pool {
	hcb_conn_t *conn[HCB_POOL_SIZE];
	hcb_error_t *err;
	hcb_pool_config_t *cfg;
};

hcb_pool_t *hcb_new_pool(hcb_pool_config_t *cfg) {
	hcb_pool_t *ret;
	ret = malloc(sizeof(*ret));
	ret->err = hcb_new_error("Pool");
	ret->cfg = cfg;
	for (int i = 0; i < HCB_POOL_SIZE; i++) {
		ret->conn[i] = hcb_new_conn();
	}
	return ret;
}

void hcb_pool_new_connection(hcb_pool_t *pool, const int conn_fd) {
	for (int i = 0; i < HCB_POOL_SIZE; i++) {
		if (!hcb_conn_get_is_active(pool->conn[i])) {
			hcb_conn_establish(pool->conn[i], conn_fd);
			if (hcb_error_get_is_error(hcb_conn_get_error(pool->conn[i]))) {
				hcb_error_set(pool->err, hcb_error_formatted_get(hcb_conn_get_error(pool->conn[i])));
				hcb_error_reset(hcb_conn_get_error(pool->conn[i]));
				return;
			}
			return;
		}
	}
}

hcb_error_t *hcb_pool_get_error(hcb_pool_t *pool) {
	return pool->err;
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
