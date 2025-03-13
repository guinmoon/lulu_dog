#ifndef i2C_SLAVE_HELPER_H
#define i2C_SLAVE_HELPER_H

#include "Arduino.h"
#include "global_def.h"
#include <mutex>
#include <Wire.h>

class LuLuDog;

class I2CSlaveHeler
{

private:
    LuLuDog *luluDog;
    std::mutex i2c_mutex;

public:
    I2CSlaveHeler(LuLuDog *luluDog)
    {
        this->luluDog = luluDog;
    }

    void sendCommand(int command);

    void SendCommand(int command, int arg1);

    void ConfirmCommand(int command, int arg1);

    static void reciveThread(void *_this);

    uint8_t requestI2CByte();

    void reciveTask();

    ~I2CSlaveHeler();
};

#endif