/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* Includes */
#include "led.h"
#include "common.h"

/* Public functions */
void led_on(void) { gpio_set_level(BLE_CONNECT_LED_GPIO, true); }

void led_off(void) { gpio_set_level(BLE_CONNECT_LED_GPIO, false); }

void led_init(void) {
    ESP_LOGI(TAG, "example configured to blink gpio led!");
	//configure LED GPIO
	gpio_config_t io_conf = {};
	io_conf.pin_bit_mask = 1 << BLE_CONNECT_LED_GPIO;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
	io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);
}
