// https://aliexpress.ru/item/1005007552403202.html?sku_id=12000041264231643&spm=a2g2w.productlist.search_results.0.27e25c64JaDElS

#include <Arduino.h>
#include <Wire.h>
#include "LittleFS.h"
#include "pin_config.h"
#include "display_helper.h"
#include "gyro_helper.h"
#include "battery_helper.h"
#include "audio_helper.h"
#include "commands.h"


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


void setup(void)
{
    Serial.begin(115200);
    log_d("LuLu2 hello");
    init_battery();
    initPSRAM();
    initFS();
    InitDisplay();
    gyroInit();
    playGif("/eye3.gif");
    initAudio();
    playWav("/woof2.wav");
}

void loop()
{
    delay(1000);
}