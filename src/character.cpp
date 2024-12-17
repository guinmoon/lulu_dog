#include <Wire.h>
#include "character.h"
#include "commands.h"
#include "display_helper.h"
#include "audio_helper.h"
#include "global_def.h"
#include "battery_helper.h"

unsigned long lastImpact = millis();
bool sleeping = false;
int allowedOnCharging[4] = {0, 10, 11, 12};
int maxChoise = 13;

void sendCommand(int command)
{
    WIRE.beginTransmission(8); // Адрес ведомого устройства
    WIRE.write(command);
    WIRE.endTransmission();
    log_d("Sended.");
}

void sendCommand(int command, int arg1)
{
    WIRE.beginTransmission(8); // Адрес ведомого устройства
    WIRE.write(command);
    WIRE.write(arg1);
    WIRE.endTransmission();
    log_d("Sended.");
}

void dogActivitiWatcherThread(void *args)
{
    while (true)
    {
        if ((millis() - lastImpact) / 1000 >= MAX_INACTIVE_SEC && !sleeping)
        {
            sleeping = true;
            sendCommand(COMMAND_SET_TAIL_SPEED, 0);
            log_d("SLEEP");
            stopGif();
            delay(1000);
            showSleepAnimation();
        }
        delay(1000);
    }
    vTaskDelete(NULL);
}

void startDogActivitiWatcher()
{
    xTaskCreatePinnedToCore(
        dogActivitiWatcherThread, /* Task function. */
        "Task7",                  /* name of task. */
        10000,                    /* Stack size of task */
        NULL,                     /* parameter of the task */
        1,                        /* priority of the task */
        NULL,                     /* Task handle to keep track of created task */
        0);
}

int getAllowedChoice()
{
    bool allowed = false;
    int choice = random(maxChoise);
    if (!isCharging())
        return choice;
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

void doRandomReact(int direction)
{

    lastImpact = millis();
    if (sleeping)
    {
        sleeping = false;
        log_d("WAKE");
        stopSleepAnimation();
        delay(200);
    }
    
    int choice = getAllowedChoice();
    
    switch (choice)
    {
    case 0:
        sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        playGif("/eye1.gif");
        break;
    case 1:
        sendCommand(COMMAND_SET_TAIL_SPEED, 6);
        delay(200);
        sendCommand(COMMAND_SIT, 4);
        playGif("/eye1.gif");
        playWav("woof2.wav");
        break;
    case 2:
        sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        delay(200);
        sendCommand(COMMAND_STAND, 2);
        playWav("woof1.wav");
        playGif("/eye2.gif");
        break;
    case 3:
        sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        delay(200);
        sendCommand(COMMAND_LAYDOWN, 4);
        playGif("/eye3.gif");
        break;
    case 4:
        sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        delay(200);
        // sendCommand(COMMAND_HAPPY, 3);
        playGif("/eye4.gif");
        break;
    case 5:
        sendCommand(COMMAND_LEFTHAND, 4);
        delay(200);
        sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        playWav("woof3.wav");
        // sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        playGif("/eye4.gif");
        break;
    case 6:
        sendCommand(COMMAND_SET_TAIL_SPEED, 6);
        delay(200);
        sendCommand(COMMAND_LAYDOWN, 3);
        playGif("/eye3.gif");
        playWav("woof2.wav");
        break;
    case 7 ... 8:
        sendCommand(COMMAND_SET_TAIL_SPEED, 7);
        delay(200);
        sendCommand(COMMAND_HALFLAYDOWN, 2);
        playGif("/eye3.gif");
        playWav("woof2.wav");
        break;
        // case 9:
        //     sendCommand(COMMAND_SET_TAIL_SPEED, 7);
        //     delay(200);
        //     sendCommand(COMMAND_DANCE1,4);
        //     playGif("/eye5.gif");
        //     break;
    case 10:
        sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        playWav("woof1.wav");
        playGif("/eye3.gif");
        break;
    case 11:
        sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        playGif("/eye2.gif");
        break;
    case 12:
        sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        playGif("/eye5.gif");
        break;
    default:
        // sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        playGif("/eye5.gif");
        break;
    }
}