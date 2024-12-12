#include <Wire.h>
#include "character.h"
#include "commands.h"
#include "gif_helper.h"

#define WIRE Wire1

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

void doRandomReact()
{

    int choice = random(13);
    switch (choice)
    {
        // default:
        //     sendCommand(COMMAND_LEFTHAND, 5);
        //     sendCommand(COMMAND_SET_TAIL_SPEED, 6);
        //     playGif("/eye4.gif");
    case 0:
        // sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        break;
    case 1:
        sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        sendCommand(COMMAND_SIT, 4);
        playGif("/eye1.gif");
        break;
    case 2:
        sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        sendCommand(COMMAND_STAND, 4);
        playGif("/eye2.gif");
        break;
    case 3:
        sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        sendCommand(COMMAND_LAYDOWN, 4);
        playGif("/eye3.gif");
        break;
    case 4:
        sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        sendCommand(COMMAND_HAPPY, 4);
        playGif("/eye4.gif");
        break;
    case 5:
        sendCommand(COMMAND_LEFTHAND, 5);
        // sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        playGif("/eye4.gif");
        break;
    default:
        // sendCommand(COMMAND_SET_TAIL_SPEED, 4);
        sendCommand(COMMAND_SET_TAIL_SPEED, 0);
        playGif("/eye5.gif");
        break;
    }
}