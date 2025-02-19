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

SensorQMI8658 GyroHelper::qmi;

GyroHelper::GyroHelper(LuLuDog *_luluDog)
{
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

void GyroHelper::PauseGyro()
{
    gyroActive = false;
}

void GyroHelper::ResumeGyro()
{
    gyroActive = true;
}

bool GyroHelper::InitGyro()
{
    if (!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, IIC_SDA, IIC_SCL))
    {

        log_d("QMI8658_L_SLAVE_ADDRESS error: %d");
        delay(1000);
    }

    //  CALIBRATION
    // uint16_t gX_gain = 0, gY_gain = 0, gZ_gain = 0;
    // bool ret = false;
    // while (!ret)
    // {

    //     // Calibrate only once, do not obtain calibration value
    //     // result = qmi.calibration();

    //     // Get the calibration value after calibration
    //     ret = qmi.calibration(&gX_gain, &gY_gain, &gZ_gain);
    //     if (ret)
    //     {
    //         log_d("All calibrations are completed");
    //         break;
    //     }
    //     int i = 5;
    //     while (i--)
    //     {
    //         log_d("Calibration failed, please leave the sensor alone and keep quiet! , Will try again in %i seconds", i);
    //     }
    // }
    // log_d("Calibration values: X(%i) Y(%i) Z(%i)", gX_gain, gY_gain, gZ_gain);

    uint16_t gX_gain = 9369, gY_gain = 11435, gZ_gain = 10837;
    // //  X(9378) Y(11389) Z(10837)
    // // X(9369) Y(11435) Z(10837)
    qmi.writeCalibration(gX_gain, gY_gain, gZ_gain);
    /////

    // qmi.configAccelerometer(SensorQMI8658::ACC_RANGE_4G, SensorQMI8658::ACC_ODR_500Hz);

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
        "Task3",                      /* name of task. */
        10000,                        /* Stack size of task */
        this,                         /* parameter of the task */
        tskIDLE_PRIORITY,             /* priority of the task */
        NULL,                         /* Task handle to keep track of created task */
        1);

    return true;
}

void GyroHelper::gyroAndAccelReadThread(void *_this)
{
    ((GyroHelper *)_this)->gyroAndAccelReadTask();
    vTaskDelete(NULL);
}

uint8_t priority = SensorQMI8658::PRIORITY0; //(X > Y> Z)
//* Defines the maximum duration (in sample) for a valid peak.
//* In a valid peak, the linear acceleration should reach or be higher than the PeakMagThr
//* and should return to quiet (no significant movement) within UDMThr, at the end of PeakWindow.
uint8_t peakWindow = 20; // 20 @500Hz ODR
//* Defines the minimum quiet time before the second Tap happen.
//* After the first Tap is detected, there should be no significant movement (defined by UDMThr) during the TapWindow.
//* The valid second tap should be detected after TapWindow and before DTapWindow.
uint16_t tapWindow = 50; // 50 @500Hz ODR
//* Defines the maximum time for a valid second Tap for Double Tap,
//* count start from the first peak of the valid first Tap.
uint16_t dTapWindow = 250; // 250 @500Hz ODR
//* Defines the ratio for calculating the average of the movement
//* magnitude. The bigger of Gamma, the bigger weight of the latest  data.
float alpha = 0.0625;
//* Defines the ratio for calculating the average of the movement
//* magnitude. The bigger of Gamma, the bigger weight of the latest data.
float gammma = 0.25;
//* Threshold for peak detection.
float peakMagThr = 0.8; // 0.8g square
//* Undefined Motion threshold. This defines the threshold of the
//* Linear Acceleration for quiet status.
float UDMTh = 0.4; // 0.4g square

uint8_t modeCtrl = SensorQMI8658::ANY_MOTION_EN_X |
                   SensorQMI8658::ANY_MOTION_EN_Y |
                   SensorQMI8658::ANY_MOTION_EN_Z |
                   SensorQMI8658::NO_MOTION_EN_X |
                   SensorQMI8658::NO_MOTION_EN_Y |
                   SensorQMI8658::NO_MOTION_EN_Z;

//* Define the slope threshold of the x-axis for arbitrary motion detection
float AnyMotionXThr = 100.0; //  x-axis 100mg threshold
//* Define the slope threshold of the y-axis for arbitrary motion detection
float AnyMotionYThr = 100.0; //  y-axis 100mg threshold
//* Define the slope threshold of the z-axis for arbitrary motion detection
float AnyMotionZThr = 1.0; //  z-axis 1mg threshold
//* Defines the minimum number of consecutive samples (duration) that the absolute
//* of the slope of the enabled axis/axes data should keep higher than the threshold
uint8_t AnyMotionWindow = 1; //  1 samples

