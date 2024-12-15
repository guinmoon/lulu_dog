#include "battery_helper.h"
#include "display_helper.h"
#include <Arduino.h>
#include "global_def.h"



const int voltageDividerPin = VOLTAGE_DIVEDER_PIN;
float vRef = VREF;    // Power supply voltage of ESP32-S3 (unit: volts)
float R1 = BATTERY_R1; // Resistance value of the first resistor (unit: ohms)
float R2 = BATTERY_R2;

float currentVoltage = 0;

void init_battery()
{
    pinMode(voltageDividerPin, INPUT);
    start_battery_thread();
}

float get_battery_voltage_cached()
{
    return currentVoltage;
}

float get_battery_voltage()
{
    int adcValue = analogRead(voltageDividerPin);

    // Convert to voltage
    float voltage = (float)adcValue * (vRef / 4095.0);

    // Apply the voltage divider formula to calculate the actual voltage
    currentVoltage = voltage * ((R1 + R2) / R2);
    return currentVoltage;
}

void battery_thread(void *params)
{
    while (true)
    {
        get_battery_voltage();
        setVoltageBuf(currentVoltage);
        delay(1000);
    }
    vTaskDelete(NULL);
}



void start_battery_thread()
{
    xTaskCreatePinnedToCore(
        battery_thread, /* Task function. */
        "Task4",        /* name of task. */
        10000,          /* Stack size of task */
        NULL,           /* parameter of the task */
        3,              /* priority of the task */
        NULL,           /* Task handle to keep track of created task */
        1);
}