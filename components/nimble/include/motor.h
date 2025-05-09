/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#ifndef MOTOR_H
#define MOTOR_H

/* Includes */
/* ESP APIs */
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/mcpwm_types.h"
#include "driver/mcpwm_cmpr.h"

/* Defines */

/* Public function declarations */
uint8_t get_motor_state(void);
void motor_on(void);
void motor_off(void);
void motor_init(mcpwm_cmpr_handle_t *cmpr_handles);

#endif // MOTOR_H
