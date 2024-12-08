#include <stdbool.h>
#include <stdint.h>

#define DIRECTION_PIN_1 16
#define DIRECTION_PIN_2 17
#define PWM_PIN 18

void motor_init_front(void);
void motor_control_front_motors(uint8_t speed, bool forward);