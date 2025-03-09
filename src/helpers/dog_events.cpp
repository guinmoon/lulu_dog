#include <Arduino.h>
#include "dog_events.hpp"
#include "lulu_dog.h"
#include "eyes_drawer.h"

DogEvents::DogEvents(LuLuDog *_luluDog)
{
    this->luluDog = _luluDog;
}

DogEvent *DogEvents::BuildTouchEvent(int x, int y, int8_t touchCount)
{
    DogEvent *event = new DogEvent();
    TouchEvent *touchArgs = new TouchEvent();

    touchArgs->x = x;
    touchArgs->y = y;
    touchArgs->touchCount = touchCount;

    event->eventType = TOUCH;
    event->eventArgs = touchArgs;

    return event;
}

DogEvent *DogEvents::BuildAccEvent(float deltaX, float deltaY, float deltaZ, int direction)
{
    DogEvent *event = new DogEvent();
    AccelerometerEvent *accelerometerArgs = new AccelerometerEvent();

    accelerometerArgs->deltaX = deltaX;
    accelerometerArgs->deltaY = deltaY;
    accelerometerArgs->deltaZ = deltaZ;
    accelerometerArgs->direction = direction;

    event->eventType = ACCELEROMETER;
    event->eventArgs = accelerometerArgs;

    return event;
}

DogEvent *DogEvents::BuildGyroEvent(float deltaX, float deltaY, float deltaZ, int direction)
{
    DogEvent *event = new DogEvent();
    GyroEvent *gyroArgs = new GyroEvent();

    gyroArgs->deltaX = deltaX;
    gyroArgs->deltaY = deltaY;
    gyroArgs->deltaZ = deltaZ;
    gyroArgs->direction = direction;

    event->eventType = GYRO;
    event->eventArgs = gyroArgs;

    return event;
}

void DogEvents::EmitDogEvent(DogEvent *e)
{
    if (e == NULL || (e->eventArgs == NULL && e->eventType == NONE))
    {
        return;
    }
    switch (DogEventType(e->eventType))
    {
    case TOUCH:
        OnTouchEvent((TouchEvent *)e->eventArgs);
        break;
    case ACCELEROMETER:
        OnAccelerometerEvent((AccelerometerEvent *)e->eventArgs);
        break;
    case GYRO:
        OnGyroEvent((GyroEvent *)e->eventArgs);
        break;

    default:
        break;
    }
}

void DogEvents::EmitDogEvent(DogEvent *e1, DogEvent *e2)
{
    if (e1 == NULL && e2 == NULL)
    {
        return;
    }
    if (e1->eventType == ACCELEROMETER && e2->eventType == GYRO)
    {
        OnAccelerometerAndGyroEvent(
            (AccelerometerEvent *)e1->eventArgs,
            (GyroEvent *)e2->eventArgs);
        return;
    }
}

void DogEvents::OnTouchEvent(TouchEvent *args)
{
    if (args == NULL)
    {
        return;
    }
    OnExternalImpact();
    log_d("%i %i %i", args->x, args->y, args->touchCount);
    if (args->touchCount == 1)
    {
        luluDog->luluCharacter->doRandomReactGif(-1,true);
        log_d("Single Touch Detected");
    }
    if (args->touchCount == 2)
    {
        log_d("Double Touch Detected");
        luluDog->ShowMenu();
    }
    if (args->touchCount == LONG_PRESS_T_COUNT)
    {
        log_d("Long Press Detected");
        luluDog->displayHelper->StopGif();
        luluDog->displayHelper->pauseResumeEyes(false);
        luluDog->displayHelper->luluEyes->setMood(0);
        // luluDog->displayHelper->StopGif();
        // luluDog->displayHelper->ShowMatrixAnimation();
    }
    if (args->touchCount == LONG_PRESS_T_END_COUNT)
    {
        log_d("Long Press Detected");        
        luluDog->displayHelper->pauseResumeEyes(true);
        luluDog->luluCharacter->doRandomReactGif(-1,false);
        // luluDog->displayHelper->luluEyes->setMood(HAPPY);
    }
}

void DogEvents::OnAccelerometerEvent(AccelerometerEvent *e)
{
    if (e == NULL)
    {
        return;
    }
    log_d("\nACC: %f %f %f \nD: %i", e->deltaX, e->deltaY, e->deltaZ, e->direction);
    delay(200);
    if (!luluDog->touchHelper->released || !luluDog->gyroHelper->gyroActive)
        return;
    OnGyroOrAccEvent();
    OnExternalImpact();
    // luluDog->luluCharacter->doRandomReact(-1);
}

void DogEvents::OnGyroEvent(GyroEvent *e)
{
    if (e == NULL)
    {
        return;
    }
    log_d("\nGYRO: %f %f %f\nD: %i", e->deltaX, e->deltaY, e->deltaZ, e->direction);
    delay(200);
    if (!luluDog->touchHelper->released || !luluDog->gyroHelper->gyroActive)
        return;
    OnGyroOrAccEvent();
    OnExternalImpact();
}

