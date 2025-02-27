#ifndef DOG_EVENTS_H
#define DOG_EVENTS_H

#include <stdint.h>

class LuLuDog;

enum DogEventType {
    DOG_EVENT_TYPE_NONE,
    DOG_EVENT_TYPE_TOUCH,
    DOG_EVENT_TYPE_GYRO,
    DOG_EVENT_TYPE_ACCELEROMETER,
    DOG_EVENT_TYPE_RTC,
    DOG_EVENT_TYPE_VOLTAGE,
    DOG_EVENT_TYPE_BUTTON,
    DOG_EVENT_TYPE_TOUCH_BUTTON
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