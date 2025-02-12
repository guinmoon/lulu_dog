#ifndef LULU_DOG_H
#define LULU_DOG_H

#include "global_def.h"
#include "display_helper.h"
#include "gyro_helper.h"
#include "battery_helper.h"
#include "audio_helper.h"
#include "commands.h"
#include "character.h"
#include "touch_helper.h"
#include "lvgl_helper.h"
#include "js_helper.h"

class LuLuDog
{
private:
    static LuLuDog* instance;
public:
    
    DisplayHelper* displayHelper;
    GyroHelper* gyroHelper;
    AudioHelper* audioHelper;
    LuLuCharacter* luluCharacter;
    BatteryHelper* batteryHelper;
    TouchHelper* touchHelper;
    LVGLHelper* lvglHelper;
    JSRunner* jsRunner;
    LuLuDog();
    void Init();
    void setVoltageBuf(float voltage);
    void PauseDog();
    void ResumeDog();
    void ShowMenu();
    void Action1();
    void ExitMenu();
    static void DoubleTapCallBack(int x, int y);
    static void LongPressCallBack(int x, int y);
    

};

#endif