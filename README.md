Where I left off:

4/3/25:
Wrote initializer and configrator component for ESP I2C interface for use with gyroscope/accelerometer module.
Need to do a more in-depth reading of the gy/acc datasheet to figure out how to format message send/receive. This should be done in main/main.c using the i2c_master_transmit(), i2c_master_receive, and i2c_master_transmit_receive() functions described here: 

https://docs.espressif.com/projects/esp-idf/en/v5.2.5/esp32/api-reference/peripherals/i2c.html#_CPPv419i2c_master_transmit23i2c_master_dev_handle_tPK7uint8_t6size_ti

Once that is done, I can begin testing, probably by using the logic analyzer and/or connecting the gy521 to the ESP32 and sending/receiving actual messages.

4/9/25:
Added some code to send and receive messages to the gy/acc module. Specifically, the code added to the main method attempts to read from some of the gy/acc registers. The registers it attempts to read from contain the gyroscope and acceloremeter x, y, z readings.

The next step is to set up the hardware and test out the code. If everything works, the raw data read from the registers will be printed to the ESP log. In the more likely event that everything does not work, I can break out a logic analyzer and sniff the messages directly and see where things are behaving differently than expected. 

4/11/25:
I set up the hardware and tested the code. It appears that the accelerometer reading is correct (x = 0g, y = 0g, z = 1g when on a flat surface). The gyroscope reading indicated about 5deg/s x, and 0deg/s y, z. I am not sure if this is to be expected or not.

5/4/25:
After extensive testing using 1D and 2D simulations, I think it is necessary to build some hardware to determine if the simulations are actually going to be useful for PID tuning or whatever other purpose they may prove to be useful for. 

Given this, I am now switching from working on purely software sim/test to a hardware-based test method. First, I am adding bluetooth control to the drone, so that tests can be started, stopped, or otherwise manipulated without physical intervention. 

The bluetooth control will be based on the examples in esp-idf/examples/bluetooth/ble_get_started/nimble/* which is located at:

https://github.com/espressif/esp-idf/tree/master/examples/bluetooth/ble_get_started/nimble/

I would like to be able to control the device from my cellphone, the examples use a cellphone app, so I think this should be possible.
