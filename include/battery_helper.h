#ifndef BATTERY_HELPER_H
#define BATTERY_HELPER_H


#include "global_def.h"
// #include "display_helper.h"

class LuLuDog;

class BatteryHelper
{
private:
    // DisplayHelper* displayHelper;  
    LuLuDog* luluDog;  
    const int voltageDividerPin = VOLTAGE_DIVEDER_PIN;
    float vRef = VREF;     // Power supply voltage of ESP32-S3 (unit: volts)
    float R1 = BATTERY_R1; // Resistance value of the first resistor (unit: ohms)
    float R2 = BATTERY_R2;

    float currentVoltage = 0;
    float lastVoltage = -1;
    bool charging = false;

public:
    BatteryHelper(LuLuDog* _luluDog);
    // BatteryHelper();
    // void SetDisplayHelper(DisplayHelper* _displayHelper);
    void InitBattery();
    void BatteryTask();
    float get_battery_voltage_cached();
    float get_battery_voltage();
    static void battery_thread(void *_this);
    void fillScreen();
    void printVoltageOnScreen();
    bool isCharging();
};

// extern BatteryHelper batteryHelper;
#endif