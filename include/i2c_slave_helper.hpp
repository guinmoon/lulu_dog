#ifndef i2C_SLAVE_HELPER_H
#define i2C_SLAVE_HELPER_H

#include "Arduino.h"
#include "global_def.h"
#include <mutex>
#include <Wire.h>

class LuLuDog;

class I2CSlaveHelper
{

private:
    LuLuDog *luluDog;
    std::mutex i2c_mutex;
    bool _reciveSuspended = false;
    bool _isCommandFinished = true;

    const int N = 5;                              // Минимальное количество касаний для интенсивности
    const unsigned long maxPause = 700;           // Максимальная пауза между касаниями (в миллисекундах)
    const unsigned long doubleTapThreshold = 300; // Пауза для двойного нажатия (в миллисекундах)

    unsigned long lastTouchTime = 0; // Время последнего касания
    int slaveTouchCount = 0;         // Количество касаний

    // Флаг для отслеживания потенциального двойного нажатия
    bool potentialDoubleTap = false;

public:
    bool IsCommandFinished()
    {
        return _isCommandFinished;
    }

    I2CSlaveHelper(LuLuDog *luluDog)
    {
        this->luluDog = luluDog;
    }

    void sendCommand(int command);

    void SendCommand(int command, int arg1);

    void ConfirmCommand(int command, int arg1);

    static void commandConfirmThread(void *_this);

    // uint8_t requestI2CByte();

    void commandConfirmTask();

    void StartReciveThread();
    static void I2CReciveThread(void *_this);
    void I2CReciveTask();

    void OnSlaveTouchEvent();

    ~I2CSlaveHelper();
};

#endif