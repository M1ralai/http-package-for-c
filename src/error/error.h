#ifndef HCB_SERVER_ERROR
#define HCB_SERVER_ERROR

#ifdef __cplusplus
extern "C" {
#endif

// status code 0 mean there is no error
typedef struct hcb_error hcb_error_t;

hcb_error_t *new_hcb_error(const char *module_name);

hcb_error_t *hcb_error_free(hcb_error_t *err);

const char *hcb_error_formatted_get(hcb_error_t *err);

void hcb_error_set(hcb_error_t *err, const char *msg);

#ifdef __cplusplus
}
#endif

#endif
