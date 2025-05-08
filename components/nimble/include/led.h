/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#ifndef LED_H
#define LED_H

/* Includes */
/* ESP APIs */
#include "driver/gpio.h"

/* Configurations */
#define BLE_CONNECT_LED_GPIO 2

/* Public function declarations */
void led_on(void);
void led_off(void);
void led_init(void);

#endif // LED_H