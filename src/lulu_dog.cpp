#include "lulu_dog.h"


LuLuDog::LuLuDog (){    
    audioHelper = new AudioHelper();
    batteryHelper = new BatteryHelper(this);
    displayHelper = new DisplayHelper(this);     
    luluCharacter = new LuLuCharacter(this);
    gyroHelper = new GyroHelper(this);
    touchHelper  = new TouchHelper(this);
    lvglHelper = new LVGLHelper(this);
    // touchHelper = new TouchHelper();
}

void LuLuDog::Init(){
    batteryHelper->InitBattery();
    displayHelper->InitDisplay();
    touchHelper->InitTouch();
    gyroHelper->InitGyro();
    audioHelper->InitAudio();
    audioHelper->PlayWav("/woof1.wav");
    displayHelper->PlayGif("/eye5.gif");
    luluCharacter->StartDogActivitiWatcher();
    lvglHelper->gfx = displayHelper->gfx;
}

void LuLuDog::setVoltageBuf(float voltage){
    displayHelper->setVoltageBuf( voltage);
}

void LuLuDog::PauseDog(){
    displayHelper->StopGif();
    gyroHelper->PauseGyro();    
}

void LuLuDog::ResumeDog(){    
    gyroHelper->ResumeGyro();
}

void LuLuDog::ShowMenu(){
    PauseDog();
    lvglHelper->InitDisplayLVGL();
}

