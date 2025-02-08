#include "gyro_helper.h"
#include "SensorQMI8658.hpp"
#include "lulu_dog.h"
#include "global_def.h"

// SensorQMI8658 qmi;

// IMUdata acc;
// IMUdata gyr;

// // Дополнительные переменные для отслеживания изменений ускорения и угловой скорости
// IMUdata prevAcc = {0, 0, 0}; // Предыдущее значение ускорений
// IMUdata prevGyr = {0, 0, 0}; // Предыдущее значение угловых скоростей

// const float impactThresholdAcc = IMPACT_THRESHHOLD_ACC; // Пороговое значение изменений ускорения (низкое из-за минимальных изменений)
// const float impactThresholdGyr = IMPACT_THRESHHOLD_GYR; // Пороговое значение изменений угловой скорости

// unsigned long lastGyroActionTime = 0;                      // Время последнего вызова doOnGyro
// const unsigned long gyroActionPeriod = GYRO_ACTION_PERIOD; // Время в миллисекундах

// bool gyroActionFirstTime = true;

// int direction = 0;

GyroHelper::GyroHelper(LuLuDog* _luluDog){
    this->luluDog = _luluDog;
}

void GyroHelper::doOnGyro(int direction)
{
    // Реализация вашей функции, вызываемой при обнаружении удара
    log_d("doOnGyro called!");
    delay(200);
    if (!luluDog->touchHelper->released || !gyroActive)
        return;
    luluDog->luluCharacter->doRandomReact(direction);
}

void GyroHelper::PauseGyro(){
    gyroActive = false;
}

void GyroHelper::ResumeGyro(){
    gyroActive = true;
}

bool GyroHelper::InitGyro()
{
    if (!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, IIC_SDA, IIC_SCL))
    {

        log_d("QMI8658_L_SLAVE_ADDRESS error: %d");
        delay(1000);
    }

    qmi.configAccelerometer(
        SensorQMI8658::ACC_RANGE_4G,
        SensorQMI8658::ACC_ODR_1000Hz,
        SensorQMI8658::LPF_MODE_0);

    qmi.configGyroscope(
        SensorQMI8658::GYR_RANGE_64DPS,
        SensorQMI8658::GYR_ODR_896_8Hz,
        SensorQMI8658::LPF_MODE_3);

    qmi.enableGyroscope();
    qmi.enableAccelerometer();

    qmi.dumpCtrlRegister();

    xTaskCreatePinnedToCore(
        this->gyroAndAccelReadThread, /* Task function. */
        "Task3",              /* name of task. */
        10000,                /* Stack size of task */
        this,                 /* parameter of the task */
        tskIDLE_PRIORITY,     /* priority of the task */
        NULL,                 /* Task handle to keep track of created task */
        1);

    return true;
}

void GyroHelper::gyroAndAccelReadThread(void *_this)
{
    ((GyroHelper *)_this)->gyroAndAccelReadTask();
    vTaskDelete(NULL);
}

void GyroHelper::gyroAndAccelReadTask()
{
    while (true)
    {
        if (!gyroActive){
            delay(300);
            continue;
        }
        if (qmi.getDataReady())
        {
            bool impactDetected = false;
            int direction = GYRO_D_NONE;

            if (qmi.getAccelerometer(acc.x, acc.y, acc.z))
            {
                // Вычисление изменений акселерометра
                float deltaX = acc.x - prevAcc.x;
                float deltaY = acc.y - prevAcc.y;
                float deltaZ = acc.z - prevAcc.z;

                // Определение направления по акселерометру
                if (abs(deltaX) > impactThresholdAcc)
                {
                    impactDetected = true;
                    direction = (deltaX > 0) ? GYRO_D_RIGHT : GYRO_D_LEFT;
                }
                else if (abs(deltaY) > impactThresholdAcc)
                {
                    impactDetected = true;
                    direction = (deltaY > 0) ? GYRO_D_FORWARD : GYRO_D_BACKWARD;
                }
                else if (abs(deltaZ) > impactThresholdAcc)
                {
                    impactDetected = true;
                    direction = (deltaZ > 0) ? GYRO_D_UP : GYRO_D_DOWN;
                }

                prevAcc = acc;
            }

            if (qmi.getGyroscope(gyr.x, gyr.y, gyr.z))
            {
                // Вычисление изменений гироскопа
                float deltaGyrX = gyr.x - prevGyr.x;
                float deltaGyrY = gyr.y - prevGyr.y;
                float deltaGyrZ = gyr.z - prevGyr.z;

                // Определение направления по гироскопу
                if (abs(deltaGyrX) > impactThresholdGyr)
                {
                    impactDetected = true;
                    direction = (deltaGyrX > 0) ? GYRO_D_ROTATE_RIGHT : GYRO_D_ROTATE_LEFT;
                }
                else if (abs(deltaGyrY) > impactThresholdGyr)
                {
                    impactDetected = true;
                    direction = (deltaGyrY > 0) ? GYRO_D_TILT_FORWARD : GYRO_D_TILT_BACKWARD;
                }
                else if (abs(deltaGyrZ) > impactThresholdGyr)
                {
                    impactDetected = true;
                    direction = (deltaGyrZ > 0) ? GYRO_D_TILT_UP : GYRO_D_TILT_DOWN;
                }

                prevGyr = gyr;
            }

            if (impactDetected)
            {
                unsigned long currentMillis = millis();
                if (currentMillis - lastGyroActionTime >= gyroActionPeriod)
                {
                    doOnGyro(direction);
                    log_d("Impact %i detected! ", direction); // Вызов функции с указанием направления
                    lastGyroActionTime = currentMillis;       // Обновление времени последнего вызова
                }
            }
        }

        delay(gyroDelay); // Пауза для снижения частоты опроса
    }
}

// GyroHelper gyroHelper;