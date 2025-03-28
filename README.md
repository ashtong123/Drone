This is project with the goal of buliding a quadcopter drone more or less from scratch. There are a few main features I would like to work on:

1. Ability to control a motor from analog signal (such as a joystick from a remote control)
    a. 
    Firstly, I would like to get the ESP32 to read a pot signal into the ADC
    Second, I want to take that signal and use it to control the duty cycle of a PWM
    Third, I want to connect that PWM to the speed of a motor
These goals will get us to a point where we can use an analog device to control our motor speed, which will form the basis for something like using a joystick to maneuver our drone.

2. Ability to read from the gyroscope I am planning to use
    a. 
    First, I want to read the gyroscope values (I believe it uses I2C)
    Next, I want to use those values as feedback to the motor system. 

Eventually, I want the motor system to be controlled in two ways:
1. Autonomous: this is the domain of the gyroscope. The gyroscope is responsible for making sure the drone is 'balanced' and does not accelerate undesirably when receiving the second type of control:
2. Manual: this is the domain of the analog input. This is for functions like increasing the drone height or changing speed/direction.

