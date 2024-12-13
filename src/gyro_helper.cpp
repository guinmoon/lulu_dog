#include "pin_config.h"
#include "gyro_helper.h"
#include "character.h"
#include "SensorQMI8658.hpp"

SensorQMI8658 qmi;

IMUdata acc;
IMUdata gyr;

// Дополнительные переменные для отслеживания изменений ускорения и угловой скорости
IMUdata prevAcc = {0, 0, 0}; // Предыдущее значение ускорений
IMUdata prevGyr = {0, 0, 0}; // Предыдущее значение угловых скоростей

const float impactThresholdAcc = 0.2; // Пороговое значение изменений ускорения (низкое из-за минимальных изменений)
const float impactThresholdGyr = 0.9; // Пороговое значение изменений угловой скорости

unsigned long lastGyroActionTime = 0;        // Время последнего вызова doOnGyro
const unsigned long gyroActionPeriod = 2000; // Время в миллисекундах
bool gyroActionFirstTime = true;

int direction = 0;

void doOnGyro()
{
    // Реализация вашей функции, вызываемой при обнаружении удара
    log_d("doOnGyro called!");
    doRandomReact();
}

bool gyroInit()
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
        gyroAndAccelReadTask, /* Task function. */
        "Task3",              /* name of task. */
        10000,                /* Stack size of task */
        NULL,                 /* parameter of the task */
        3,                    /* priority of the task */
        NULL,                 /* Task handle to keep track of created task */
        1);

    return true;
}

void gyroAndAccelReadTask(void *params)
{
    while (true)
    {

        if (qmi.getDataReady())
        {
            bool impactDetected = false;

            if (qmi.getAccelerometer(acc.x, acc.y, acc.z))
            {

                // Вычисление изменения ускорения
                float deltaX = abs(acc.x - prevAcc.x);
                float deltaY = abs(acc.y - prevAcc.y);
                float deltaZ = abs(acc.z - prevAcc.z);

                // Проверка изменений ускорения
                // Определение направления по акселерометру
                if (deltaX > impactThresholdAcc)
                {
                    impactDetected = true;
                    direction = (acc.x - prevAcc.x > 0) ? GYRO_D_RIGHT : GYRO_D_LEFT;
                }
                if (deltaY > impactThresholdAcc)
                {
                    impactDetected = true;
                    direction = (acc.y - prevAcc.y > 0) ? GYRO_D_FORWARD: GYRO_D_BACKWARD;
                }
                if (deltaZ > impactThresholdAcc)
                {
                    impactDetected = true;
                    direction = (acc.z - prevAcc.z > 0) ? GYRO_D_UP : GYRO_D_DOWN;
                }

                // Сохранение текущего значения как предыдущее для следующей итерации
                prevAcc = acc;
            }

            if (qmi.getGyroscope(gyr.x, gyr.y, gyr.z))
            {
                float deltaGyrX = abs(gyr.x - prevGyr.x);
                float deltaGyrY = abs(gyr.y - prevGyr.y);
                float deltaGyrZ = abs(gyr.z - prevGyr.z);
                // Вычисление изменения угловой скорости
                if (deltaGyrX > impactThresholdGyr)
                {
                    impactDetected = true;
                    direction = (gyr.x - prevGyr.x > 0) ? GYRO_D_ROTATE_RIGHT : GYRO_D_ROTATE_LEFT;
                }
                if (deltaGyrY > impactThresholdGyr)
                {
                    impactDetected = true;
                    direction = (gyr.y - prevGyr.y > 0) ? GYRO_D_TILT_FORWARD : GYRO_D_TILT_BACKWARD;
                }
                if (deltaGyrZ > impactThresholdGyr)
                {
                    impactDetected = true;
                    direction = (gyr.z - prevGyr.z > 0) ? GYRO_D_TILT_UP: GYRO_D_TILT_DOWN;
                }

                // Сохранение текущего значения как предыдущее для следующей итерации
                prevGyr = gyr;
            }

            if (impactDetected)
            {
                unsigned long currentMillis = millis();
                if (currentMillis - lastGyroActionTime >= gyroActionPeriod)
                {
                    if (gyroActionFirstTime)
                    {
                        gyroActionFirstTime = false;
                        continue;
                    }
                    log_d("Impact detected!");
                    doOnGyro();                         // Вызов функции
                    lastGyroActionTime = currentMillis; // Обновление времени последнего вызова
                }

                // Здесь можно вызвать функцию для управления роботом, например, выполнить анимацию.
            }
        }

        delay(100); // Пауза для снижения частоты опроса
    }
    vTaskDelete(NULL);
}