#include "server.h"

#include <stdio.h>
#include <stdlib.h>

#include "../error/error.h"
#include "../io/socket.h"

struct hcb_server {
	hcb_socket_t *sock;
	hcb_error_t *err;
};

hcb_server_t *hcb_new_server(const char *port) {
	hcb_server_t *ret = malloc(sizeof(*ret));
	hcb_error_t *err = hcb_new_error("Server");
	hcb_socket_t *sock = hcb_new_socket(port);
	ret->err = err;
	ret->sock = sock;
	return ret;
}

void hcb_server_start(hcb_server_t *server) {
	hcb_socket_start(server->sock);
	if (hcb_error_get_is_error(hcb_socket_get_error(server->sock))) {
		hcb_error_set(server->err, hcb_error_formatted_get(hcb_socket_get_error(server->sock)));
		hcb_error_reset(hcb_socket_get_error(server->sock));
		return;
	}
}

hcb_error_t *hcb_server_get_error(const hcb_server_t *server) {
	return server->err;
}

hcb_server_t *hcb_server_free(hcb_server_t *server) {
	if (server != NULL) {

		if (server->err != NULL) {
			server->err = hcb_error_free(server->err);
		}
		if (server->sock != NULL) {
			server->sock = hcb_socket_free(server->sock);
		}
		free(server);
	}
	return NULL;
}
