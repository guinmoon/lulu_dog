#ifndef LVGL_HELPER_H
#define LVGL_HELPER_H

#include <Arduino.h>
#include "lv_conf.h"
#include "demos/lv_demos.h"
// #include <TFT_eSPI.h>
#include "Arduino_GFX_Library.h"

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
    static Arduino_GFX *gfx;
    static bool lvglExit;
    static esp_timer_handle_t lvgl_tick_timer;
    // static TFT_eSPI* gfx;
    LVGLHelper(LuLuDog* _luluDog);
    static void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
    void InitDisplayLVGL();
    void BuildApp();
    static void ExitMenu(lv_event_t * e);
    static void GoSleep(lv_event_t * e);
    void ShowMenu();
    static void example_increase_lvgl_tick(void *arg);
};

#endif