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

    pinMode(2, OUTPUT);    
    digitalWrite(2, HIGH);
    delay(500);
    // WakeUp RP2040 via i2C
    // Wire.end();
    // pinMode(IIC_SDA, OUTPUT);    
    // pinMode(IIC_SCL, OUTPUT);
    // digitalWrite(IIC_SDA, HIGH);
    // digitalWrite(IIC_SCL, HIGH);
    
    // digitalWrite(IIC_SDA, LOW);
    // digitalWrite(IIC_SCL, LOW);
    // pinMode(IIC_SDA, INPUT);    
    // pinMode(IIC_SCL, INPUT);
    // Wire.begin(IIC_SDA,IIC_SCL);
    
    
}


void HWInit(){
    InitPWR();
    Serial.begin(115200);
    // pinMode(BUZZER_PIN, OUTPUT);
    log_d("LuLu2 hello");    
    InitPSRAM();
    InitFS();
}