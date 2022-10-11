#include <network_config.h>
#include <WiFi.h>
#include <Preferences.h>
#include <Serial_LCD.h>

void setup_wifi()
{
    Preferences prefs;     // 声明Preferences对象
    prefs.begin("config"); // 打开命名空间config
    WiFi.mode(WIFI_AP_STA);
    String wifi_ssid = prefs.getString("ssid", "");     // 读取ssid
    String wifi_passwd = prefs.getString("passwd", ""); // 读取passwd
    if (wifi_ssid != "" && wifi_passwd != "")
    {
        WiFi.begin(wifi_ssid.c_str(), wifi_passwd.c_str());
        Serial.println("[网络管理]:WIFI连接到:" + wifi_ssid);
    }
    else
    {
        Serial.println("[网络管理]:没有读取到WIFI数据，不启动网络");
    }
    prefs.end(); // 关闭命名空间config
}

void network_config_begin()
{
    Serial.println("[线程管理]:启动SmartConfig配网线程");
    Preferences prefs;     // 声明Preferences对象
    prefs.begin("config"); // 打开命名空间config
    WiFi.beginSmartConfig();
    // Wait for SmartConfig packet from mobile
    Serial.println("[网络管理]:等待SmartConfig配网");
    while (!WiFi.smartConfigDone())
    {
        LCD_print("network.t0.txt=\"—\"");
        delay(500);
        LCD_print("network.t0.txt=\"|\"");
        delay(500);
    }
    prefs.putString("ssid", WiFi.SSID());  // 写入ssid
    prefs.putString("passwd", WiFi.psk()); // 写入passwd
    prefs.end();                           // 关闭命名空间config
    Serial.println("[网络管理]:配网完成，等待30秒重启");
    LCD_print("tips.title.txt=\"配网成功，请重启\"");
    LCD_print("tips.center.txt=\"WIFI: " + WiFi.SSID() + "\r密码: " + WiFi.psk() + "\"");
    LCD_print("tips.last_page.val=0");
    LCD_print("page tips");
    delay(30000);
    ESP.restart();
}