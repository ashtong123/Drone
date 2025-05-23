#ifndef NIMBLE_INIT_CONFIG_H
#define NIMBLE_INIT_CONFIG_H

#include "common.h"
#include "gap.h"
#include "gatt_svc.h"
#include "heart_rate.h"
#include "motor.h"

/* Library function declarations */
void ble_store_config_init(void);

/* Private function declarations */
void on_stack_reset(int reason);
void on_stack_sync(void);
void nimble_host_config_init(void);
void nimble_init(mcpwm_cmpr_handle_t *cmpr_handles);


#endif
