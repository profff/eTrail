
#ifndef MOTOR_H
#define MOTOR_H
#include <stdint.h>
#include "DacESP32.h"

void motor_setup();
void motor_loop();
uint8_t motor_getValue();
#define MOTOR_MIN_LEVEL 80
#define MOTOR_MAX_LEVEL 256

extern DacESP32 dac1;

#endif
