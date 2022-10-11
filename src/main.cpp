#include <Arduino.h>
#include <Serial_LCD.h>
#include <LCD_keypad.h>
#include <Unclock_Servo.h>

void keypad_Task(void *parameter)
{
  Serial.println("[线程管理]:启动键盘扫描线程");
  while (1)
  {
    keypad_while();
    delay(1);
  }
}

void LCD_Task(void *parameter)
{
  Serial.println("[线程管理]:屏幕通讯线程");
  while (1)
  {
    LCD_while();
    delay(1);
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  LCD_setup();
  unclock_servo_setup();

  //当按下 IO0 按钮，进入菜单
  pinMode(0, INPUT);
  attachInterrupt(0, show_menu, FALLING); // 设置外部中断
}

void loop()
{
  LCD_while();
  keypad_while();
  // put your main code here, to run repeatedly:
}
