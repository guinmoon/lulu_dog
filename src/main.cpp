// https://aliexpress.ru/item/1005007552403202.html?sku_id=12000041264231643&spm=a2g2w.productlist.search_results.0.27e25c64JaDElS

#include <Arduino.h>
#include <Wire.h>
#include "LittleFS.h"
#include "global_def.h"
#include "display_helper.h"
#include "gyro_helper.h"
#include "battery_helper.h"
#include "audio_helper.h"
#include "commands.h"
#include "character.h"

void initPSRAM()
{
    psramInit();
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());
}

void initFS()
{
    if (!LittleFS.begin())
    {
        log_d("Failed to mount LittleFS");
        return;
    }
}

void InitPWR(){
    pinMode(SYS_EN_PIN, OUTPUT);
    digitalWrite(SYS_EN_PIN, HIGH);
}

void setup(void)
{
    InitPWR();
    Serial.begin(115200);
    // pinMode(BUZZER_PIN, OUTPUT);    
    log_d("LuLu2 hello");
    initBattery();
    initPSRAM();
    initFS();
    InitDisplay();    
    gyroInit();
    initAudio();
    playWav("/woof2.wav");
    // playGif("/eye1.gif");
    // sendCommand(COMMAND_SET_TAIL_SPEED, 7);
    // delay(200);
    // sendCommand(COMMAND_DANCE1,4);
    playGif("/eye5.gif");
    startDogActivitiWatcher();
}

void loop()
{
    // myTone(BUZZER_PIN);
    // delay(200);
    // myNoTone(BUZZER_PIN);
    // tone(BUZZER_PIN, 2000);
    // delay(1000);
    // noTone(BUZZER_PIN);
    delay(1000);
}