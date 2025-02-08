#include <Wire.h>
#include "character.h"
#include "lulu_dog.h"

#include <mutex>

// LuLuCharacter luluCharacter;

LuLuCharacter::LuLuCharacter(LuLuDog* _luluDog){
    luluDog = _luluDog;
}

int LuLuCharacter::generateRandomWithProbabilities(float probabilities[], int size)
{
    // Cуммируем вероятности, чтобы они представляли собой диапазоны
    float cumulativeProbabilities[size];
    cumulativeProbabilities[0] = probabilities[0];
    for (int i = 1; i < size; i++)
    {
        cumulativeProbabilities[i] = cumulativeProbabilities[i - 1] + probabilities[i];
    }

    // Генерируем случайное число от 0 до 1
    float randomValue = random(1000) / 1000.0;

    // Определяем, в какой диапазон попало случайное число
    for (int i = 0; i < size; i++)
    {
        if (randomValue < cumulativeProbabilities[i])
        {
            // Возвращаем i+1, чтобы числа были 1 до n, i чтобы от 0..n-1
            return i;
        }
    }

    // В случае ошибки возвращаем 0
    return 0;
}

void LuLuCharacter::NormalizeProbabilities()
{
    float sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += probabilities[i];
    }
    for (int i = 0; i < size; i++)
    {
        probabilities[i] /= sum;
    }
}

void LuLuCharacter::sendCommand(int command)
{
    WIRE.beginTransmission(8); // Адрес ведомого устройства
    WIRE.write(command);
    WIRE.endTransmission();
    log_d("Sended: %i", command);
}

void LuLuCharacter::SendCommand(int command, int arg1)
{
    std::lock_guard<std::mutex> lck(i2c_mutex);
    WIRE.beginTransmission(8); // Адрес ведомого устройства
    WIRE.write(command);
    WIRE.write(arg1);
    WIRE.endTransmission();
    log_d("Sended %i:%i", command, arg1);
}

void LuLuCharacter::SleepPrepare()
{
    SendCommand(COMMAND_SET_TAIL_SPEED, 0);
    log_d("SLEEP");
    luluDog->displayHelper->StopGif();
    delay(1000);
    SendCommand(COMMAND_SET_TAIL_SPEED, 0);
}

void LuLuCharacter::GoToDeepSleep()
{
    log_d("PREPARE to SLEEP: %i ms", 2000);
    sleeping = true;
    deepSleeping = true;
    SleepPrepare();
    gpio_deep_sleep_hold_en();
    gpio_hold_en((gpio_num_t)SYS_EN_PIN);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)TP_INT, 0);
    esp_deep_sleep_start();
    log_d("SLEEPING FOR: %i ms", 2000);
}

void LuLuCharacter::GoToSleep()
{
    SleepPrepare();
    sleeping = true;
    luluDog->displayHelper->showSleepAnimation();
}

void LuLuCharacter::DogActivitiWatcherTask()
{
    while (true)
    {
        if (suspended){
            lastImpact = millis();
            delay(1000);
            continue;
        }
        if ((millis() - lastImpact) / 1000 >= SLEEP_AFTER && !sleeping)
        {
            GoToSleep();
        }
        if ((millis() - lastImpact) / 1000 >= DEEP_SLEEP_AFTER && !luluDog->batteryHelper->isCharging())
        {
            GoToDeepSleep();
        }

        delay(1000);
    }
}

void LuLuCharacter::DogActivitiWatcherThread(void *_this)
{
    ((LuLuCharacter *)_this)->DogActivitiWatcherTask();
    vTaskDelete(NULL);
}

void LuLuCharacter::RP2040PingTask()
{
    while (true)
    {
        if (!pingPaused)
            SendCommand(RP_SYS_COMMAND_PING, 0);
        delay(2000);
    }
}

void LuLuCharacter::RP2040PingThread(void *_this)
{
    ((LuLuCharacter *)_this)->RP2040PingTask();
    vTaskDelete(NULL);
}

void LuLuCharacter::StartDogActivitiWatcher()
{
    NormalizeProbabilities();
    xTaskCreatePinnedToCore(
        this->DogActivitiWatcherThread, /* Task function. */
        "Task7",                        /* name of task. */
        4096,                          /* Stack size of task */
        this,                           /* parameter of the task */
        tskIDLE_PRIORITY,               /* priority of the task */
        NULL,                           /* Task handle to keep track of created task */
        0);

    xTaskCreatePinnedToCore(
        this->RP2040PingThread, /* Task function. */
        "Task11",               /* name of task. */
        4096,                  /* Stack size of task */
        this,                   /* parameter of the task */
        tskIDLE_PRIORITY,       /* priority of the task */
        NULL,                   /* Task handle to keep track of created task */
        0);
}

