This project contains the firmware needed to run a quadcopter drone based on the ESP32 platform. The current hardware consists
of the following:
 - ESP dev board (the smaller the better)
 - PWM controlled brushed DC motors with propellers
	- Flyback diodes and amplifying BJT transistors for each
 - GY-521 gyroscope/accelerometer module
	    //
        //for register addresses and meaning see the instructions on page 37:
        //https://www.haoyuelectronics.com/Attachment/GY-521/mpu6050.pdf
        
 - HC-SR04 Ultrasonic sensor module 
 - A 3D printed frame
 - Perfboard holding the electrical components together
 
On the software side, per the ESP32 IDF guidance, the project is split into "components", which are as follows:
- main component --> main method, threaded functions, and initializing calls for each of the other components 
- nimble component --> configuration and initialization functions for the NimBLE stack, used for remotely communicating 
to the drone
- mcmpwm component --> configuration and initialization functions for the motor controller PWM 
- adc_init_config --> configuration and initialization functions for the analog to digital converter, used to convert 
analog signals to PWM duty cycles, this may become a legacy feature - it was originally used to allow a wired potentiometer to 
control the motor duty, but I am currently transitioning to a bluetooth based input, which would allow for direct digital control
- i2c_master_init_config --> configuration and initialization functions for i2c master, used to communicate with the GY-521

The general setup philosophy of the components is as follows:
(a) non-main component --> header and source files for functions which need to be called to initialize and configure the module,
called "{module_name}_init_config"
(b) main component --> for each module, contains a function "{module_name}_config_main" which holds the calls to the setup functions
declared and defined in (a)

In addition, some components require threaded functions, which are also in the main component. Finally, some component related-code
is used in the actual app_main() function in the main component.

When building ESP32 projects, the default is to include all possible unmanaged modules in the build structure. This project 
tries to avoid including any unnecessary code. This is accomplished by explicitly telling CMake which modules to include in the
project-level and component-level CMakeLists.txt. 

In addition, the sdkconfig file contains important configurations for the project, and will likely be updated continuously as
the project grows

This project is a work in progress and is a personal project of mine, which I am undertaking both as an intellectual challenge 
and a serious engineering pursuit. Aside from the personal value, this project is of general value because it seeks to make the
simplest, lightest, cheapest quadcopter drone possible. The electrical components can generally be obtained for a few dollars,  
and the frame can be printed for free if you own a printer, or for a small fee if you don't. 

Due to the complexity of tuning the controller for a quadcopter drone, I plan to build several test stands and simulations to
get approximate tunings before setting an actual quadcopter loose. These are in the related repo, drone-sim
