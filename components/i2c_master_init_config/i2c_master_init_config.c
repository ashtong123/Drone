#include "i2c_master_init_config.h"

void i2c_master_bus_config(
        i2c_port_num_t i2c_port,
        gpio_num_t sda_io_num,
        gpio_num_t scl_io_num,
        i2c_clock_source_t clk_source,
        uint8_t glitch_ignore_cnt,
        int intr_priority,
        size_t trans_queue_depth,
        uint32_t enable_internal_pullup,
        i2c_master_bus_handle_t *ret_bus_handle)
{
    const i2c_master_bus_config_t bus_config = {
        .i2c_port = i2c_port,
        .sda_io_num = sda_io_num,
        .scl_io_num = scl_io_num,
        .clk_source = clk_source,
        .glitch_ignore_cnt = glitch_ignore_cnt,
        .intr_priority = intr_priority,
        .trans_queue_depth = trans_queue_depth,
        .flags.enable_internal_pullup = enable_internal_pullup,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, ret_bus_handle));
    return;
}

void i2c_device_config(
        i2c_addr_bit_len_t dev_addr_length,
        uint16_t device_address,
        uint32_t scl_speed_hz,
        uint32_t scl_wait_us,
        uint32_t disable_ack_check,
        i2c_master_bus_handle_t bus_handle,
        i2c_master_dev_handle_t *ret_handle)
{
    const i2c_device_config_t dev_config = {
        .dev_addr_length = dev_addr_length,
        .device_address = device_address,
        .scl_speed_hz = scl_speed_hz,
        .scl_wait_us = scl_wait_us,
        .flags.disable_ack_check = disable_ack_check,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_config, ret_handle));
    return;
}
