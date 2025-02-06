// https://aliexpress.ru/item/1005007552403202.html?sku_id=12000041264231643&spm=a2g2w.productlist.search_results.0.27e25c64JaDElS

#include "lulu_dog.h"
#include "hw_init.h"


static LuLuDog luluDog;


void setup(void)
{

    HWInit();
    luluDog.Init();
    // batteryHelper.InitBattery();
    // displayHelper.InitDisplay();
    // touchHelper.InitTouch();
    // gyroHelper.InitGyro();
    // audioHelper.InitAudio();
    // audioHelper.PlayWav("/woof1.wav");
    // displayHelper.PlayGif("/eye5.gif");
    // luluCharacter.StartDogActivitiWatcher();
 
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
    // delay(5);
    // lv_timer_handler();
}