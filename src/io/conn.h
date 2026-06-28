#ifndef HCB_CONN
#define HCB_CONN

#ifdef __cpluscplus
extern "C" {
#endif

#include "../error/error.h"

typedef struct hcb_conn hcb_conn_t;

hcb_conn_t *hcb_new_conn();

void hcb_conn_establish(hcb_conn_t *conn, const int conn_fd);

int hcb_conn_get_is_active(hcb_conn_t *conn);

hcb_error_t *hcb_conn_get_error(hcb_conn_t *conn);

hcb_conn_t *hcb_conn_free(hcb_conn_t *conn);

#ifdef __cplusplus
}
#endif
#endif
