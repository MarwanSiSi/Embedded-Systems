#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "light.h"

#include "br_motor.h"
#include "bl_motor.h"
#include "front_motors.h"

#include "speed.h"
#include "led.h"
#include "speed_led.h"
#include "buzzer.h"
#include "ultrasonicFront.h"
#include "ultrasonicBehind.h"


void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == ECHO_PIN_FRONT) {
        if (events & GPIO_IRQ_EDGE_RISE) {
            front_start_time = time_us_32();
        } else if (events & GPIO_IRQ_EDGE_FALL) {
            front_end_time = time_us_32();
            front_measurement_done = true;
        }
    }
    else if (gpio == ECHO_PIN_BEHIND) {
        if (events & GPIO_IRQ_EDGE_RISE) {
            behind_start_time = time_us_32();
        } else if (events & GPIO_IRQ_EDGE_FALL) {
            behind_end_time = time_us_32();
            behind_measurement_done = true;
        }
    }
}

int main() {
    stdio_init_all();

    //init light sensor
    init_light_sensor();
  
    // based ambient light, turn on leds
    init_led();
     
    // init motor front left
    motor_init_back_left();

    // init motor front right
    motor_init_back_right();

    // init front motors
    motor_init_front();

    // init speed sensor
    init_speed_sensor();

    // init buzzer
    init_buzzer();


    // init ultrasonic sensor front
    init_ultrasonic_sensor_front();
    

    // init ultrasonic sensor behind
    init_ultrasonic_sensor_behind();

    // Register single callback for both pins
    gpio_set_irq_enabled_with_callback(ECHO_PIN_FRONT, 
                                 GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                                 true,
                                 &gpio_callback);

    gpio_set_irq_enabled(ECHO_PIN_BEHIND, 
                    GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                    true);

    // Variables for buzzer timing
    uint32_t last_beep_time = 0;
    uint32_t current_time;
    
    while(true) {
    float distanceBehind = get_distance_behind_cm();
    float distanceFront = get_distance_front_cm();
    printf("Front: %f  cm: \n", distanceFront);
    printf("Behind: %f  cm: \n", distanceBehind);
    
    current_time = to_ms_since_boot(get_absolute_time());
    
    // Graduated beeping based on distance for both sensors
    uint32_t beep_interval = 0;
    bool should_beep = false;
    
    // Check front distance
    if(distanceFront > 0 && distanceFront < 20) {
        should_beep = true;
        if(distanceFront < 5) {
            beep_interval = 100;  // Very fast beeping (100ms) when very close
        } else if(distanceFront < 10) {
            beep_interval = 300;  // Medium fast beeping (300ms) when moderately close
        } else {
            beep_interval = 500;  // Slower beeping (500ms) when further away
        }
    }
    
    // Check behind distance - use the shorter interval if both sensors detect obstacles
    if(distanceBehind > 0 && distanceBehind < 20) {
        should_beep = true;
        uint32_t behind_interval;
        if(distanceBehind < 5) {
            behind_interval = 100;
        } else if(distanceBehind < 10) {
            behind_interval = 300;
        } else {
            behind_interval = 500;
        }
        // Use the shorter interval (faster beeping) if both sensors are triggered
        beep_interval = (beep_interval > 0) ? 
            (behind_interval < beep_interval ? behind_interval : beep_interval) : 
            behind_interval;
}
    
    // Beep if either sensor detected an obstacle
    if(should_beep && current_time - last_beep_time >= beep_interval) {
        buzzer_on();
        sleep_ms(50);  // Short beep duration
        buzzer_off();
        last_beep_time = current_time;
    }

        // Light sensor logic
        int readingLight = get_light_reading();
        if(readingLight > 1700) {
            led_on();
        } else {
            led_off();
        }

        // Motor control
        motor_control_back_left(255, true);
        motor_control_back_right(255, true);
        motor_control_front_motors(255, true);

        int rpm_fast = calculate_motor_speed();
        printf("Speed: %d RPM\n", rpm_fast);
        
    }
}