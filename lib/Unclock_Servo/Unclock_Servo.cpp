#include "Unclock_Servo.h"

#define open_angle 80             //解锁角度
#define close_angle 0             //正常角度
#define servo_pin 21              //设置舵机引脚21
#define servo_freq 50             //固定50Hz
#define servo_max_pulse_with 2500 //舵机最大脉宽(us)
#define servo_min_pulse_with 500  //舵机最小脉宽(us)
Servo unclock_servo;

void unclock_servo_setup()
{
    // ESP32PWM::allocateTimer(0);
    // unclock_servo.attach(servo_pin, 1000, 2000);
    pwm_setup();
}

void pwm_setup()
{
    pinMode(servo_pin, OUTPUT);
}

void pwm_generate(int angle, int ms)
{
    int cycle = ms > 20 ? ms / 20 : 1;
    uint32_t time = angle / 180.0 * (servo_max_pulse_with - servo_min_pulse_with) + servo_min_pulse_with;
    Serial.printf("time = %d\ncycle = %d\n", time, cycle);
    while (cycle--)
    {
        digitalWrite(servo_pin, HIGH);
        delayMicroseconds(time);
        digitalWrite(servo_pin, LOW);
        delayMicroseconds(20000 - time);
    }
}

void open_Task(void *parameter)
{
    Serial.println("[硬件控制]: 解锁");
    // unclock_servo.setPeriodHertz(50); // 开启频率为50Hz
    pwm_generate(open_angle, 1000);
    Serial.println("[硬件控制]: 上锁");
    pwm_generate(close_angle, 1000);
    // unclock_servo.setPeriodHertz(0); // 关闭频率，防止烧舵机
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