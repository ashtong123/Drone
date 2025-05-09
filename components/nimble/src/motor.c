/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* Includes */
#include "motor.h"
#include "common.h"

static mcpwm_cmpr_handle_t mcpwm_cmpr_handle_local; 
static uint8_t motor_state;

void motor_on(void) { 
	ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(mcpwm_cmpr_handle_local, (1000)));
	ESP_LOGI(TAG, "Motors turned on!");
	motor_state = 1;
}

void motor_off(void) { 
	ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(mcpwm_cmpr_handle_local, (0))) 
	ESP_LOGI(TAG, "Motors turned off!");
	motor_state = 0;
}

void motor_init(mcpwm_cmpr_handle_t *mcpwm_cmpr_handle) {
    ESP_LOGI(TAG, "example configured to toggle motors!");
    mcpwm_cmpr_handle_local = *mcpwm_cmpr_handle;
	//Set initial motor speed as 0
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(mcpwm_cmpr_handle_local, (0)));
	motor_state = 0;
}

uint8_t get_motor_state(void) {
	return motor_state;
}