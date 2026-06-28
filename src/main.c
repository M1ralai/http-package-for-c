#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error/error.h"
#include "server/server.h"

#define DEBUG

int main() {
	hcb_server_t *server = hcb_new_server("8080", hcb_new_pool_config(32, 32));
	hcb_server_start(server);
	if (hcb_error_get_is_error(hcb_server_get_error(server))) {
		printf("%s", hcb_error_formatted_get(hcb_server_get_error(server)));
	} else {
		printf("There is no error congrats");
	}
	hcb_server_free(server);
}

/*
int response(int *sock) {
	int buffer_len = 320;
	char *buffer = (char *)malloc(buffer_len);
	int received = recv(*sock, buffer, buffer_len, 0);
	if (received == -1) {
		perror("error occured when recieving data");
		return -1;
	}
	printf("%d byte received, message: %s", received, buffer);
	free(buffer);
	return 0;
}

void *loop(void *sock) {
	int disconnected = 0;
	int *sock_int = (int *)sock;
	while (!disconnected) {
		disconnected = response(sock_int);
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;

	int err = getaddrinfo(NULL, "8080", &hints, &res);
	if (err != 0) {
		fprintf(stderr, "errro occured when getting addr info: %s\n", gai_strerror(err));
		exit(-1);
	}

	int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socket_fd == -1) {
		perror("error occured when set socket");
		exit(-1);
	}

	int yes = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

	err = bind(socket_fd, res->ai_addr, res->ai_addrlen);
	if (err != 0) {
		perror("error occured when binding");
		exit(-1);
	}
	freeaddrinfo(res);

	err = listen(socket_fd, 5);
	if (err != 0) {
		perror("error occured when listening:");
		exit(-1);
	}

	pthread_t conn_threads;
	while (1) {
		struct sockaddr conn_sock;
		int len = sizeof(conn_sock);
		int conn_fd = accept(socket_fd, &conn_sock, &len);
		if (conn_fd == -1) {
			perror("error occured when accepting connection");
			exit(-1);
		}

		if (pthread_create(&conn_threads, NULL, loop, &conn_fd) != 0) {
			perror("Thread oluşturulamadı");
			close(conn_fd);
			continue;
		}

		pthread_join(conn_threads, NULL);
	}
	close(socket_fd);
	return 0;
}
*/
