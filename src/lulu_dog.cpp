#include "lulu_dog.h"

LuLuDog *LuLuDog::instance;
ConfigHelper *LuLuDog::configHelper;

LuLuDog::LuLuDog()
{
    configHelper = new ConfigHelper(this);
    dogEvents = new DogEvents(this);
    i2cSlaveHelper = new I2CSlaveHelper(this);    
    batteryHelper = new BatteryHelper(this);
    audioHelper = new AudioHelper(this);    
    displayHelper = new DisplayHelper(this);
    luluCharacter = new LuLuCharacter(this);
    gyroHelper = new GyroHelper(this);
    touchHelper = new TouchHelper(this);
    lvglHelper = new LVGLHelper(this);
    jsRunner = new JSRunner(this);
    fsWebServer = new LuLuWebServer(this);    
    instance = this;
    
    // touchHelper = new TouchHelper();
}



void LuLuDog::Init()
{
    HighPowMode();    
    configHelper->LoadConfig("/config.json");
    batteryHelper->InitBattery();
    displayHelper->InitDisplay();
    touchHelper->InitTouch();    
    gyroHelper->InitGyro();
    audioHelper->InitAudio();
    jsRunner->jsInit();
    audioHelper->PlayWav("/audio/woof1.wav");
    // luluCharacter->doRandomReact(-1,false);
    dogEvents->StartDogActivitiWatcher();
    dogEvents->StartSlavePingThread();
    i2cSlaveHelper->StartReciveThread();
    // ShowMenu();
    // if (configHelper->EnableWifi)
        fsWebServer->Init();

    NormalPowMode();
    // jsRunner->jsEvalFile("/js/script1.js");
    // jsRunner->jsEvalFile("/js/demo.js");


}

void LuLuDog::setVoltageBuf(float voltage)
{
    displayHelper->setVoltageBuf(voltage);
}

// void LuLuDog::DoubleTapCallBack(int x, int y)
// {
//     instance->ShowMenu();
// }

// void LuLuDog::LongPressCallBack(int x, int y)
// {
//     instance->displayHelper->StopGif();
//     instance->displayHelper->ShowMatrixAnimation();
//     // delay(5000);
//     // instance->displayHelper->StopMatrixAnimation();
// }

void LuLuDog::Action1()
{
    ExitMenu();
    jsRunner->jsEvalFile(JS_ACTION1);
}

void LuLuDog::Action2()
{
    ExitMenu();
    jsRunner->jsEvalFile(JS_ACTION2);
}

void LuLuDog::Action3()
{
    ExitMenu();
    jsRunner->jsEvalFile(JS_ACTION3);
}

void LuLuDog::PauseDog()
{
    displayHelper->StopGif();
    displayHelper->pauseEyes();
    gyroHelper->PauseGyro();
    dogEvents->eventsSuspended = true;
    touchHelper->switchToLVGLTask(true);
}

void LuLuDog::MemInfo()
{
    log_d("Used PSRAM: %d", ESP.getPsramSize() - ESP.getFreePsram());
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());    
    log_d("CPU F: %d", getCpuFrequencyMhz());
    log_d("Xtal F: %d", getXtalFrequencyMhz());    
    log_d("Apb F: %d", getApbFrequency());
    
    // uint32_t getXtalFrequencyMhz();
}

void LuLuDog::ResumeDog()
{
    gyroHelper->ResumeGyro();
    dogEvents->eventsSuspended = false;
    touchHelper->switchToLVGLTask(false);
    displayHelper->resumeEyes();
    audioHelper->PlayWav("/audio/woof1.wav");
    luluCharacter->doRandomReact(-1,false);
    // displayHelper->pauseResumeEyes(false);
    // displayHelper->PlayGif("/imgs/eye5.gif");
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
    // displayHelper->ShowMatrixAnimation();
    // delay(1500);
    // displayHelper->StopMatrixAnimation();
    delay(100);

    lvglHelper->InitDisplayLVGL();
    lvglHelper->ShowMenu();
}

void LuLuDog::LowPowMode(){
    setCpuFrequencyMhz(80);
    // MemInfo();
}

void LuLuDog::NormalPowMode(){
    setCpuFrequencyMhz(160);
    // MemInfo();
}

void LuLuDog::HighPowMode(){
    setCpuFrequencyMhz(240);
    // MemInfo();
}