int LuLuCharacter::getAllowedRandomReact()
{
    bool allowed = false;
    int choice = generateRandomWithProbabilities(probabilities, size);
    // if (!isCharging())
    return choice;

    // Если заряжается то выбираем доступный в этом режиме вариант
    while (!allowed)
    {
        choice = random(maxChoise);
        for (int i = 0; i < 4; i++)
        {
            if (choice == allowedOnCharging[i])
            {
                allowed = true;
                break;
            }
        }
    }
    return choice;
}

int LuLuCharacter::GetAllowedSceneReact()
{
    int choice = random(1);
    return choice;
}

void LuLuCharacter::_wake()
{
    if (sleeping)
    {
        sleeping = false;
        log_d("WAKE");
        luluDog->displayHelper->stopSleepAnimation();
        delay(200);
        // SendCommand(RP_SYS_COMMAND_WAKE,0);
    }
}

void LuLuCharacter::doReact(int command, int speed, int tail_speed, char *eye, char *wav)
{
    pingPaused = true;
    if (command != -1)
        SendCommand(command, speed);
    if (tail_speed != -1)
    {
        delay(200);
        SendCommand(COMMAND_SET_TAIL_SPEED, tail_speed);
    }
    if (eye != nullptr)
        luluDog->displayHelper->PlayGif(eye);
    if (wav != nullptr)
        luluDog->audioHelper->PlayWav(wav);
    delay(3000);
    pingPaused = false;
}

void LuLuCharacter::doRandomReact(int direction)
{

    int current_time = millis();
    if (current_time - lastImpact < LAST_IMPACT_MIN_PERIOD || suspended)
    {
        lastImpact = current_time;
        return;
    }
    lastImpact = current_time;
    _wake();

    if (luluDog->displayHelper->showMatrixAnimation){
        luluDog->displayHelper->StopMatrixAnimation();
    }

    int choice = getAllowedRandomReact();

    switch (choice)
    {
    case 0:
        doReact(-1, -1, 0, "/eye1.gif", nullptr);
        break;
    case 1:
        doReact(COMMAND_SIT, 4, 6, "/eye1.gif", "/woof2.wav");
        break;
    case 2:
        doReact(COMMAND_SIT, 5, 4, "/eye1.gif", "/woof2.wav");
        break;
    case 3:
        doReact(COMMAND_STAND, 2, 0, "/eye2.gif", "/woof1.wav");
        break;
    case 4:
        doReact(COMMAND_LAYDOWN, 4, 4, "/eye2.gif", nullptr);
        break;
    case 5:
        doReact(-1, -1, 4, "/eye4.gif", nullptr);
        break;
    case 6:
        doReact(COMMAND_LEFTHAND, 4, 4, "/eye4.gif", "/woof3.wav");
        break;
    case 7:
        doReact(COMMAND_LAYDOWN, 3, 6, "/eye3.gif", "/woof2.wav");
        break;
    case 8:
        doReact(COMMAND_LAYDOWN, 3, 0, "/eye3.gif", "/woof2.wav");
        break;
    case 9:
        doReact(COMMAND_HALFLAYDOWN, 2, 7, "/eye3.gif", "/woof2.wav");
        break;
    case 10:
        doReact(-1, -1, 4, "/eye5.gif", "/woof1.wav");
        break;
    case 11:
        doReact(COMMAND_RIGHTHAND, 4, 4, "/eye4.gif", "/woof3.wav");
        break;
    case 12:
        doReact(COMMAND_FULLLAYDOWN, 7, 0, "/eye3.gif", "/woof1.wav");
        break;
    case 13:
        doReact(COMMAND_TAILLEGSSTAND, 4, 4, "/eye3.gif", "/woof1.wav");
        break;
    case 14:
        doReact(COMMAND_HALFLAYDOWNTAIL, 4, 6, "/eye3.gif", "/woof1.wav");
        break;
    default:
        doReact(-1, -1, 0, "/eye5.gif", nullptr);
        break;
    }
}

void LuLuCharacter::DoSceneReact(int x, int y)
{
    int current_time = millis();
    // if (current_time - lastImpact < LAST_IMPACT_MIN_PERIOD)
    // {
    //     lastImpact = current_time;
    //     return;
    // }
    lastImpact = current_time;
    _wake();

    int choice = GetAllowedSceneReact();
    switch (choice)
    {
    // case 0:
    //     SendCommand(COMMAND_LEFTHAND, 4);
    //     delay(200);
    //     SendCommand(COMMAND_SET_TAIL_SPEED, 4);
    //     PlayWav("/woof3.wav");
    //     PlayGif("/eye4.gif");
    //     break;
    case 1:
        doReact(COMMAND_DANCE1, 4, 7, "/eye5.gif", "/woof1.wav");
        break;
    default:
        doReact(COMMAND_DANCE1, 4, 7, "/eye5.gif", "/woof1.wav");
        break;
    }
}