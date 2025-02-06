#include "battery_helper.h"
#include <Arduino.h>
#include "global_def.h"
#include "lulu_dog.h"




BatteryHelper::BatteryHelper(LuLuDog* _luluDog){
    this->luluDog =  _luluDog;
}


void BatteryHelper::InitBattery()
{
    pinMode(voltageDividerPin, INPUT);
    randomSeed(analogRead(voltageDividerPin));
    xTaskCreatePinnedToCore(
        this->battery_thread, /* Task function. */
        "Task4",        /* name of task. */
        5000,          /* Stack size of task */
        this,           /* parameter of the task */
        3,              /* priority of the task */
        NULL,           /* Task handle to keep track of created task */
        1);
}

bool BatteryHelper::isCharging(){
    return charging;
}

float BatteryHelper::get_battery_voltage_cached()
{
    return currentVoltage;
}

float BatteryHelper::get_battery_voltage()
{
    int adcValue = analogRead(voltageDividerPin);

    // Convert to voltage
    float voltage = (float)adcValue * (vRef / 4095.0);

    // Apply the voltage divider formula to calculate the actual voltage
    currentVoltage = voltage * ((R1 + R2) / R2);
    return currentVoltage;
}

void BatteryHelper::BatteryTask(){
    while (true)
    {
        get_battery_voltage();
        luluDog->setVoltageBuf(currentVoltage);
        log_d("V: %f",currentVoltage);
        if (lastVoltage == -1)
            lastVoltage = currentVoltage;
        else{
            if (lastVoltage-currentVoltage>=BATTERY_CHARGING_THR){
                charging = false;
            }
            if (currentVoltage>BATTERY_CHARGING_V_MIN && 
                (currentVoltage-lastVoltage>=BATTERY_CHARGING_THR || currentVoltage>=BATTERY_CHARGING_V)) {
                charging = true;
            }
        }
        lastVoltage = currentVoltage;
        delay(1000);
    }
}

void BatteryHelper::battery_thread(void *_this)
{
    ((BatteryHelper *)_this)->BatteryTask();
    vTaskDelete(NULL);
}



// BatteryHelper batteryHelper;