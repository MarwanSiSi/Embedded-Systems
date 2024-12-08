#include "stdbool.h"

#define BR_MOTOR_PIN1 10
#define BR_MOTOR_PIN2 11
#define BR_MOTOR_PWM_PIN 12


// function declarations
void motor_init_back_right();
void motor_control_back_right(int speed,bool forward);
