#ifndef __UNCLOCK_SERVO_h
#define __UNCLOCK_SERVO_h

#include <Arduino.h>
#include <ESP32Servo.h>

void unclock_servo_setup();
void unclock_servo_open();
void pwm_setup();
void pwm_generate(int angle, int ms);

#endif // __UNCLOCK_SERVO_h