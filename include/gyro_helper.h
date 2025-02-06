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
#define GYRO_D_TILT_FORWARD 9
#define GYRO_D_TILT_BACKWARD 10
#define GYRO_D_TILT_UP 11
#define GYRO_D_TILT_DOWN 12

class LuLuDog;

class GyroHelper
{
private:
    SensorQMI8658 qmi;

    IMUdata acc;
    IMUdata gyr;

    // Дополнительные переменные для отслеживания изменений ускорения и угловой скорости
    IMUdata prevAcc = {0, 0, 0}; // Предыдущее значение ускорений
    IMUdata prevGyr = {0, 0, 0}; // Предыдущее значение угловых скоростей

    const float impactThresholdAcc = IMPACT_THRESHHOLD_ACC; // Пороговое значение изменений ускорения (низкое из-за минимальных изменений)
    const float impactThresholdGyr = IMPACT_THRESHHOLD_GYR; // Пороговое значение изменений угловой скорости

    unsigned long lastGyroActionTime = 0;                      // Время последнего вызова doOnGyro
    const unsigned long gyroActionPeriod = GYRO_ACTION_PERIOD; // Время в миллисекундах

    bool gyroActionFirstTime = true;
    bool gyroActive = true;

    

    int direction = 0;
    LuLuDog* luluDog;

public:
    GyroHelper(LuLuDog* _luluDog);
    void doOnGyro(int direction);
    bool InitGyro();
    void PauseGyro();
    void ResumeGyro();
    static void gyroAndAccelReadThread(void *params);
    void gyroAndAccelReadTask();
    int gyroDelay = 100;
};


// extern GyroHelper gyroHelper;

#endif