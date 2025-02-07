#ifndef TOUCH_HELPER_H
#define TOUCH_HELPER_H

#include "TouchDrvCSTXXX.hpp"
#include "character.h"
#include "lvgl.h"

class LuLuDog;

class TouchHelper
{
private:
    LuLuDog* luluDog;
    static TouchDrvCSTXXX touch;
    static int16_t x[5];
    static int16_t y[5];
    static bool isPressed ;
    

    unsigned long lastTouchTime = 0;
    bool longPressDetected = false;
    bool doubleTapDetected = false;
    bool released = false;
    bool longPressActivated = false;

    bool wasPressed = false;

    unsigned long pressStartTime = 0;
    unsigned long lastReleaseTime = 0;

    // Константы для настройки временных интервалов
    const unsigned long longPressThreshold = 1500; // 1000 мс (1 секунда)
    const unsigned long doubleTapTimeout = 800;

public:
    bool suspended = false;
    TouchHelper(LuLuDog* _luluDog);
    void InitTouch();
    void detectLongOrDoubleTap();
    static void TouchReadThread(void *params);
    void TouchReadTask();
    static void LVGLTouchpadRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
};

#endif