#include "i2c_slave_helper.hpp"
#include "lulu_dog.h"

void I2CSlaveHelper::sendCommand(int command)
{
    WIRE.beginTransmission(LULU_SLAVE_ADDRESS); // Адрес ведомого устройства
    WIRE.write(command);
    WIRE.endTransmission();
    log_d("Sended: %i", command);
}

void I2CSlaveHelper::SendCommand(int command, int arg1)
{
    std::lock_guard<std::mutex> lck(i2c_mutex);
    WIRE.beginTransmission(LULU_SLAVE_ADDRESS); // Адрес ведомого устройства
    WIRE.write(command);
    WIRE.write(arg1);
    WIRE.endTransmission();
    if (command < 100)
    { // Not system command
        _isCommandFinished = false;
        luluDog->lastAction = command;
    }
    log_d("Sended %i:%i", command, arg1);
}

void I2CSlaveHelper::ConfirmCommand(int command, int arg1)
{
    xTaskCreatePinnedToCore(
        commandConfirmThread,     /* Task function. */
        "Task11",         /* name of task. */
        4096,             /* Stack size of task */
        this,             /* parameter of the task */
        tskIDLE_PRIORITY, /* priority of the task */
        NULL,             /* Task handle to keep track of created task */
        0);
}

void I2CSlaveHelper::commandConfirmThread(void *_this)
{
    ((I2CSlaveHelper *)_this)->commandConfirmTask();
    vTaskDelete(NULL);
}



// uint8_t I2CSlaveHelper::requestI2CByte()
// {
//     auto res = WIRE.requestFrom(LULU_SLAVE_ADDRESS, 1);
//     if (res == 0)
//     {
//         log_e("I2C not Received");
//         luluDog->displayHelper->setIdleMode(true);
//         delay(2500);
//         luluDog->gyroHelper->ResumeGyro();
//         return 0;
//     }
//     uint8_t buf;
//     int commandId = WIRE.readBytes(&buf, 1);
//     log_d("I2CReceive: %d", buf);
//     return buf;
// }

void I2CSlaveHelper::StartReciveThread(){
    xTaskCreatePinnedToCore(
        I2CReciveThread,     /* Task function. */
        "Task2icRecive",         /* name of task. */
        4096,             /* Stack size of task */
        this,             /* parameter of the task */
        tskIDLE_PRIORITY, /* priority of the task */
        NULL,             /* Task handle to keep track of created task */
        0);
}

void I2CSlaveHelper::I2CReciveThread(void *_this){
    ((I2CSlaveHelper *)_this)->I2CReciveTask();
    vTaskDelete(NULL);
}

void I2CSlaveHelper::I2CReciveTask(){
    unsigned long currentTime = millis();
    while (!_reciveSuspended){

        delay(50);
        auto res = WIRE.requestFrom(LULU_SLAVE_ADDRESS, 1);
        if (res == 0)
        {            
            _isCommandFinished = false;
            delay(200);
            continue;
        }
        uint8_t commandId;
        int recived = WIRE.readBytes(&commandId, 1);
        if (commandId == 4){
            _isCommandFinished = true;            
        }
        currentTime = millis();
        if (commandId == 5){
            OnSlaveTouchEvent();
        }        
        if (currentTime - lastTouchTime > maxPause) {
            if (slaveTouchCount == 2){
                luluDog->dogEvents->OnSlaveDoubleTouchEvent();
            }
            slaveTouchCount = 0;
            // lastTouchTime = currentTime;
        }
        // log_d("I2CReceive: %d", commandId);

    }
}

void I2CSlaveHelper::commandConfirmTask()
{
    // delay(1500);
    while (!_isCommandFinished ){
        log_d("LuLu running command");
        delay(500);
    }
    luluDog->displayHelper->setIdleMode(true);
    luluDog->gyroHelper->ResumeGyro();
    // uint8_t response = requestI2CByte();

    // if (response == 3)
    // {
    //     while (response == 3)
    //     {
    //         delay(1000);
    //         uint8_t response = requestI2CByte();
    //     }
    // }

    // if (response == 4)
    // {
    //     luluDog->displayHelper->setIdleMode(true);
    //     luluDog->gyroHelper->ResumeGyro();
    // }
}

void I2CSlaveHelper::OnSlaveTouchEvent() {
    unsigned long currentTime = millis();
    
    // Проверяем, является ли это повторным касанием, чтобы сбросить флаг потенциального двойного нажатия
    if (potentialDoubleTap && (currentTime - lastTouchTime <= maxPause)) {
        potentialDoubleTap = false; // Сбрасываем флаг, так как были продолженные касания
    }
    
    // Проверяем, прошло ли меньше maxPause c момента последнего касания
    if (currentTime - lastTouchTime <= maxPause) {
        // Увеличиваем счетчик касаний
        slaveTouchCount++;

        // Проверяем, было ли обнаружено интенсивное поглаживание
        if (slaveTouchCount >= N) {            
            luluDog->dogEvents->OnSlaveIntensiveTouchEvent(slaveTouchCount);
        }
    }

    // Обновляем время последнего касания
    lastTouchTime = currentTime;
    luluDog->dogEvents->OnSlaveTouchEvent();
}