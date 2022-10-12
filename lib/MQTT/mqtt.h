#ifndef __MQTT_H
#define __MQTT_H
#include <Arduino.h>
void mqtt_callback(char *topic, byte *payload, unsigned int length);
void mqtt_int();
void mqtt_disable();
void mqtt_while();
void mqtt_show_tips(String title, String center);
void mqtt_check_online();
#endif // __TASK_H