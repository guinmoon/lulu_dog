
#ifndef DISLPAY_HELPER_H
#define DISLPAY_HELPER_H

#include "Arduino_GFX_Library.h"
#include <DigitalRainAnimation.hpp>
// #include <TFT_eSPI.h>
#include <Wire.h>
#include <AnimatedGIF.h>
#include "LittleFS.h"
#include "global_def.h"
#include "battery_helper.h"

class LuLuDog;

class DisplayHelper
{
private:
    
    // File gifFile;
    uint8_t *gifData = nullptr;
    int32_t gifSize = 0;
    TaskHandle_t Task1;
    char voltageBuf[15];

    bool play = true;
    bool wake = false;

    uint8_t *pTurboBuffer;
    uint8_t *pFrameBuffer;
    
    // BatteryHelper* batteryHelper;
    LuLuDog* luluDog;

public:
    static Arduino_DataBus *bus;
    static Arduino_GFX *gfx;
    static AnimatedGIF gif;
    static DigitalRainAnimation<Arduino_GFX> matrix_effect;
    // DisplayHelper(BatteryHelper* _batteryHelper);
    DisplayHelper(LuLuDog* _luluDog);
    static void GIFDraw(GIFDRAW *pDraw);
    static void GIFDraw_24bit(GIFDRAW *pDraw);
    bool loadGIFToMemory(const char *filename);
    static void PlayInfiniteThread(void *pvParameters);
    void PlayInfiniteTask();
    void InitDisplay();
    void PlayGif(const char *fname);
    void printOnDisplay(char *text, int x, int y);
    void setVoltageBuf(float voltage);
    void showSleepAnimation();
    void StopGif();    
    void stopSleepAnimation();
    void drawBatteryheart();
    void fillScreen();
    void DisplayOff();
    void DisplayOn();
    void ShowMatrixAnimation();
    void InitMatrixAnimation();
    void StopMatrixAnimation();
    static void MatrixAnimationThread(void* _this);
    void drawHeart(int x, int y, uint16_t color);
    static bool showMatrixAnimation;
};



// extern DisplayHelper displayHelper;
#endif