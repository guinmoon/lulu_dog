#include <Arduino.h>
#include "Arduino_GFX_Library.h"
#include "pin_config.h"
#include <Wire.h>
#include <AnimatedGIF.h>
#include "homer_small_car.h"
#include "LittleFS.h"
#include "gif_helper.h"
#include "gyro_helper.h"
#include "commands.h"

// #include "Audio.h"
#include "AudioGeneratorWAV.h"
#include "AudioFileSourcePROGMEM.h"
#include "AudioOutputI2S.h"



#define I2S_DOUT 10 // Используем GPIO18 для Data Out
#define I2S_BCLK 11 // Используем GPIO17 для Bit Clock
#define I2S_LRC 2   // Используем GPIO3 для Left/Right Clock (Word Select)

AudioGeneratorWAV *wav;
AudioFileSourcePROGMEM *progMemFile;
AudioOutputI2S *out;

uint8_t *wavData = nullptr;
size_t wavSize = 0;

bool loadWAVToMemory(const char *filename)
{
    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        log_d("Failed to open WAV file");
        return false;
    }

    wavSize = file.size();
    log_d("Used PSRAM: %d", ESP.getPsramSize() - ESP.getFreePsram());
    wavData = (uint8_t *)ps_malloc(wavSize);
    log_d("WAV Size: %d Used PSRAM: %d", wavSize, ESP.getPsramSize() - ESP.getFreePsram());
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());
    if (!wavData)
    {
        log_d("Failed to allocate memory for WAV");
        file.close();
        return false;
    }

    file.read(wavData, wavSize);
    file.close();
    return true;
}

// void setServoPosition(int servoID, int targetPos, int speed)
// {
//     Wire1.beginTransmission(8); // Адрес ведомого устройства
//     Wire1.write(servoID);
//     Wire1.write(targetPos);
//     Wire1.write(speed);
//     Wire1.endTransmission();
//     log_d("Sended.");
// }

void sendCommand(int command)
{
    Wire1.beginTransmission(8); // Адрес ведомого устройства
    Wire1.write(command);
    Wire1.endTransmission();
    log_d("Sended.");
}

void sendCommand(int command, int arg1)
{
    Wire1.beginTransmission(8); // Адрес ведомого устройства
    Wire1.write(command);
    Wire1.write(arg1);
    Wire1.endTransmission();
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

void touchReadTask(void *params)
{
    while (true)
    {
        int val = digitalRead(2);
        if (val != 0)
        {
            log_d("loop %d", val);
            doRandomReact();
        }
    }
    vTaskDelete(NULL);
}




// Audio audio;

void setup(void)
{
    USBSerial.begin(115200);
    // Wire.begin(13, 14);
    Wire1.begin(17, 18);

    log_d("Arduino_GFX Hello World example");

    // Init Display
    InitDisplay();

    // delay(500);

    if (!LittleFS.begin())
    {
        log_d("Failed to mount LittleFS");
        return;
    }

    psramInit();
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());

    // audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    // audio.setVolume(100);
    // audio.connecttoFS(LittleFS,"/R2-D2_17_8k.mp3");
    // audio.connecttoFS(LittleFS,"/example.wav");

    out = new AudioOutputI2S();
    out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    out->begin();

    // Загружаем WAV в память
    if (loadWAVToMemory("/example.wav"))
    { // Замените на путь к вашему файлу
        // Используем AudioFileSourcePROGMEM для воспроизведения из памяти
        log_d("loaded wav");
        log_d("%d", wavSize);
        progMemFile = new AudioFileSourcePROGMEM(wavData, wavSize);
        wav = new AudioGeneratorWAV();
        log_d("inited wav");
        wav->begin(progMemFile, out);
    }

    playGif("/eye3.gif");

    // pinMode(2, INPUT);

    gyroInit();

    // xTaskCreatePinnedToCore(
    //     touchReadTask, /* Task function. */
    //     "Task2",       /* name of task. */
    //     10000,         /* Stack size of task */
    //     NULL,          /* parameter of the task */
    //     2,             /* priority of the task */
    //     NULL,          /* Task handle to keep track of created task */
    //     1);

    xTaskCreatePinnedToCore(
        gyroAndAccelReadTask, /* Task function. */
        "Task3",              /* name of task. */
        10000,                /* Stack size of task */
        NULL,                 /* parameter of the task */
        3,                    /* priority of the task */
        NULL,                 /* Task handle to keep track of created task */
        1);

    // sendCommand(COMMAND_SET_TAIL_SPEED,2);
    // delay(2000);
    // sendCommand(COMMAND_SET_TAIL_SPEED,4);
    // delay(2000);
    // sendCommand(COMMAND_SET_TAIL_SPEED,0);
    // delay(2000);
}

void loop()
{

    // audio.loop();
    // log_d("loop");
    // delay(1000);
    if (wav->isRunning())
    {
        log_d("WAV plaing\n");
        if (!wav->loop())
            wav->stop();
    }
    else
    {
        log_d("WAV done\n");
        delay(1000);
    }
}