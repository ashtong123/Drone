#include "adc_init_config.h"

void adc_oneshot_init(adc_unit_t unit_id, adc_oneshot_clk_src_t clk_src, adc_ulp_mode_t ulp_mode, adc_oneshot_unit_handle_t *out_handle)
{
    adc_oneshot_unit_handle_t handle = NULL;

    const adc_oneshot_unit_init_cfg_t config = {
        .unit_id = unit_id,
        .clk_src = clk_src,
        .ulp_mode = ulp_mode,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&config, &handle));
    *out_handle = handle;
    return;
}

void adc_oneshot_config(adc_atten_t atten, adc_bitwidth_t bitwidth, adc_oneshot_unit_handle_t handle, adc_channel_t channel)
{
    const adc_oneshot_chan_cfg_t config = {
        .bitwidth = bitwidth,
        .atten = atten,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(handle, channel, &config));
    return;
}

