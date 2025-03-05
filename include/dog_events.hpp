#ifndef DOG_EVENTS_H
#define DOG_EVENTS_H

#include <stdint.h>

class LuLuDog;

enum DogEventType {
    NONE = 0,
    TOUCH,
    GYRO, 
    ACCELEROMETER,
    RTC,
    VOLTAGE,
    BUTTON,
    TOUCH_BUTTON
};


struct TouchEvent {
        int x;
        int y;
        int8_t touchCount;
};

struct GyroEvent {
    float deltaX;
    float deltaY;
    float deltaZ;        
};

struct AccelerometerEvent {
    float deltaX;
    float deltaY;
    float deltaZ;
};

struct RtcEvent {
    int year;
    int month;
    int day;
    int hour;
    int minute;
};

struct VoltageEvent {
    float voltage;
};

struct ButtonEvent {
    int button;
    int8_t pressCount;
};


struct DogEvent {     
    void * eventArgs;
    DogEventType eventType;
};

class DogEvents {
    public:
        DogEvents(LuLuDog *_luluDog);
        ~DogEvents();
        void OnDogEvent(DogEvent *e);

    private:
        LuLuDog *luluDog;
};

#endif