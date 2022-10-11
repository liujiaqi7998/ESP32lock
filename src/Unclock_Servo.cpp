#include "Unclock_Servo.h"

#define open_angle 80 //解锁角度
#define close_angle 0 //正常角度
#define servo_pin 21 //设置舵机引脚21


Servo unclock_servo;

void unclock_servo_setup()
{
    ESP32PWM::allocateTimer(0);
    unclock_servo.attach(servo_pin, 1000, 2000);
}

void unclock_servo_open()
{
    Serial.println("unclock_servo_open");
    unclock_servo.setPeriodHertz(50); // 开启频�?
    unclock_servo.write(open_angle);
    delay(1000);
    Serial.println("unclock_servo_close");
    unclock_servo.write(close_angle);
    delay(1000);
    unclock_servo.setPeriodHertz(0); // 关闭频率，防止烧舵机
}