void DogEvents::OnAccelerometerAndGyroEvent(AccelerometerEvent *accE, GyroEvent *gyroE)
{
    if (accE == NULL && gyroE == NULL)
    {
        return;
    }
    OnGyroOrAccEvent();
    log_d("\nACC: %f %f %f \nD: %i + \nGYRO: %f %f %f\nD: %i",
          accE->deltaX, accE->deltaY, accE->deltaZ, accE->direction,
          gyroE->deltaX, gyroE->deltaY, gyroE->deltaZ, gyroE->direction);
    delay(200);
    if (!luluDog->touchHelper->released || !luluDog->gyroHelper->gyroActive)
        return;
    OnExternalImpact();
    if (accE->direction == GYRO_D_RIGHT &&
        (gyroE->direction == GYRO_D_TILT_BACKWARD || gyroE->direction == GYRO_D_ROTATE_LEFT))
    {
        luluDog->luluCharacter->RightHand();
    }
    if (accE->direction == GYRO_D_LEFT &&
        (gyroE->direction == GYRO_D_TILT_FORWARD || gyroE->direction == GYRO_D_ROTATE_RIGHT))
    {
        luluDog->luluCharacter->LeftHand();
    }
    // luluDog->luluCharacter->doRandomReact(-1);
}

void DogEvents::OnGyroOrAccEvent()
{
    if (luluDog->displayHelper->showMatrixAnimation)
    {
        luluDog->displayHelper->StopMatrixAnimation();
        // luluDog->displayHelper->PlayGif("/imgs/eye5.gif");
    }
}

void DogEvents::OnExternalImpact()
{
    int current_time = millis();
    lastImpact = current_time;
    if (!eventsSuspended)
        Wake();

    // if (current_time - lastImpact < LAST_IMPACT_MIN_PERIOD || suspended)
    // {
    //     lastImpact = current_time;
    //     return;
    // }
}

void DogEvents::Wake()
{
    if (sleeping)
    {
        sleeping = false;
        log_d("WAKE");
        luluDog->displayHelper->stopSleepAnimation();
        delay(200);
        // SendCommand(RP_SYS_COMMAND_WAKE,0);
    }
}

void DogEvents::SleepPrepare()
{

    luluDog->luluCharacter->SendCommand(COMMAND_SET_TAIL_SPEED, 0);
    log_d("SLEEP");
    luluDog->displayHelper->StopGif();
    delay(1000);
    luluDog->luluCharacter->SendCommand(COMMAND_SET_TAIL_SPEED, 0);
}

void DogEvents::GoToDeepSleep()
{
    if (!DEEP_SLEEP_ON)
        return;
    log_d("PREPARE to SLEEP: %i ms", 2000);
    sleeping = true;
    deepSleeping = true;
    SleepPrepare();
    gpio_deep_sleep_hold_en();
    gpio_hold_en((gpio_num_t)SYS_EN_PIN);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)TP_INT, 0);
    esp_deep_sleep_start();
    log_d("SLEEPING FOR: %i ms", 2000);
}

void DogEvents::GoToSleep()
{
    if (!SLEEP_ON)
        return;
    SleepPrepare();
    sleeping = true;
    luluDog->displayHelper->showSleepAnimation();
}

void DogEvents::DogActivitiWatcherTask()
{
    while (true)
    {
        if (eventsSuspended)
        {
            lastImpact = millis();
            delay(1000);
            continue;
        }
        if ((millis() - lastImpact) / 1000 >= SLEEP_AFTER && !sleeping)
        {
            GoToSleep();
        }
        if ((millis() - lastImpact) / 1000 >= DEEP_SLEEP_AFTER && !luluDog->batteryHelper->isCharging())
        {
            GoToDeepSleep();
        }

        delay(1000);
    }
}

void DogEvents::DogActivitiWatcherThread(void *_this)
{
    ((DogEvents *)_this)->DogActivitiWatcherTask();
    vTaskDelete(NULL);
}

void DogEvents::RP2040PingTask()
{
    while (true)
    {
        if (!pingPaused)
            luluDog->luluCharacter->SendCommand(RP_SYS_COMMAND_PING, 0);
        delay(2000);
    }
}

void DogEvents::RP2040PingThread(void *_this)
{
    ((DogEvents *)_this)->RP2040PingTask();
    vTaskDelete(NULL);
}

void DogEvents::StartDogActivitiWatcher()
{

    xTaskCreatePinnedToCore(
        DogActivitiWatcherThread, /* Task function. */
        "Task7",                  /* name of task. */
        4096,                     /* Stack size of task */
        this,                     /* parameter of the task */
        tskIDLE_PRIORITY,         /* priority of the task */
        NULL,                     /* Task handle to keep track of created task */
        0);
}

void DogEvents::StartSlavePingThread()
{
    xTaskCreatePinnedToCore(
        RP2040PingThread, /* Task function. */
        "Task11",         /* name of task. */
        4096,             /* Stack size of task */
        this,             /* parameter of the task */
        tskIDLE_PRIORITY, /* priority of the task */
        NULL,             /* Task handle to keep track of created task */
        0);
}

void DogEvents::OnLongPressChPosition(int x, int y)
{
    // log_d("long press at %d,%d ", x, y);
    luluDog->displayHelper->SetEyePosition(x-40, y-40);
}

