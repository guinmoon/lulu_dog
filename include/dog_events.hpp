#ifndef DOG_EVENTS_H
#define DOG_EVENTS_H

#include <stdint.h>

class LuLuDog;

enum DogEventType
{
    NONE = 0,
    TOUCH = 1,
    GYRO = 2,
    ACCELEROMETER = 3,
    RTC = 4,
    VOLTAGE = 5,
    BUTTON = 6,
    TOUCH_BUTTON = 7
};


struct TouchEvent
{
    int x;
    int y;
    int8_t touchCount;
};

struct GyroEvent
{
    float deltaX;
    float deltaY;
    float deltaZ;
    int direction;
};

struct AccelerometerEvent
{
    float deltaX;
    float deltaY;
    float deltaZ;
    int direction;
};

// struct AccelerometerGyroEvent
// {
//     float deltaAccX;
//     float deltaAccY;
//     float deltaAccZ;
//     int directionAcc;
//     float deltaGyroX;
//     float deltaGyroY;
//     float deltaGyroZ;
//     int directionGyro;
// };

struct RtcEvent
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
};

struct VoltageEvent
{
    float voltage;
};

struct ButtonEvent
{
    int button;
    int8_t pressCount;
};

struct DogEvent
{
    void *eventArgs;
    DogEventType eventType;
};

class DogEvents
{
public:
    unsigned long lastImpact = millis();
    bool deepSleeping = false;
    bool sleeping = false;
    bool pingPaused = false;
    bool eventsSuspended = false;

    static void DogActivitiWatcherThread(void *args);
    static void RP2040PingThread(void *args);
    void Wake();    
    void SleepPrepare();
    void GoToDeepSleep();
    void GoToSleep();    
    void StartDogActivitiWatcher();
    void RP2040PingTask();
    void DogActivitiWatcherTask();
    void StartSlavePingThread();

    DogEvent *BuildTouchEvent(int x, int y, int8_t touchCount);
    DogEvent *BuildAccEvent(float deltaX, float deltaY, float deltaZ, int direction);
    DogEvent *BuildGyroEvent(float deltaX, float deltaY, float deltaZ, int direction);
    DogEvents(LuLuDog *_luluDog);
    ~DogEvents();
    void EmitDogEvent(DogEvent *e);
    void EmitDogEvent(DogEvent *e1, DogEvent *e2);
    void OnTouchEvent(TouchEvent *args);
    void OnAccelerometerEvent(AccelerometerEvent *args);
    void OnGyroEvent(GyroEvent *args);
    void OnAccelerometerAndGyroEvent(AccelerometerEvent *accE, GyroEvent *gyroE);
    void OnExternalImpact();

private:
    LuLuDog *luluDog;
};

#endif