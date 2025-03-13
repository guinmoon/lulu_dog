#include "i2c_slave_helper.hpp"
#include "lulu_dog.h"

void I2CSlaveHeler::sendCommand(int command)
{
    WIRE.beginTransmission(LULU_SLAVE_ADDRESS); // Адрес ведомого устройства
    WIRE.write(command);
    WIRE.endTransmission();
    log_d("Sended: %i", command);
}

void I2CSlaveHeler::SendCommand(int command, int arg1)
{
    std::lock_guard<std::mutex> lck(i2c_mutex);
    WIRE.beginTransmission(LULU_SLAVE_ADDRESS); // Адрес ведомого устройства
    WIRE.write(command);
    WIRE.write(arg1);
    WIRE.endTransmission();
    if (command < 100)
    { // Not system command
        luluDog->lastAction = command;
    }
    log_d("Sended %i:%i", command, arg1);
}

void I2CSlaveHeler::ConfirmCommand(int command, int arg1)
{
    xTaskCreatePinnedToCore(
        reciveThread,     /* Task function. */
        "Task11",         /* name of task. */
        4096,             /* Stack size of task */
        this,             /* parameter of the task */
        tskIDLE_PRIORITY, /* priority of the task */
        NULL,             /* Task handle to keep track of created task */
        0);
}

void I2CSlaveHeler::reciveThread(void *_this)
{
    ((I2CSlaveHeler *)_this)->reciveTask();
    vTaskDelete(NULL);
}

uint8_t I2CSlaveHeler::requestI2CByte()
{
    auto res = WIRE.requestFrom(LULU_SLAVE_ADDRESS, 1);
    if (res == 0)
    {
        log_e("I2C not Received");
        luluDog->displayHelper->setIdleMode(true);
        delay(2500);
        luluDog->gyroHelper->ResumeGyro();
        return 0;
    }
    uint8_t buf;
    int commandId = WIRE.readBytes(&buf, 1);
    log_d("I2CReceive: %d", buf);
    return buf;
}

void I2CSlaveHeler::reciveTask()
{
    delay(1500);
    uint8_t response = requestI2CByte();

    if (response == 3)
    {
        while (response == 3)
        {
            delay(1000);
            uint8_t response = requestI2CByte();
        }
    }

    if (response == 4)
    {
        luluDog->displayHelper->setIdleMode(true);
        luluDog->gyroHelper->ResumeGyro();
    }
}