#include <Arduino.h>
#include <Wire.h>
#include "LittleFS.h"
#include "hw_init.h"
#include "global_def.h"


void InitPSRAM()
{
    psramInit();
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());
}

void InitFS()
{
    if (!LittleFS.begin())
    {
        log_d("Failed to mount LittleFS");
        return;
    }
}

void InitPWR()
{
    pinMode(SYS_EN_PIN, OUTPUT);
    digitalWrite(SYS_EN_PIN, HIGH);
    // WakeUp RP2040 via i2C
    pinMode(IIC_SDA, OUTPUT);
    digitalWrite(IIC_SDA, HIGH);
    pinMode(IIC_SCL, OUTPUT);
    digitalWrite(IIC_SCL, HIGH);
}


void HWInit(){
    InitPWR();
    Serial.begin(115200);
    // pinMode(BUZZER_PIN, OUTPUT);
    log_d("LuLu2 hello");    
    InitPSRAM();
    InitFS();
}