/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* Includes */
#include "motor.h"
#include "common.h"

static mcpwm_cmpr_handle_t mcpwm_cmpr_handles_local[4]; 
static uint8_t motor_state;

void motor_on(void) { 
    for int(i = 0; i < 4; ++i)
		ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(mcpwm_cmpr_handles_local[i], (1000)));
	ESP_LOGI(TAG, "Motors turned on!");
	motor_state = 1;
}

void motor_off(void) { 
    for int(i = 0; i < 4; ++i)
		ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(mcpwm_cmpr_handles_local[i], (0))); 
	ESP_LOGI(TAG, "Motors turned off!");
	motor_state = 0;
}

void motor_init(mcpwm_cmpr_handle_t *cmpr_handles) {
    ESP_LOGI(TAG, "example configured to toggle motors!");
    mcpwm_cmpr_handles_local = *cmpr_handles;
	//Set initial motor speed as 0
    for int(i = 0; i < 4; ++i)
		ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(mcpwm_cmpr_handles_local[i], (0)));
	motor_state = 0;
}

uint8_t get_motor_state(void) {
	return motor_state;
}