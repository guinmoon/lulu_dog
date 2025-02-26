#ifndef LVGL_HELPER_H
#define LVGL_HELPER_H

#include <Arduino.h>
#include "lv_conf.h"
#include "demos/lv_demos.h"


class LuLuDog;

class LVGLHelper
{
private:
    
    static const uint16_t screenWidth = 280;
    static const uint16_t screenHeight = 240;

    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf[screenWidth * screenHeight / 10];
    lv_obj_t *label;
    static LuLuDog* luluDog;
    static void LVGLTimerLoopThread(void* _this);

public:
    // static Arduino_GFX *gfx;
    static bool lvglExit;
    static esp_timer_handle_t lvgl_tick_timer;
    // static TFT_eSPI* gfx;
    LVGLHelper(LuLuDog* _luluDog);    
    void InitDisplayLVGL();
    void BuildApp();
    static void ExitMenu(lv_event_t * e);
    void StopLVGL();
    static void GoSleep(lv_event_t * e);
    static void Action1(lv_event_t * e);
    static void Action2(lv_event_t * e);
    static void Action3(lv_event_t * e);
    static void WifiSwitchEventHandler(lv_event_t * e);
    static void MoveSwitchEventHandler(lv_event_t * e);
    static void AudioSwitchEventHandler(lv_event_t * e);
    static void GetValuesFromConfig();
    void ShowMenu();
    static void example_increase_lvgl_tick(void *arg);
    void cleanup_resources();
};

#endif