#include "br_motor.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

void motor_init_back_right(){
    gpio_init(BR_MOTOR_PIN1);
    gpio_set_dir(BR_MOTOR_PIN1, GPIO_OUT);

    gpio_init(BR_MOTOR_PIN2);
    gpio_set_dir(BR_MOTOR_PIN2, GPIO_OUT);
    
    gpio_set_function(BR_MOTOR_PWM_PIN, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(BR_MOTOR_PWM_PIN);

    pwm_set_wrap(slice_num, 255); // speed levels
    pwm_set_clkdiv(slice_num, 4.0f);

    pwm_set_enabled(slice_num, true); // enable pwm for the slice
}

void motor_control_back_right(int speed,bool forward){
    if(speed > 255){
        speed = 255;
    }
    else if(speed == 0 || speed < 0){
        speed = 0;
    }
    else if(speed < 151){
        speed = 151;
    }

    if(forward){
        gpio_put(BR_MOTOR_PIN1, 0); 
        gpio_put(BR_MOTOR_PIN2, 1);
    }else{
        gpio_put(BR_MOTOR_PIN1, 1);
        gpio_put(BR_MOTOR_PIN2, 0);
    }

    // Set the PWM duty cycle (range is BRom 0 to 255)
    pwm_set_gpio_level(BR_MOTOR_PWM_PIN, speed);  // Channel A controls the motor
    
}