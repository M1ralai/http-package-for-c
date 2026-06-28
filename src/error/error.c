#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hcb_error {
	const char *module;
	char msg[ERROR_MESSAGE_SIZE];
	int is_error;
};

hcb_error_t *hcb_new_error(const char *module) {
	struct hcb_error *ret;
	ret = malloc(sizeof(*ret));
	if (ret == NULL) {
		printf("couldn't get enough space for err.");
		return NULL;
	}
	ret->module = module;
	ret->is_error = 0;
	return ret;
}

void hcb_error_set(hcb_error_t *err, const char *msg) {
	strncpy(err->msg, msg, ERROR_MESSAGE_SIZE - 1);
	err->is_error = 1;
}

void hcb_error_reset(hcb_error_t *err) {
	err->is_error = 0;
}

const char *hcb_error_formatted_get(const hcb_error_t *err) {
	char *ret;
	ret = malloc(128);
	snprintf(ret, 128, "MODULE: %s; MSG: %s;", err->module, err->msg);
	return ret;
}

hcb_error_t *hcb_error_free(hcb_error_t *err) {
	if (err != NULL) {
		free(err);
	}
	return NULL;
}

int hcb_error_get_is_error(const hcb_error_t *err) {
	return err->is_error;
}
