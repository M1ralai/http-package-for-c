#include "conn.h"
#include "../error/error.h"

#include <stdlib.h>

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

hcb_conn_t *hcb_conn_free(hcb_conn_t *conn) {
	if (conn->err != NULL) {
		conn->err = hcb_error_free(conn->err);
	}
	free(conn);
	return NULL;
}
