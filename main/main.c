#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "adc_init_config.h"
#include "mcpwm_init_config.h"
#include "i2c_master_init_config.h"
#include "nimble_init_config.h"
#include "main.h"

/* Private functions */
static void nimble_host_task(void *param);

/* Private variables */
//const static char *TAG = "DRONE";
static int adc_raw[2][10];
static adc_oneshot_unit_handle_t adc_oneshot_unit_handle;
static mcpwm_cmpr_handle_t mcpwm_cmpr_handle;
static i2c_master_dev_handle_t i2c_master_dev_handle;

static void nimble_host_task(void *param) 
{
    /* Task entry log */
    ESP_LOGI(TAG, "nimble host task has been started!");

    /* This function won't return until nimble_port_stop() is executed */
    nimble_port_run();

    /* Clean up at exit */
    vTaskDelete(NULL);
}

void adc_oneshot_config_main()
{
    /* ----- INITIALIZE AND CONFIGURE ADC_ONESHOT ----- */
    adc_oneshot_init(ADC_UNIT_2, 0, ADC_ULP_MODE_DISABLE, &adc_oneshot_unit_handle);
    adc_oneshot_config(
            ADC_ATTEN_DB_12, 
            ADC_BITWIDTH_DEFAULT, 
            adc_oneshot_unit_handle, 
            ADC_CHANNEL_0);
    return;
}

void mcpwm_config_main()
{
    /* ----- INITIALIZE AND CONFIGURE MCPWM ----- */
    //TIMER CONFIG 
    mcpwm_timer_handle_t mcpwm_timer_handle = NULL;
    mcpwm_timer_config(
            0, 
            MCPWM_TIMER_CLK_SRC_DEFAULT, 
            20000000,                        //20,000,000 ticks/s 
            MCPWM_TIMER_COUNT_MODE_UP,
            1000,                            //1000 ticks/cycle
            0,                                   
            0,
            0,
            &mcpwm_timer_handle);   //           ticks    1    cycles
                                    // 2,000,000 ----- * ---  ------- = 20,000 cycle/s = 20kHz
                                    //             s     100   ticks

    //MCPWM OPERATOR CONFIG
    mcpwm_oper_handle_t mcpwm_oper_handle = NULL;
    mcpwm_operator_config(0, 0, 0, 0, 0, 0, 0, 0, &mcpwm_oper_handle);

    //MCPWM COMPARATOR CONFIG
    mcpwm_comparator_config(0, 0, 0, 0, mcpwm_oper_handle, &mcpwm_cmpr_handle);

    //MCPWM GENERATOR CONFIG
    mcpwm_gen_handle_t mcpwm_gen_handle = NULL;
    mcpwm_generator_config(27, 0, 0, 0, 0, 0, mcpwm_oper_handle, &mcpwm_gen_handle);

    
    //SETUP MCPWM TIMER
    mcpwm_timer_enable(mcpwm_timer_handle);
    mcpwm_timer_start_stop(mcpwm_timer_handle, MCPWM_TIMER_START_NO_STOP);
    mcpwm_operator_connect_timer(mcpwm_oper_handle, mcpwm_timer_handle);
    mcpwm_comparator_set_compare_value(mcpwm_cmpr_handle, 0);
    
    //SETUP MCPWM GENERATOR BEHAVIOR FOR HIGH DUTY CYCLE PORTION
    //timer will count up until reaching 0 (overflow) then, set high on 0
    mcpwm_gen_timer_event_action_t mcpwm_gen_timer_event_action = {
        .direction = MCPWM_TIMER_DIRECTION_UP,
        .event = MCPWM_TIMER_EVENT_EMPTY,
        .action = MCPWM_GEN_ACTION_HIGH,
    };
        
    ESP_ERROR_CHECK(
            mcpwm_generator_set_actions_on_timer_event(
                mcpwm_gen_handle, 
                mcpwm_gen_timer_event_action,
                MCPWM_GEN_TIMER_EVENT_ACTION_END()));


    //SETUP MCPWM GENERATOR BEHAVIOR FOR LOW DUTY CYCLE PORTION
    //timer will count up until reaching compare value, then set low on compare value,
    //thus the duty cycle will increase proportionally to the compare value,
    //with a higher compare value corresponding to a longer duty cycle
    mcpwm_gen_compare_event_action_t mcpwm_gen_compare_event_action = {
        .direction = MCPWM_TIMER_DIRECTION_UP,
        .comparator = mcpwm_cmpr_handle,
        .action = MCPWM_GEN_ACTION_LOW,
    };
    ESP_ERROR_CHECK(
            mcpwm_generator_set_actions_on_compare_event(
                mcpwm_gen_handle,
                mcpwm_gen_compare_event_action,
                MCPWM_GEN_COMPARE_EVENT_ACTION_END()));

    return;
}

void i2c_master_config_main()
{
    /* INITIALIZE AND CONFIGURE I2C MASTER TO CONTROL GYROSCOPE/ACCELEROMETER MODULE */
    i2c_master_bus_handle_t i2c_master_bus_handle = NULL;
    i2c_master_bus_config(-1, 21, 22, I2C_CLK_SRC_DEFAULT, 7, 0, 0, 0, &i2c_master_bus_handle);
    i2c_device_config(I2C_ADDR_BIT_LEN_7, 0x68, 400000, 0, 0, i2c_master_bus_handle, &i2c_master_dev_handle);
    return;
}

