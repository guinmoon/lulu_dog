#ifndef CHARACTER_HELPER_H
#define CHARACTER_HELPER_H

#include <Wire.h>
#include <mutex>
#include "display_helper.h"
#include "audio_helper.h"
#include "global_def.h"
#include "commands.h"
#include "battery_helper.h"
#define THRESHOLD 40
// void doRandomReact(int direction);
// void StartDogActivitiWatcher();
// void SendCommand(int command, int arg1);
// int generateRandomWithProbabilities(float probabilities[], int size);
// void DoSceneReact(int x, int y);

class LuLuDog;

class LuLuCharacter
{
private:
    LuLuDog* luluDog;
    unsigned long lastImpact = millis();
    bool deepSleeping = false;
    bool sleeping = false;
    int allowedOnCharging[4] = {0, 10, 11, 12};
    int maxChoise = 13;
    std::mutex i2c_mutex;
    bool pingPaused = false;
    float probabilities[15] = {
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
        0.1, /*SendCommand(COMMAND_FULLLAYDOWN, 0);*/
        0.5, /*SendCommand(COMMAND_TAILLEGSSTAND, 4);*/
        0.2 /*SendCommand(COMMAND_HALFLAYDOWNTAIL, 2);*/
    };

    int size = sizeof(probabilities) / sizeof(probabilities[0]);

public:
    bool suspended = false;
    LuLuCharacter(LuLuDog* _luluDog);
    static void DogActivitiWatcherThread(void *args);
    static void RP2040PingThread(void *args);
    void _wake();
    int generateRandomWithProbabilities(float probabilities[], int size);
    void NormalizeProbabilities();
    void sendCommand(int command);
    void SendCommand(int command, int arg1);
    void SleepPrepare();
    void GoToDeepSleep();
    void GoToSleep();    
    void StartDogActivitiWatcher();
    int getAllowedRandomReact();
    int GetAllowedSceneReact();
    void doReact(int command, int speed, int tail_speed, char *eye, char *wav);
    void doRandomReact(int direction);
    void DoSceneReact(int x, int y);
    void RP2040PingTask();
    void DogActivitiWatcherTask();
};

// extern LuLuCharacter luluCharacter;

#endif