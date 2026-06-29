#include "conn.h"
#include "../error/error.h"
#include <sys/socket.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct hcb_conn {
	int conn_fd;
	int is_active;
	hcb_error_t *err;
};

hcb_conn_t *hcb_new_conn() {
	hcb_conn_t *conn;
	conn = malloc(sizeof(*conn));
	hcb_error_t *err = hcb_new_error("conn");
	conn->conn_fd = -1;
	conn->is_active = 0;
	conn->err = err;
	return conn;
}

void *hcb_conn_loop(void *arg) {
	hcb_conn_t *conn = (hcb_conn_t *)arg;
	int running = 1;
	while (running) {
		char buf[256];
		int ret = recv(conn->conn_fd, buf, 256, 0);
		if (ret == -1) {
			hcb_error_set(conn->err, "error occured");
			return NULL;
		} else if (ret == 0) {
			hcb_error_set(conn->err, "connection closed by peer");
			return NULL;
		}
		printf("%s", buf);
	}
	close(conn->conn_fd);
	conn->is_active = 0;
	return NULL;
}

void hcb_conn_establish(hcb_conn_t *conn, const int conn_fd) {
	conn->conn_fd = conn_fd;
	conn->is_active = 1;
	char buf[256] = "Hello World!";
	int ret = send(conn->conn_fd, buf, 256, 0);
	if (ret == -1) {
		hcb_error_set(conn->err, "establishment error");
	}
	pthread_t t;
	pthread_create(&t, NULL, hcb_conn_loop, conn);
}

int hcb_conn_get_is_active(hcb_conn_t *conn) {
	return conn->is_active;
}

hcb_error_t *hcb_conn_get_error(hcb_conn_t *conn) {
	return conn->err;
}

hcb_conn_t *hcb_conn_free(hcb_conn_t *conn) {
	if (conn->err != NULL) {
		conn->err = hcb_error_free(conn->err);
	}
	free(conn);
	return NULL;
}