void nimble_config_main()
{
    /* Local variables */
    int rc;
    esp_err_t ret;

    /* NVS flash initialization */
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "failed to initialize nvs flash, error code: %d ", ret);
        return;
    }

    /* NimBLE host stack initialization */
    ret = nimble_port_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "failed to initialize nimble stack, error code: %d ",
                 ret);
        return;
    }

    /* GAP service initialization */
    rc = gap_init();
    if (rc != 0) {
        ESP_LOGE(TAG, "failed to initialize GAP service, error code: %d", rc);
        return;
    }

    /* Store host configuration */
    ble_store_config_init();

    /* NimBLE host configuration initialization */
    nimble_host_config_init();
    return;
}

void app_main(void)
{
    const TickType_t xDelay = 3000 / portTICK_PERIOD_MS; 
    /* ----- INITIALIZE THE COMPONENTS BEING USED ----- */
//    adc_oneshot_config_main(&adc_oneshot_unit_handle);    //INIT ADC ONESHOT
    mcpwm_config_main(&mcpwm_cmpr_handle);                  //INIT MCPWM
    i2c_master_config_main(&i2c_master_dev_handle);         //INIT I2C MASTER
    nimble_config_main();                                   //INIT NIMBLE 
    /* ----- START ANY THREADS ----- */
    xTaskCreate(nimble_host_task, "NimBLE Host", 4*1024, NULL, 5, NULL);
    
    /* ----- READ FROM ADC_ONESHOT AND USE VALUE TO SET MCPWM COMPARE VALUE ----- */
    while(1) {
        //READ ADC VALUE (ANALOG INPUT FOR MOTOR SPEED)
//        ESP_ERROR_CHECK(adc_oneshot_read(adc_oneshot_unit_handle, ADC_CHANNEL_0, &adc_raw[0][0]));
        
        //SET MCPWM DUTY CYCLE BASED ON ADC VALUE READ PREVIOUSLY
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(mcpwm_cmpr_handle, (1000)));
        //OUTPUT ADC READ AND MCPWM VALUE TO LOG
//        ESP_LOGI(TAG, "ADC ONESHOT READ: %d, MCPWM COMPARE VALUE SET TO %d\n", adc_raw[0][0],  (1000*adc_raw[0][0]/4095));
        
        //READ GYRO/ACCEL VALUES FROM I2C SLAVE DEVICE
        //
        //for register addresses and meaning see the instructions on page 37:
        //https://www.haoyuelectronics.com/Attachment/GY-521/mpu6050.pdf
        
        //initialize i2c_master_device_handle
        const uint8_t gyro_reg[1] = {0x43}; //gyro out register
        const uint8_t accel_reg[1] = {0x3b}; //accel out register                                                      
        uint8_t gyro_read_buffer[6];  
        uint8_t accel_read_buffer[6];
        //read gyro x,y,z values into read_buffer
        ESP_ERROR_CHECK(i2c_master_transmit_receive(
                    i2c_master_dev_handle, 
                    gyro_reg, 
                    sizeof(gyro_reg), 
                    gyro_read_buffer,
                    sizeof(gyro_read_buffer),
                    3000));
        //print read_buffer values 
        //collate high and low bits into single value for x,y,z
        int16_t gyro_xyz[3];
        for(int i = 0; i < 3; ++i)
            gyro_xyz[i] = (gyro_read_buffer[2*i] << 8) + gyro_read_buffer[2*i+1];
        
        ESP_LOGI(TAG, 
                "Gyro x: %.2f deg/s, gyro y: %.2f deg/s, gyro z: %.2f deg/s\n", 
                gyro_xyz[0]/131.0, 
                gyro_xyz[1]/131.0, 
                gyro_xyz[2]/131.0);
        
        //read accel x,y,z values into read_buffer
        ESP_ERROR_CHECK(i2c_master_transmit_receive(
                    i2c_master_dev_handle, 
                    accel_reg, 
                    sizeof(accel_reg), 
                    accel_read_buffer,
                    sizeof(accel_read_buffer),
                    3000));

        int16_t accel_xyz[3];
        for(int i = 0; i < 3; ++i)
            accel_xyz[i] = (accel_read_buffer[2*i] << 8) + accel_read_buffer[2*i+1];
        
        ESP_LOGI(TAG,
                "Accel x: %.2f g, accel y: %.2f g, accel z: %.2f g\n",
                accel_xyz[0]/16384.0,
                accel_xyz[1]/16384.0,
                accel_xyz[2]/16384.0);
        //print read_buffer values 
        /*
        ESP_LOGI(TAG, "ACCEL RAW :\n");
        for(int i = 0; i < 6; ++i) {
            ESP_LOGI(TAG, "%x", accel_read_buffer[i]);
        ESP_LOGI(TAG, "\n");
        }
        */     
        vTaskDelay(xDelay);
    }
}
