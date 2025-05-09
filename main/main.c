/* Includes */
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "adc_init_config.h"
#include "mcpwm_init_config.h"
#include "i2c_master_init_config.h"
#include "nimble_init_config.h"



/* Private functions */
static void nimble_host_task(void *param);



/* Private variables */
//Motor m1-m4 PWM control handles
static mcpwm_cmpr_handle_t cmpr_handles[4];
//GY-521 coms i2c handle
static i2c_master_dev_handle_t i2c_master_dev_handle;



/* Tasks */
static void nimble_host_task(void *param) 
{
    /* Task entry log */
    ESP_LOGI(TAG, "nimble host task has been started!");

    /* This function won't return until nimble_port_stop() is executed */
    nimble_port_run();

    /* Clean up at exit */
    vTaskDelete(NULL);
}

static void heart_rate_task(void *param) 
{
    /* Task entry log */
    ESP_LOGI(TAG, "heart rate task has been started!");

    /* Loop forever */
    while (1) {
        /* Update heart rate value every 1 second */
        update_heart_rate();
        ESP_LOGI(TAG, "heart rate updated to %d", get_heart_rate());

        /* Send heart rate indication if enabled */
        send_heart_rate_indication();

        /* Sleep */
        vTaskDelay(HEART_RATE_TASK_PERIOD);
    }

    /* Clean up at exit */
    vTaskDelete(NULL);
}



