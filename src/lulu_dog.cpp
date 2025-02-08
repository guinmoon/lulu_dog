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
    lvglHelper->InitDisplayLVGL();
    displayHelper->InitMatrixAnimation();
}

void LuLuDog::setVoltageBuf(float voltage){
    displayHelper->setVoltageBuf( voltage);
}

void LuLuDog::PauseDog(){
    displayHelper->StopGif();
    gyroHelper->PauseGyro();   
    luluCharacter->suspended = true;
    touchHelper->suspended = true;     
}

void LuLuDog::ResumeDog(){    
    gyroHelper->ResumeGyro();
    luluCharacter->suspended = false;
    touchHelper->suspended = false; 
    audioHelper->PlayWav("/woof1.wav");
    displayHelper->PlayGif("/eye5.gif");
}

void LuLuDog::ShowMenu(){
    PauseDog();
    displayHelper->ShowMatrixAnimation();
    delay(1500);
    displayHelper->StopMatrixAnimation();
    lvglHelper->ShowMenu();
}