// TODO: No motion detection does not work
//* Defines the slope threshold of the x-axis for no motion detection
float NoMotionXThr = 0.1;
//* Defines the slope threshold of the y-axis for no motion detection
float NoMotionYThr = 0.1;
//* Defines the slope threshold of the z-axis for no motion detection
float NoMotionZThr = 0.1;

//* Defines the minimum number of consecutive samples (duration) that the absolute
//* of the slope of the enabled axis/axes data should keep lower than the threshold
uint8_t NoMotionWindow = 1; //  1 samples
//* Defines the wait window (idle time) starts from the first Any-Motion event until
//* starting to detecting another Any-Motion event form confirmation
uint16_t SigMotionWaitWindow = 1; //  1 samples
//* Defines the maximum duration for detecting the other Any-Motion
//* event to confirm Significant-Motion, starts from the first Any -Motion event
uint16_t SigMotionConfirmWindow = 1; //

void GyroHelper::tapEventCallback()
{
    SensorQMI8658::TapEvent event = qmi.getTapStatus();
    // SensorQMI8658::MotionEvent event = qmi.get
    // SensorQMI8658::MOTION_SIGNIFICANT
    switch (event)
    {
    case SensorQMI8658::SINGLE_TAP:
        log_d("Single-TAP");
        break;
    case SensorQMI8658::DOUBLE_TAP:
        log_d("Double-TAP");
        break;
    default:
        log_d("Unknown-TAP");
        break;
    }
}

void GyroHelper::significantMotionEventCallback()
{
    log_d("significantMotionEventCallback");
}

void GyroHelper::gyroAndAccelReadTask()
{

    // qmi.configTap(priority, peakWindow, tapWindow,
    //               dTapWindow, alpha, gammma, peakMagThr, UDMTh);

    // Enable the Tap Detection and enable the interrupt
    // qmi.enableTap(SensorQMI8658::INTERRUPT_PIN_1);

    // Set the Tap Detection callback function
    // qmi.setTapEventCallBack(tapEventCallback);
    
    // qmi.configMotion(modeCtrl,
    //                  AnyMotionXThr, AnyMotionYThr, AnyMotionZThr, AnyMotionWindow,
    //                  NoMotionXThr, NoMotionYThr, NoMotionZThr, NoMotionWindow,
    //                  SigMotionWaitWindow, SigMotionConfirmWindow);

    // // Enable the Motion Detection and enable the interrupt
    // // qmi.enableMotionDetect(SensorQMI8658::INTERRUPT_PIN_1);
    // qmi.enableMotionDetect(SensorQMI8658::INTERRUPT_PIN_DISABLE);

    // log_d("TAP DETECT STARTED");
    // qmi.setAnyMotionEventCallBack(significantMotionEventCallback);

    while (true)
    {
        if (!gyroActive)
        {
            delay(300);
            continue;
        }
        qmi.update();
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
                // log_d("%f %f %f",deltaX,deltaY,deltaZ);

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

            // if (qmi.getGyroscope(gyr.x, gyr.y, gyr.z))
            // {
            //     // Вычисление изменений гироскопа
            //     float deltaGyrX = gyr.x - prevGyr.x;
            //     float deltaGyrY = gyr.y - prevGyr.y;
            //     float deltaGyrZ = gyr.z - prevGyr.z;

            //     // Определение направления по гироскопу
            //     if (abs(deltaGyrX) > impactThresholdGyr)
            //     {
            //         impactDetected = true;
            //         direction = (deltaGyrX > 0) ? GYRO_D_ROTATE_RIGHT : GYRO_D_ROTATE_LEFT;
            //     }
            //     else if (abs(deltaGyrY) > impactThresholdGyr)
            //     {
            //         impactDetected = true;
            //         direction = (deltaGyrY > 0) ? GYRO_D_TILT_FORWARD : GYRO_D_TILT_BACKWARD;
            //     }
            //     else if (abs(deltaGyrZ) > impactThresholdGyr)
            //     {
            //         impactDetected = true;
            //         direction = (deltaGyrZ > 0) ? GYRO_D_TILT_UP : GYRO_D_TILT_DOWN;
            //     }

            //     prevGyr = gyr;
            // }

            if (impactDetected)
            {
                unsigned long currentMillis = millis();
                if (currentMillis - lastGyroActionTime >= gyroActionPeriod)
                {
                    log_d("Impact %i detected! ", direction); // Вызов функции с указанием направления
                    doOnGyro(direction);                    
                    lastGyroActionTime = currentMillis;       // Обновление времени последнего вызова
                    if(qmi.getAccelerometer(acc.x, acc.y, acc.z)){//записываем чтобы после детекта
                                                                //не было детекта в противоположную сторону
                        prevAcc = acc;
                    }
                }
            }
        }

        delay(gyroDelay); // Пауза для снижения частоты опроса
    }
}

// GyroHelper gyroHelper;