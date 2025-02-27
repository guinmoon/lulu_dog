#include <Arduino.h>
#include "lv_conf.h"
#include "demos/lv_demos.h"
#include "lvgl_helper.h"
#include "lulu_dog.h"
#include "ui.h"




void LVGLHelper::GoSleep(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_CLICKED)
    {
        luluDog->luluCharacter->GoToDeepSleep();
    }
}

void LVGLHelper::Action1(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_CLICKED)
    {
        luluDog->Action1();
    }
}

void LVGLHelper::Action2(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_CLICKED)
    {
        luluDog->Action2();
    }
}

void LVGLHelper::Action3(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_CLICKED)
    {
        luluDog->Action3();
    }
}

void LVGLHelper::WifiSwitchEventHandler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        LV_UNUSED(obj);
        bool wifiOn = lv_obj_has_state(obj, LV_STATE_CHECKED) ? true : false; 
        luluDog->configHelper->SetProperty("wifi", wifiOn);                     
    }
}

void LVGLHelper::MoveSwitchEventHandler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        LV_UNUSED(obj);
        bool moveOn = lv_obj_has_state(obj, LV_STATE_CHECKED) ? true : false; 
        luluDog->configHelper->SetProperty("move", moveOn);                     
    }
}

void LVGLHelper::AudioSwitchEventHandler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        LV_UNUSED(obj);
        bool audioOn = lv_obj_has_state(obj, LV_STATE_CHECKED) ? true : false; 
        luluDog->configHelper->SetProperty("audio", audioOn);                     
    }
}