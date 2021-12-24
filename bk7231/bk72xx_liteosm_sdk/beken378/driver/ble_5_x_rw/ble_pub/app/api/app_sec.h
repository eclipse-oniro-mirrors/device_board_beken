#ifndef APP_SEC_H_
#define APP_SEC_H_

extern const struct app_subtask_handlers app_sec_handlers;
extern ble_err_t app_sec_load_auth(uint8_t con_idx);
uint8_t app_sec_get_auth(uint8_t con_idx);

#endif
