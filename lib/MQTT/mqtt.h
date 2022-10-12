#ifndef __MQTT_H
#define __MQTT_H
#include <Arduino.h>
void mqtt_callback(char *topic, byte *payload, unsigned int length);
void mqtt_int();
void mqtt_disable();
void mqtt_while();
#endif // __TASK_H