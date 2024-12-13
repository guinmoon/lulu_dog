#include <Wire.h>
#include "character.h"
#include "commands.h"
#include "display_helper.h"
#include "audio_helper.h"

#define WIRE Wire

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

void doRandomReact(int direction)
{

    int choice = random(13);
    switch (choice)
    {
    case 0:
        // sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        sendCommand(COMMAND_SET_TAIL_SPEED, 0);
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
        sendCommand(COMMAND_LEFTHAND, 5);
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
    default:
        // sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        playGif("/eye5.gif");
        break;
    }
}