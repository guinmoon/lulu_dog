#include "lulu_dog.h"

LuLuDog *LuLuDog::instance;
ConfigHelper *LuLuDog::configHelper;

LuLuDog::LuLuDog()
{
    audioHelper = new AudioHelper(this);
    batteryHelper = new BatteryHelper(this);
    displayHelper = new DisplayHelper(this);
    luluCharacter = new LuLuCharacter(this);
    gyroHelper = new GyroHelper(this);
    touchHelper = new TouchHelper(this);
    lvglHelper = new LVGLHelper(this);
    jsRunner = new JSRunner(this);
    fsWebServer = new LuLuWebServer(this);
    configHelper = new ConfigHelper(this);
    instance = this;
    // touchHelper = new TouchHelper();
}

void LuLuDog::Init()
{

    configHelper->LoadConfig("/config.json");
    batteryHelper->InitBattery();
    displayHelper->InitDisplay();
    touchHelper->InitTouch();
    touchHelper->doubleTapCallback = this->DoubleTapCallBack;
    touchHelper->longPressCallback = this->LongPressCallBack;
    gyroHelper->InitGyro();
    audioHelper->InitAudio();
    jsRunner->jsInit();
    audioHelper->PlayWav("/audio/woof1.wav");
    displayHelper->PlayGif("/imgs/eye5.gif");
    luluCharacter->StartDogActivitiWatcher();
    // lvglHelper->gfx = displayHelper->gfx;
    // lvglHelper->InitDisplayLVGL();
    // displayHelper->InitMatrixAnimation();
    // ShowMenu();
    if (configHelper->EnableWifi)
        fsWebServer->Init();

    // luluCharacter->doReact(COMMAND_HALFLAYDOWN, 4, 6, "/imgs/eye2.gif", "/audio/woof1.wav");
    // luluCharacter->doReact(COMMAND_FULLLAYDOWN, 4, 6, "/imgs/eye6.gif", "/audio/woof3.wav");
    // luluCharacter->doReact(COMMAND_LAYDOWN, 4, 6, "/imgs/eye2.gif", "/audio/woof1.wav");
    // jsRunner->jsEvalFile("/js/demo.js");

    // jsRunner->jsEval(
    //     "for (var i=0;i<5;i++){ \
    //         log_d('Hello, world!'); \
    //         delay(2000); \
    //      }"
    // );
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
    jsRunner->jsEvalFile("/js/script1.js");
}

void LuLuDog::Action2()
{
    ExitMenu();
    jsRunner->jsEvalFile("/js/script2.js");
}

void LuLuDog::Action3()
{
    ExitMenu();
    jsRunner->jsEvalFile("/js/script3.js");
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
    audioHelper->PlayWav("/audio/awoof1.wav");
    displayHelper->PlayGif("/imgs/eye5.gif");
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