/* Configurations */
void mcpwm_config_main()
{
    /* ----- INITIALIZE AND CONFIGURE MCPWM ----- */
    //MCPWM TIMER CONFIG 
    mcpwm_timer_handle_t mcpwm_timer_handle = NULL;
    mcpwm_timer_config(
            0, 
            MCPWM_TIMER_CLK_SRC_DEFAULT, 
            20000000,                        //20,000,000 ticks/s 
            MCPWM_TIMER_COUNT_MODE_UP,
            1000,                            //1000 ticks/cycle, duty cycle ranges from 0-1000, resolution of 1
            0,                                   
            0,
            0,
            &mcpwm_timer_handle);   //           ticks    1    cycles
                                    // 20,000,000 ----- * ---  ------- = 20,000 cycle/s = 20kHz
                                    //             s     1000   ticks

    //MCPWM OPERATOR CONFIG
    mcpwm_oper_handle_t mcpwm_oper_handle = NULL;
    mcpwm_operator_config(0, 0, 0, 0, 0, 0, 0, 0, &mcpwm_oper_handle);


	/* Motor 1 setup */
    //MCPWM COMPARATOR CONFIG
    mcpwm_comparator_config(0, 0, 0, 0, mcpwm_oper_handle, &cmpr_handles[0]);
    //MCPWM GENERATOR CONFIG
    mcpwm_gen_handle_t mcpwm_gen_handle_m1 = NULL;
    mcpwm_generator_config(27, 0, 0, 0, 0, 0, mcpwm_oper_handle, &mcpwm_gen_handle_m1); //GPIO27
	
	/* Motor 2 setup */
    //MCPWM COMPARATOR CONFIG
    mcpwm_comparator_config(0, 0, 0, 0, mcpwm_oper_handle, &cmpr_handles[1]);
    //MCPWM GENERATOR CONFIG
    mcpwm_gen_handle_t mcpwm_gen_handle_m2 = NULL;
    mcpwm_generator_config(14, 0, 0, 0, 0, 0, mcpwm_oper_handle, &mcpwm_gen_handle_m2); //GPIO 14
    
	/* Motor 3 setup */
    //MCPWM COMPARATOR CONFIG
    mcpwm_comparator_config(0, 0, 0, 0, mcpwm_oper_handle, &cmpr_handles[2]); 
    //MCPWM GENERATOR CONFIG
    mcpwm_gen_handle_t mcpwm_gen_handle_m3 = NULL;
    mcpwm_generator_config(13, 0, 0, 0, 0, 0, mcpwm_oper_handle, &mcpwm_gen_handle_m3); //GPIO 13
	
	/* Motor 4 setup */
    //MCPWM COMPARATOR CONFIG
    mcpwm_comparator_config(0, 0, 0, 0, mcpwm_oper_handle, &cmpr_handles[3]);
    //MCPWM GENERATOR CONFIG
    mcpwm_gen_handle_t mcpwm_gen_handle_m4 = NULL;
    mcpwm_generator_config(12, 0, 0, 0, 0, 0, mcpwm_oper_handle, &mcpwm_gen_handle_m4); //GPIO 12
	
	//Combine motor 1-4 handles into array
	mcpwm_gen_handle_t gen_handles[4] = {
		mcpwm_gen_handle_m1, 
		mcpwm_gen_handle_m2, 
		mcpwm_gen_handle_m3,
		mcpwm_gen_handle_m4};
	
    //SETUP MCPWM TIMER
    mcpwm_timer_enable(mcpwm_timer_handle);
    mcpwm_timer_start_stop(mcpwm_timer_handle, MCPWM_TIMER_START_NO_STOP);
    mcpwm_operator_connect_timer(mcpwm_oper_handle, mcpwm_timer_handle);
	
	//initialize motors m1-m4 to off (0 duty cycle)
	for(int i = 0; i < 4; ++i)
		mcpwm_comparator_set_compare_value(cmpr_handles[i], 0);
	
    //SETUP MCPWM GENERATOR BEHAVIOR FOR HIGH DUTY CYCLE PORTION for motors m1-m4
    //timer will count up until reaching 0 (overflow) then, set high on 0
	for(i = 0; i < 4; ++i) {
		mcpwm_gen_timer_event_action_t mcpwm_gen_timer_event_action = {
			.direction = MCPWM_TIMER_DIRECTION_UP,
			.event = MCPWM_TIMER_EVENT_EMPTY,
			.action = MCPWM_GEN_ACTION_HIGH,
		};
			
		ESP_ERROR_CHECK(
				mcpwm_generator_set_actions_on_timer_event(
					gen_handles[i], 
					mcpwm_gen_timer_event_action,
					MCPWM_GEN_TIMER_EVENT_ACTION_END()));


		//SETUP MCPWM GENERATOR BEHAVIOR FOR LOW DUTY CYCLE PORTION
		//timer will count up until reaching compare value, then set low on compare value,
		//thus the duty cycle will increase proportionally to the compare value,
		//with a higher compare value corresponding to a longer duty cycle
		mcpwm_gen_compare_event_action_t mcpwm_gen_compare_event_action = {
			.direction = MCPWM_TIMER_DIRECTION_UP,
			.comparator = cmpr_handles[i],
			.action = MCPWM_GEN_ACTION_LOW,
		};
		ESP_ERROR_CHECK(
				mcpwm_generator_set_actions_on_compare_event(
					gen_handles[i],
					mcpwm_gen_compare_event_action,
					MCPWM_GEN_COMPARE_EVENT_ACTION_END()));
	}
	
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



/* Main */
void app_main(void)
{
    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; //control system refresh rate 
	
    /* ----- INITIALIZE THE COMPONENTS BEING USED ----- */
    mcpwm_config_main(&mcpwm_cmpr_handle);                  //CONFIG AND INIT MCPWM
    i2c_master_config_main(&i2c_master_dev_handle);         //CONFIG AND INIT I2C MASTER
    nimble_init(&cmpr_handles);                        //CONFIG AND INIT NIMBLE 
    
	
	/* ----- START ANY THREADS ----- */
    xTaskCreate(nimble_host_task, "NimBLE Host", 4*1024, NULL, 5, NULL);
    xTaskCreate(heart_rate_task, "Heart Rate", 4*1024, NULL, 5, NULL);

    
	/* ----- CONTROL LOOP ----- */
    while(1) {
        /* Sensor Reads */
		//variables for reading and writing
        const uint8_t gyro_reg[1] = {0x43}; 	//gyro out register address
        const uint8_t accel_reg[1] = {0x3b}; 	//accel out register address                                               
        uint8_t gyro_read_buffer[6];  			//raw gyro data written here
        uint8_t accel_read_buffer[6];			//raw accel data written here
        //read gyro x,y,z values into read_buffer
        ESP_ERROR_CHECK(i2c_master_transmit_receive(
                    i2c_master_dev_handle, 
                    gyro_reg, 
                    sizeof(gyro_reg), 
                    gyro_read_buffer,
                    sizeof(gyro_read_buffer),
                    3000));
        //data is read from two separate registers one for MSB one for LSB, 
		//so we collate high and low bits into individual values for each of x,y,z
        int16_t gyro_xyz[3];
        for(int i = 0; i < 3; ++i)
            gyro_xyz[i] = (gyro_read_buffer[2*i] << 8) + gyro_read_buffer[2*i+1];
        
		//print the human-formatted gyro data to the log
        ESP_LOGI(TAG, 
                "Gyro x: %.2f deg/s, gyro y: %.2f deg/s, gyro z: %.2f deg/s\n", 
                gyro_xyz[0]/131.0, 
                gyro_xyz[1]/131.0, 
                gyro_xyz[2]/131.0);
        
		//repeat the steps above for the accelerometer read
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
		
		
		
		/* Sensor data processing */
		
		
		
		/* Controller output calculation */
		
		
		
		/* Pass controller outputs to motors */
		
		
        
		/* Refresh rate delay (may not be needed) */
		vTaskDelay(xDelay);
    }
}
