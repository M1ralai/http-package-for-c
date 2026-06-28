
#ifndef HCB_SOCKET
#define HCB_SOCKET

#ifdef __cplusplus
extern "C" {
#endif

#include "../error/error.h"

typedef struct hcb_socket hcb_socket_t;

hcb_socket_t *hcb_new_socket(const char *port);

void hcb_socket_start(hcb_socket_t *sock);

void hcb_socket_listen(hcb_socket_t *sock);

int hcb_socket_accept_loop(hcb_socket_t *sock);

hcb_error_t *hcb_socket_get_error(const hcb_socket_t *sock);

hcb_socket_t *hcb_socket_free(hcb_socket_t *sock);

#ifdef __cplusplus
}
#endif

#endif
