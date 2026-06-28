#include "socket.h"

#include <sys/socket.h>
#include <sys/types.h>

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../error/error.h"

struct hcb_socket {
	int socket_fd;
	const char *port;
	hcb_error_t *err;
};

hcb_socket_t *hcb_new_socket(const char *port) {
	hcb_socket_t *ret;
	ret = malloc(sizeof(*ret));
	hcb_error_t *err = hcb_new_error("Socket");
	ret->err = err;
	ret->socket_fd = -1;
	ret->port = port;
	return ret;
}

void hcb_socket_start(hcb_socket_t *sock) {
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	int err = getaddrinfo(NULL, sock->port, &hints, &res);
	if (err != 0) {
		char err_buf[ERROR_MESSAGE_SIZE];
		snprintf(err_buf, ERROR_MESSAGE_SIZE, "error occured when getting addrinfo: %s", gai_strerror(err));
		hcb_error_set(sock->err, err_buf);
		return;
	}
	int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socket_fd == -1) {
		char err_buf[ERROR_MESSAGE_SIZE];
		snprintf(err_buf, ERROR_MESSAGE_SIZE, "socket error occured: %s", strerror(errno));
		hcb_error_set(sock->err, err_buf);
		return;
	}
	sock->socket_fd = socket_fd;
	freeaddrinfo(res);
}

void *hcb_socket_free(hcb_socket_t *sock) {
	if (sock != NULL) {
		if (sock->err != NULL) {
			sock->err = hcb_error_free(sock->err);
		}
		if (sock->socket_fd != -1) {
			close(sock->socket_fd);
		}
		free(sock);
	}
	return NULL;
}

hcb_error_t *hcb_socket_get_error(const hcb_socket_t *sock) {
	return sock->err;
}
