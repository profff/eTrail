#include "motor.h"

DacESP32 dac1(GPIO_NUM_25);
static  uint8_t MotorLevel=MOTOR_MIN_LEVEL;

void motor_setup(){

}
void motor_loop(){
  MotorLevel++;
  if(MotorLevel<MOTOR_MIN_LEVEL)
  MotorLevel=MOTOR_MIN_LEVEL;
  dac1.outputVoltage((uint8_t)MotorLevel);
  Serial.print(" M:");
  Serial.print(MotorLevel,1);
  Serial.println();

}
uint8_t motor_getValue(){
  return  MotorLevel;
}
