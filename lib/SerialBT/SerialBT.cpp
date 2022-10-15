#include "SerialBT.h"

#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void int_bt_Serial(){
    SerialBT.begin("EN_Door"); //Bluetooth device name
}