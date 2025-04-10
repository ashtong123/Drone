#ifndef I2C_MASTER_CONFIG_H
#define I2C_MASTER_CONFIG_H

#include "driver/i2c_master.h"
#include "driver/i2c_types.h"

void i2c_master_bus_config(
        i2c_port_num_t i2c_port,
        gpio_num_t sda_io_num,
        gpio_num_t scl_io_num,
        i2c_clock_source_t clk_source,
        uint8_t glitch_ignore_cnt,
        int intr_priority,
        size_t trans_queue_depth,
        uint32_t enable_internal_pullup,
        i2c_master_bus_handle_t *ret_handle);

void i2c_device_config(
        i2c_addr_bit_len_t dev_addr_length,
        uint16_t device_address,
        uint32_t scl_speed_hz,
        uint32_t scl_wait_us,
        uint32_t disable_ack_check,
        i2c_master_bus_handle_t bus_handle,
        i2c_master_dev_handle_t *ret_handle);

#endif
