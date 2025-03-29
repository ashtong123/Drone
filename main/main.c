#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"

const static char *TAG = "DRONE";
static int adc_raw[2][10];


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

void app_main(void)
{
    adc_oneshot_unit_handle_t handle = NULL;
    adc_oneshot_init(ADC_UNIT_2, 0, ADC_ULP_MODE_DISABLE, &handle);
    adc_oneshot_config(ADC_ATTEN_DB_12, ADC_BITWIDTH_DEFAULT, handle, ADC_CHANNEL_0);

    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

    while(1) {
        ESP_ERROR_CHECK(adc_oneshot_read(handle, ADC_CHANNEL_0, &adc_raw[0][0]));
        ESP_LOGI(TAG, "ADC%d CHANNEL[%d] Raw Data: %d", ADC_UNIT_2 + 1, ADC_CHANNEL_0, adc_raw[0][0]);
    
        vTaskDelay(xDelay);
    }
}
