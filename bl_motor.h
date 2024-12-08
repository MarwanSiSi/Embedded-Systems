#include "stdbool.h"

#define BL_MOTOR_PIN1 6
#define BL_MOTOR_PIN2 7
#define BL_MOTOR_PWM_PIN 8


// function declarations
void motor_init_back_left();
void motor_control_back_left(int speed, bool forward);
