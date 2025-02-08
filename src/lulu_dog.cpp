#include "lulu_dog.h"

LuLuDog *LuLuDog::instance;

LuLuDog::LuLuDog()
{
    audioHelper = new AudioHelper();
    batteryHelper = new BatteryHelper(this);
    displayHelper = new DisplayHelper(this);
    luluCharacter = new LuLuCharacter(this);
    gyroHelper = new GyroHelper(this);
    touchHelper = new TouchHelper(this);
    lvglHelper = new LVGLHelper(this);
    instance = this;
    // touchHelper = new TouchHelper();
}

void LuLuDog::Init()
{
    batteryHelper->InitBattery();
    displayHelper->InitDisplay();
    touchHelper->InitTouch();
    touchHelper->doubleTapCallback = this->DoubleTapCallBack;
    touchHelper->longPressCallback = this->LongPressCallBack;
    gyroHelper->InitGyro();
    audioHelper->InitAudio();
    audioHelper->PlayWav("/woof1.wav");
    displayHelper->PlayGif("/eye5.gif");
    luluCharacter->StartDogActivitiWatcher();
    lvglHelper->gfx = displayHelper->gfx;
    // lvglHelper->InitDisplayLVGL();
    displayHelper->InitMatrixAnimation();
}

void LuLuDog::setVoltageBuf(float voltage)
{
    displayHelper->setVoltageBuf(voltage);
}

void LuLuDog::DoubleTapCallBack(int x, int y)
{    
    instance->ShowMenu();
}

void LuLuDog::LongPressCallBack(int x, int y)
{
    instance->displayHelper->StopGif();
    instance->displayHelper->ShowMatrixAnimation();
    // delay(5000);
    // instance->displayHelper->StopMatrixAnimation();
}

void LuLuDog::Action1()
{
    ExitMenu();
    luluCharacter->doReact(COMMAND_DANCE1, 4, 7, "/eye5.gif", "/woof1.wav");
}

void LuLuDog::PauseDog()
{
    displayHelper->StopGif();
    gyroHelper->PauseGyro();
    luluCharacter->suspended = true;
    touchHelper->suspended = true;
}

void LuLuDog::ResumeDog()
{
    gyroHelper->ResumeGyro();
    luluCharacter->suspended = false;
    touchHelper->suspended = false;
    audioHelper->PlayWav("/woof1.wav");
    displayHelper->PlayGif("/eye5.gif");
}

void LuLuDog::ExitMenu()
{
    lvglHelper->StopLVGL();
    displayHelper->fillScreen();
    ResumeDog();
    delay(200);
    
}

void LuLuDog::ShowMenu()
{
    PauseDog();
    displayHelper->ShowMatrixAnimation();
    delay(1500);
    displayHelper->StopMatrixAnimation();
    lvglHelper->InitDisplayLVGL();
    lvglHelper->ShowMenu();
}
