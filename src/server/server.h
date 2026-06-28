#ifndef HCB_SERVER_H
#define HCB_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../error/error.h"
#include "../io/pool.h"

typedef struct hcb_server hcb_server_t;

hcb_server_t *hcb_new_server(const char *port, hcb_pool_config_t *cfg);

void hcb_server_start(hcb_server_t *server);

hcb_server_t *hcb_server_free(hcb_server_t *server);

hcb_error_t *hcb_server_get_error(const hcb_server_t *server);

#ifdef __cplusplus
}
#endif
#endif
