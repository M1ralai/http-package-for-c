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
	int yes = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

	err = bind(socket_fd, res->ai_addr, res->ai_addrlen);
	if (err != 0) {
		perror("error occured when binding");
		exit(-1);
	}
	freeaddrinfo(res);
	sock->socket_fd = socket_fd;
}

void hcb_socket_listen(hcb_socket_t *sock) {
	int err = listen(sock->socket_fd, 5);
	if (err != 0) {
		char err_buf[ERROR_MESSAGE_SIZE];
		snprintf(err_buf, ERROR_MESSAGE_SIZE, "socket error occured while listening: %s", strerror(errno));
		hcb_error_set(sock->err, err_buf);
		return;
	}
}

int hcb_socket_accept_loop(hcb_socket_t *sock) {
	struct sockaddr conn_sock;
	int len = sizeof(conn_sock);
	int ret_fd = accept(sock->socket_fd, &conn_sock, (socklen_t *)&len);
	if (ret_fd == -1) {
		char err_buf[ERROR_MESSAGE_SIZE];
		snprintf(err_buf, ERROR_MESSAGE_SIZE, "socket error occured while accepting: %s", strerror(errno));
		hcb_error_set(sock->err, err_buf);
		return 0;
	}
	return ret_fd;
}

hcb_socket_t *hcb_socket_free(hcb_socket_t *sock) {
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
