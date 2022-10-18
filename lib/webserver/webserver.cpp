#include "webserver.h"
#include "Unclock_Servo.h"
#include "FingerPrint.h"
#include <Preferences.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <WiFi.h>

AsyncWebServer server(80);

void int_web()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", main_HTML); });

    server.on("/unlock", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                String back_data;
            if (request->hasParam("admin_key")) {
                String inputMessage1;
                inputMessage1 = request->getParam("admin_key")->value();
                //解锁逻辑
                Preferences prefs;                                           // 声明Preferences对象
                prefs.begin("config");                                       // 打开命名空间config
                String real_admin_password = prefs.getString("admin_k", ""); // 读取管理员密码
                prefs.end();
                if(inputMessage1 == real_admin_password){
                     unclock_servo_open();
                    back_data = "{\"e\":\"开锁成功\"}";
                }else{
                    back_data = "{\"e\":\"管理员密码错误\"}";
                }
            }
            else {
                back_data = "{\"e\":\"请输入管理员密码\"}";
            }
            request->send(200, "application/json", back_data); });

    server.on("/getlist", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                String back_data;
            if (request->hasParam("admin_key")) {
                String inputMessage1;
                inputMessage1 = request->getParam("admin_key")->value();
                //解锁逻辑
                Preferences prefs;                                           // 声明Preferences对象
                prefs.begin("config");                                       // 打开命名空间config
                String real_admin_password = prefs.getString("admin_k", ""); // 读取管理员密码
                prefs.end();
                if(inputMessage1 == real_admin_password){
                    back_data = FingerPrint_GetDataJson();
                }else{
                    back_data = "{\"e\":\"管理员密码错误\"}";
                }
            }
            else {
                back_data = "{\"e\":\"请输入管理员密码\"}";
            }
            request->send(200, "application/json", back_data); });

    server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                String back_data;
            if (request->hasParam("admin_key")) {
                String inputMessage1;
                inputMessage1 = request->getParam("admin_key")->value();
                //解锁逻辑
                Preferences prefs;                                           // 声明Preferences对象
                prefs.begin("config");                                       // 打开命名空间config
                String real_admin_password = prefs.getString("admin_k", ""); // 读取管理员密码
                prefs.end();
                if(inputMessage1 == real_admin_password){
                    back_data = "{\"e\":\"连接成功 MAC:" + WiFi.macAddress() + "\"}";
                }else{
                    back_data = "{\"e\":\"管理员密码错误\"}";
                }
            }
            else {
                back_data = "{\"e\":\"请输入管理员密码\"}";
            }
            request->send(200, "application/json", back_data); });

    server.on("/set_user_key", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                String back_data;
            if (request->hasParam("admin_key") && request->hasParam("new_user_key")) {
                String inputMessage1;
                String inputMessage2;
                inputMessage1 = request->getParam("admin_key")->value();
                inputMessage2 = request->getParam("new_user_key")->value();
                //判断inputMessage2是否为6位数字
                if (inputMessage2.length() != 6)
                {
                    back_data = "{\"e\":\"新用户密码必须为6位数字\"}";
                }
                else
                {
                    //解锁逻辑
                    Preferences prefs;                                           // 声明Preferences对象
                    prefs.begin("config");                                       // 打开命名空间config
                    String real_user_password = prefs.getString("user_k", "");   // 读取用户密码
                    String real_admin_password = prefs.getString("admin_k", ""); // 读取管理员密码
                    prefs.end();
                    if(inputMessage1 == real_admin_password){
                        if (inputMessage2 != real_admin_password)
                        {
                            Preferences prefs;                                           // 声明Preferences对象
                            prefs.begin("config");                                       // 打开命名空间config
                            prefs.putString("user_k", inputMessage2); // 写入解锁密码
                            prefs.end();                         // 关闭命名空间
                            back_data = "{\"e\":\"设置成功\"}";
                        }else{
                            back_data = "{\"e\":\"用户密码不能和管理员密码相同\"}";
                        }
                    }else{
                        back_data = "{\"e\":\"管理员密码错误\"}";
                    }
                }
            }
            else {
                back_data = "{\"e\":\"请输入管理员密码和新用户密码\"}";
            }
            request->send(200, "application/json", back_data); });

    server.on("/set_admin_key", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                String back_data;
            if (request->hasParam("admin_key") && request->hasParam("new_admin_key")) {
                String inputMessage1;
                String inputMessage2;
                inputMessage1 = request->getParam("admin_key")->value();
                inputMessage2 = request->getParam("new_admin_key")->value();
                //判断inputMessage2是否为6位数字
                if (inputMessage2.length() != 6)
                {
                    back_data = "{\"e\":\"新管理员密码必须为6位数字\"}";
                }
                else
                {
                    //解锁逻辑
                    Preferences prefs;                                           // 声明Preferences对象
                    prefs.begin("config");                                       // 打开命名空间config
                    String real_user_password = prefs.getString("user_k", "");   // 读取用户密码
                    String real_admin_password = prefs.getString("admin_k", ""); // 读取管理员密码
                    prefs.end();
                    if(inputMessage1 == real_admin_password){
                        if (inputMessage2 != real_user_password)
                        {
                            Preferences prefs;                                           // 声明Preferences对象
                            prefs.begin("config");                                       // 打开命名空间config
                            prefs.putString("admin_k", inputMessage2); // 写入解锁密码
                            prefs.end();                         // 关闭命名空间
                            back_data = "{\"e\":\"设置成功\"}";
                        }else{
                            back_data = "{\"e\":\"管理员密码不能和用户密码相同\"}";
                        }
                    }else{
                        back_data = "{\"e\":\"管理员密码错误\"}";
                    }
                }
            }
            else {
                back_data = "{\"e\":\"请输入管理员密码和新用户密码\"}";
            }
            request->send(200, "application/json", back_data); });

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    AsyncElegantOTA.begin(&server); // Start AsyncElegantOTA
    server.begin();
}