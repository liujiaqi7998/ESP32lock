#include "mqtt.h"
#include "WiFiClient.h"
#include <PubSubClient.h>
#include "ArduinoJson.h"
#include <Preferences.h>
#include "WiFi.h"

// MQTT Broker
const char *mqtt_broker = "broker-cn.emqx.io"; //服务器地址
const int mqtt_port = 1883;                    //端口号
const char *mqtt_username = "emqx";            //用户名
const char *mqtt_password = "public";          //密码
const char *rec_topic = "ENdoor/rec";          //订阅主题
const char *send_topic = "ENdoor/send";        //发送主题
const char *clientID = "ESP32-20501";          //客户端ID

WiFiClient espClient;
PubSubClient client(espClient);

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    String raw_data;
    for (int i = 0; i < length; i++)
    {
        raw_data += (char)payload[i];
    }
    // Serial.println("[网络管理]:收到了数据:" + raw_data);
    DynamicJsonDocument doc(2048);
    if (deserializeJson(doc, raw_data))
    {
        Serial.println("[网络管理]:MQTT收到了非JSON数据:" + raw_data);
        return;
    }
    //先过一下管理员验证
    Preferences prefs;                                           // 声明Preferences对象
    prefs.begin("config");                                       // 打开命名空间config
    String real_admin_password = prefs.getString("admin_k", ""); // 读取管理员密码
    prefs.end();
    String key = doc["key"].as<String>();
    if (key != real_admin_password)
    {
        Serial.println("[网络管理]:MQTT数据密码验证错误:" + key);
        mqtt_show_tips("错误", "密码错误");
        return;
    }

    String type = doc["type"].as<String>();

    if (type == "set_admin_passwd")
    {
        // 设置管理员密码
        String new_passwd = doc["passwd"].as<String>();

        if (new_passwd.length() != 6)
        {
            // 长度不对
            mqtt_show_tips("设置密码失败", "密码必须是长度6位");
            return;
        }
        Preferences prefs;                                         // 声明Preferences对象
        prefs.begin("config");                                     // 打开命名空间config
        String real_user_password = prefs.getString("user_k", ""); // 读取用户密码
        if (new_passwd == real_user_password)
        {
            // 密码和普通用户密码相同
            mqtt_show_tips("设置密码失败", "管理员密码不能和普通密码相同");
            return;
        }
        prefs.putString("admin_k", new_passwd); // 写入解锁密码
        prefs.end();                            // 关闭命名空间
                                                // 设置成功
        Serial.println("[网络管理]:修改管理员密码为:" + new_passwd);
        mqtt_show_tips("修改成功", "管理员密码已修改");
        return;
    }
    if (type == "set_user_passwd")
    {
        // 设置用户密码
        String new_passwd = doc["passwd"].as<String>();

        if (new_passwd.length() != 6)
        {
            // 长度不对
            mqtt_show_tips("设置密码失败", "密码必须是长度6位");
            return;
        }
        Preferences prefs;                                          // 声明Preferences对象
        prefs.begin("config");                                      // 打开命名空间config
        String real_user_password = prefs.getString("admin_k", ""); // 读取用户密码
        if (new_passwd == real_user_password)
        {
            // 密码和普通用户密码相同
            mqtt_show_tips("设置密码失败", "普通密码不能和管理员密码相同");
            return;
        }
        prefs.putString("user_k", new_passwd); // 写入解锁密码
        prefs.end();                           // 关闭命名空间
                                               // 设置成功
        Serial.println("[网络管理]:修改普通密码为:" + new_passwd);
        mqtt_show_tips("修改成功", "普通密码已修改");
        return;
    }
    if (type == "check")
    {
        mqtt_check_online();
        return;
    }
}

// MQTT控制网页弹出提示框
void mqtt_show_tips(String title, String center)
{
    DynamicJsonDocument doc(2048);
    doc["type"] = "tips";
    doc["title"] = title;
    doc["center"] = center;
    String json;
    serializeJson(doc, json);
    client.publish(send_topic, json.c_str());
}
// 在线设备检查
void mqtt_check_online()
{
    DynamicJsonDocument doc(2048);
    doc["type"] = "online";
    doc["MAC"] = String(WiFi.macAddress());
    doc["IP"] = WiFi.localIP().toString();
    String json;
    serializeJson(doc, json);
    client.publish(send_topic, json.c_str());
}

void mqtt_int()
{
    client.disconnect();
    client.setServer(mqtt_broker, mqtt_port);
    client.connect(clientID, mqtt_username, mqtt_password);
    client.setCallback(&mqtt_callback);
    client.subscribe(rec_topic);
    mqtt_check_online();
    Serial.println("[网络管理]:mqtt连接成功");
}

void mqtt_disable()
{
    client.disconnect();
}

void mqtt_while()
{
    client.loop();
}