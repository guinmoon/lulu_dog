#include <Wire.h>
#include "character.h"
#include "commands.h"
#include "display_helper.h"
#include "audio_helper.h"
#include "global_def.h"
#include "battery_helper.h"
#include <Adafruit_SleepyDog.h>

unsigned long lastImpact = millis();
bool deepSleeping = false;
bool sleeping = false;
int allowedOnCharging[4] = {0, 10, 11, 12};
int maxChoise = 13;

// float probabilities[] = {0.6, 0.5, 0.5, 0.5, 0.5, 0.5, 0.9, 0.3, 0.5, 0.3, 0.5, 0.5, 0.5};
float probabilities[] = {
    0.5, /*sendCommand(COMMAND_SET_TAIL_SPEED, 0);*/
    0.4, /*sendCommand(COMMAND_SIT, 4);*/
    0.5, /*sendCommand(COMMAND_SIT, 5);*/
    0.4, /*sendCommand(COMMAND_STAND, 2);*/
    0.3, /*SendCommand(COMMAND_LAYDOWN, 4);*/
    0.3, /*SendCommand(COMMAND_SET_TAIL_SPEED, 4);*/
    0.9, /*SendCommand(COMMAND_LEFTHAND, 4);*/
    0.2, /*SendCommand(COMMAND_LAYDOWN, 3); tail 6*/
    0.5, /*SendCommand(COMMAND_LAYDOWN, 3); tail 0*/
    0.7, /*SendCommand(COMMAND_HALFLAYDOWN, 2);*/
    0.2, /*SendCommand(COMMAND_SET_TAIL_SPEED, 4);*/
    0.9, /*SendCommand(COMMAND_RIGHTHAND, 4);*/
};
int size = sizeof(probabilities) / sizeof(probabilities[0]);

int generateRandomWithProbabilities(float probabilities[], int size)
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

void NormalizeProbabilities()
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

void sendCommand(int command)
{
    WIRE.beginTransmission(8); // Адрес ведомого устройства
    WIRE.write(command);
    WIRE.endTransmission();
    log_d("Sended: %i", command);
}

void SendCommand(int command, int arg1)
{
    WIRE.beginTransmission(8); // Адрес ведомого устройства
    WIRE.write(command);
    WIRE.write(arg1);
    WIRE.endTransmission();
    log_d("Sended %i:%i", command, arg1);
}

#define THRESHOLD 40

void SleepPrepare(){
    SendCommand(COMMAND_SET_TAIL_SPEED, 0);
    log_d("SLEEP");
    StopGif();
    delay(1000);
    SendCommand(COMMAND_SET_TAIL_SPEED, 0);
}

void GoToDeepSleep()
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

void GoToSleep()
{
    SleepPrepare();
    sleeping = true;
    showSleepAnimation();
}

void dogActivitiWatcherThread(void *args)
{
    while (true)
    {
        if ((millis() - lastImpact) / 1000 >= SLEEP_AFTER && !sleeping)
        {
            GoToSleep();
        }
        if ((millis() - lastImpact) / 1000 >= DEEP_SLEEP_AFTER && !isCharging())
        {
            GoToDeepSleep();
        }
        delay(1000);
    }
    vTaskDelete(NULL);
}

void StartDogActivitiWatcher()
{
    NormalizeProbabilities();
    xTaskCreatePinnedToCore(
        dogActivitiWatcherThread, /* Task function. */
        "Task7",                  /* name of task. */
        10000,                    /* Stack size of task */
        NULL,                     /* parameter of the task */
        1,                        /* priority of the task */
        NULL,                     /* Task handle to keep track of created task */
        0);
}

int getAllowedRandomReact()
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

int GetAllowedSceneReact()
{
    int choice = random(2);
    return choice;
}

void _wake()
{
    if (sleeping)
    {
        sleeping = false;
        log_d("WAKE");
        stopSleepAnimation();
        delay(200);
    }
}

