#include "Unclock_Servo.h"

#define open_angle 80 //解锁角度
#define close_angle 0 //正常角度
#define servo_pin 21  //设置舵机引脚21

Servo unclock_servo;

void unclock_servo_setup()
{
    ESP32PWM::allocateTimer(3);
    unclock_servo.attach(servo_pin, 1000, 2000);
    unclock_servo.setPeriodHertz(50); // 开启频率为50Hz
}

void open_Task(void *parameter)
{
    Serial.println("[硬件控制]: 解锁");
    for (int i = close_angle; i < open_angle; i++)
    {
        unclock_servo.write(i);
        delay(10);
    }
    delay(1000);
    Serial.println("[硬件控制]: 上锁");
    for (int i = open_angle; i > close_angle; i--)
    {
        unclock_servo.write(i);
        delay(10);
    }
    delay(1000);
    vTaskDelete(NULL);
}

void unclock_servo_open()
{
    xTaskCreate(
        open_Task,   /* Task function. */
        "open_Task", /* String with name of task. */
        10000,       /* Stack size in bytes. */
        NULL,        /* Parameter passed as input of the task */
        1,           /* Priority of the task. */
        NULL);       /* Task handle. */
}