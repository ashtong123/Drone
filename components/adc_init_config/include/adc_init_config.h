#ifndef ADC_INIT_CONFIG_H
#define ADC_INIT_CONFIG_H

#include "esp_adc/adc_oneshot.h"

/* initialize the adc_oneshot handle */
void adc_oneshot_init(adc_unit_t unit_id, adc_oneshot_clk_src_t clk_src, adc_ulp_mode_t ulp_mode, adc_oneshot_unit_handle_t *out_handle);

/* configure the adc_oneshot handle */
void adc_oneshot_config(adc_atten_t atten, adc_bitwidth_t bitwidth, adc_oneshot_unit_handle_t handle, adc_channel_t channel);

#endif