void doRandomReact(int direction)
{

    int current_time = millis();
    if (current_time - lastImpact < LAST_IMPACT_MIN_PERIOD)
    {
        lastImpact = current_time;
        return;
    }
    lastImpact = current_time;
    _wake();

    int choice = getAllowedRandomReact();

    switch (choice)
    {
    case 0:
        SendCommand(COMMAND_SET_TAIL_SPEED, 0);
        PlayGif("/eye1.gif");
        break;
    case 1:
        SendCommand(COMMAND_SET_TAIL_SPEED, 6);
        delay(200);
        SendCommand(COMMAND_SIT, 4);
        PlayGif("/eye1.gif");
        PlayWav("woof2.wav");
        break;
    case 2:
        SendCommand(COMMAND_SET_TAIL_SPEED, 4);
        delay(200);
        SendCommand(COMMAND_SIT, 5);
        PlayGif("/eye1.gif");
        PlayWav("woof2.wav");
        break;
    case 3:
        SendCommand(COMMAND_SET_TAIL_SPEED, 0);
        delay(200);
        SendCommand(COMMAND_STAND, 2);
        PlayWav("woof1.wav");
        PlayGif("/eye2.gif");
        break;
    case 4:
        SendCommand(COMMAND_SET_TAIL_SPEED, 4);
        delay(200);
        SendCommand(COMMAND_LAYDOWN, 4);
        PlayGif("/eye3.gif");
        break;
    case 5:
        SendCommand(COMMAND_SET_TAIL_SPEED, 4);
        delay(200);
        // sendCommand(COMMAND_HAPPY, 3);
        PlayGif("/eye4.gif");
        break;
    case 6:
        SendCommand(COMMAND_LEFTHAND, 4);
        delay(200);
        SendCommand(COMMAND_SET_TAIL_SPEED, 4);
        PlayWav("woof3.wav");
        // sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        PlayGif("/eye4.gif");
        break;
    case 7:
        SendCommand(COMMAND_SET_TAIL_SPEED, 6);
        delay(200);
        SendCommand(COMMAND_LAYDOWN, 3);
        PlayGif("/eye3.gif");
        PlayWav("woof2.wav");
        break;

    case 8:
        SendCommand(COMMAND_SET_TAIL_SPEED, 0);
        delay(200);
        SendCommand(COMMAND_LAYDOWN, 3);
        PlayGif("/eye3.gif");
        PlayWav("woof2.wav");
        break;

    case 9:
        SendCommand(COMMAND_SET_TAIL_SPEED, 7);
        delay(200);
        SendCommand(COMMAND_HALFLAYDOWN, 2);
        PlayGif("/eye3.gif");
        PlayWav("woof2.wav");
        break;

    case 10:
        SendCommand(COMMAND_SET_TAIL_SPEED, 4);
        PlayWav("woof1.wav");
        PlayGif("/eye5.gif");
        break;
    case 11:
        SendCommand(COMMAND_RIGHTHAND, 4);
        delay(200);
        SendCommand(COMMAND_SET_TAIL_SPEED, 4);
        PlayWav("woof3.wav");
        // sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        PlayGif("/eye4.gif");
        break;
    // case 12:
    //     sendCommand(COMMAND_SET_TAIL_SPEED, 7);
    //     delay(200);
    //     sendCommand(COMMAND_DANCE1,4);
    //     playGif("/eye5.gif");
    //     break;
    default:
        // sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        SendCommand(COMMAND_SET_TAIL_SPEED, 0);
        PlayGif("/eye5.gif");
        break;
    }
}

void DoSceneReact(int x, int y)
{
    int current_time = millis();
    if (current_time - lastImpact < LAST_IMPACT_MIN_PERIOD)
    {
        lastImpact = current_time;
        return;
    }
    lastImpact = current_time;
    _wake();

    int choice = GetAllowedSceneReact();
    switch (choice)
    {
    case 0:
        SendCommand(COMMAND_LEFTHAND, 4);
        delay(200);
        SendCommand(COMMAND_SET_TAIL_SPEED, 4);
        PlayWav("woof3.wav");
        PlayGif("/eye4.gif");
        break;
    case 1:
        SendCommand(COMMAND_SET_TAIL_SPEED, 7);
        delay(200);
        SendCommand(COMMAND_DANCE1, 4);
        PlayGif("/eye5.gif");
        break;
    default:
        SendCommand(COMMAND_LEFTHAND, 4);
        delay(200);
        SendCommand(COMMAND_SET_TAIL_SPEED, 0);
        PlayWav("woof3.wav");
        PlayGif("/eye4.gif");
        break;
    }
}