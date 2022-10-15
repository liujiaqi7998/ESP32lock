#include <Libraries.h>



#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
AsyncWebServer server(80);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  LCD_setup(); //初始化LCD

  show_first(); //开机画面
  delay(500);
  Store_Init();          //初始化存储
  unclock_servo_setup(); //初始化舵机
  setup_wifi();          //初始化wifi
  FingerPrint_Init();    //初始化指纹模块
  Task_int();            //初始化多线程
  //当按下 IO0 按钮，进入菜单
  pinMode(0, INPUT);
  attachInterrupt(0, show_menu, FALLING); // 设置外部中断
  //当有手指在指纹传感器上，准备指纹识别
  pinMode(18, INPUT_PULLDOWN);
  attachInterrupt(18, FingerPrint_Unlock, RISING); // 设置外部中断

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "Hi! This is a EN Door Web Page. V5.0.6"); });
  AsyncElegantOTA.begin(&server); // Start AsyncElegantOTA
  server.begin();

  //int_bt_Serial();  // 启动蓝牙串口
  back_home_page(); //开机完成，返回主屏
}

void loop()
{
  // LCD_while();
  // keypad_while();
  // put your main code here, to run repeatedly:
}
