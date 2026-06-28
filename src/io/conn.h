#ifndef HCB_CONN
#define HCB_CONN

#ifdef __cpluscplus
extern "C" {
#endif

typedef struct hcb_conn hcb_conn_t;

hcb_conn_t *hcb_new_conn();

hcb_conn_t *hcb_conn_free(hcb_conn_t *conn);

#ifdef __cplusplus
}
#endif
#endif
