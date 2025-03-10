#ifndef GYRO_HELPER_H
#define GYRO_HELPER_H

#include "SensorQMI8658.hpp"
#include "global_def.h"
#include "character.h"

#define GYRO_D_NONE 0
#define GYRO_D_RIGHT 1
#define GYRO_D_LEFT 2
#define GYRO_D_FORWARD 3
#define GYRO_D_BACKWARD 4
#define GYRO_D_UP 5
#define GYRO_D_DOWN 6
#define GYRO_D_ROTATE_RIGHT 7
#define GYRO_D_ROTATE_LEFT 8
#define GYRO_D_TILT_FORWARD 9 //turn right
#define GYRO_D_TILT_BACKWARD 10 //turn left
#define GYRO_D_TILT_UP 11
#define GYRO_D_TILT_DOWN 12

// 1 + 10 || 8 dog left hand
// 2 + 9 || 7  dog right hand

class LuLuDog;

class GyroHelper
{
private:
    static SensorQMI8658 qmi;

    IMUdata acc;
    IMUdata gyr;

    // Дополнительные переменные для отслеживания изменений ускорения и угловой скорости
    IMUdata prevAcc = {0, 0, 0}; // Предыдущее значение ускорений
    IMUdata prevGyr = {0, 0, 0}; // Предыдущее значение угловых скоростей

    const float impactThresholdAccX = IMPACT_THRESHHOLD_ACC_X;
    const float impactThresholdAccY = IMPACT_THRESHHOLD_ACC_Y;
    const float impactThresholdAccZ = IMPACT_THRESHHOLD_ACC_Z;
     // Пороговое значение изменений ускорения (низкое из-за минимальных изменений)
    const float impactThresholdGyrX = IMPACT_THRESHHOLD_GYR_X;
    const float impactThresholdGyrY = IMPACT_THRESHHOLD_GYR_Y;
    const float impactThresholdGyrZ = IMPACT_THRESHHOLD_GYR_Z; // Пороговое значение изменений угловой скорости

    unsigned long lastAccActionTime = 0;
    unsigned long lastGyroActionTime = 0;                      // Время последнего вызова doOnGyro
    const unsigned long gyroActionPeriod = GYRO_ACTION_PERIOD; // Время в миллисекундах

    bool gyroActionFirstTime = true;
    

    

    int directionAcc = 0;
    int directionGyro = 0;
    LuLuDog* luluDog;

    bool _gyroResumed = false;

public:
    bool gyroActive = true;
    GyroHelper(LuLuDog* _luluDog);
    // void doOnGyro(int direction);
    bool InitGyro();
    void PauseGyro();
    void ResumeGyro();
    static void gyroAndAccelReadThread(void *params);
    void gyroAndAccelReadTask();
    static void tapEventCallback();
    static void significantMotionEventCallback();
    int gyroDelay = 50;
};


// extern GyroHelper gyroHelper;

#endif