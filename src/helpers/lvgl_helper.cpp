#include <Arduino.h>
#include "lv_conf.h"
#include "demos/lv_demos.h"
#include "lvgl_helper.h"
#include "lulu_dog.h"
#include "ui.h"
// #define LV_COLOR_16_SWAP 1

lv_disp_draw_buf_t LVGLHelper::draw_buf;
lv_color_t LVGLHelper::buf[screenWidth * screenHeight / 10];
Arduino_GFX *LVGLHelper::gfx;

LVGLHelper::LVGLHelper(LuLuDog *_luluDog)
{
    luluDog = _luluDog;
    gfx = luluDog->displayHelper->gfx;
    // log_d("gfx: %i -> ",luluDog->displayHelper->gfx,gfx);
}

void LVGLHelper::my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
    gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
    log_d("%i, %i, %i, %i, %i, %i", gfx, area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

    lv_disp_flush_ready(disp);
}

#define EXAMPLE_LVGL_TICK_PERIOD_MS 2

// void LVGLHelper::example_increase_lvgl_tick(void *arg)
// {
//     /* Tell LVGL how many milliseconds has elapsed */
//     lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
// }

void LVGLHelper::LVGLTimerLoopThread(void *_this)
{
    while (true)
    {
        delay(5);
        lv_timer_handler();
    }
    // ((LVGLHelper *)_this)->LVGLTimerLoopTask();
    vTaskDelete(NULL);
}

static void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        LV_LOG_USER("Selected month: %s\n", buf);
    }
}

/**
 * An infinite roller with the name of the months
 */
void lv_example_roller_1(lv_obj_t *parent)
{
    lv_obj_t *roller1 = lv_roller_create(parent);
    lv_roller_set_options(roller1,
                          "January\n"
                          "February\n"
                          "March\n"
                          "April\n"
                          "May\n"
                          "June\n"
                          "July\n"
                          "August\n"
                          "September\n"
                          "October\n"
                          "November\n"
                          "December",
                          LV_ROLLER_MODE_INFINITE);

    lv_roller_set_visible_row_count(roller1, 4);
    lv_obj_center(roller1);
    lv_obj_add_event_cb(roller1, event_handler, LV_EVENT_ALL, NULL);
}


void LVGLHelper::BuildApp(){
     lv_example_roller_1(lv_scr_act());
    // label = lv_label_create(lv_scr_act());
    // lv_label_set_text(label, "Initializing...");

    // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void LVGLHelper::InitDisplayLVGL()
{

    lv_init();

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = this->my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = luluDog->touchHelper->LVGLTouchpadRead;
    lv_indev_drv_register(&indev_drv);

    // const esp_timer_create_args_t lvgl_tick_timer_args = {
    //     .callback = &this->example_increase_lvgl_tick,
    //     .name = "lvgl_tick"};

    //   const esp_timer_create_args_t reboot_timer_args = {
    //     .callback = &example_increase_reboot,
    //     .name = "reboot"
    //   };

    // esp_timer_handle_t lvgl_tick_timer = NULL;
    // esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
    // esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000);

    // esp_timer_handle_t reboot_timer = NULL;
    // esp_timer_create(&reboot_timer_args, &reboot_timer);
    // esp_timer_start_periodic(reboot_timer, 2000 * 1000);

//    BuildApp();
    ui_init();

    xTaskCreatePinnedToCore(
        this->LVGLTimerLoopThread, /* Task function. */
        "TaskLVGL",                /* name of task. */
        40000,                     /* Stack size of task */
        this,                      /* parameter of the task */
        2 | portPRIVILEGE_BIT,     /* priority of the task */
        NULL,                      /* Task handle to keep track of created task */
        0);
}
