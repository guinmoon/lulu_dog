#include <Arduino.h>
#include "pin_config.h"
#include <Wire.h>
#include "LittleFS.h"
#include "gif_helper.h"
#include "gyro_helper.h"
#include "commands.h"
// #include <ESP_I2S.h>

#include "Audio.h"

// #include "AudioGeneratorWAV.h"
// #include "AudioFileSourcePROGMEM.h"
// // #include "AudioOutputI2S.h"
// #include "AudioOutputI2SNoDAC.h"

#define I2S_DOUT 17 // Используем GPIO18 для Data Out
#define I2S_BCLK 18 // Используем GPIO17 для Bit Clock
#define I2S_LRC 16   // Используем GPIO3 для Left/Right Clock (Word Select)

// AudioGeneratorWAV *wav;
// AudioFileSourcePROGMEM *progMemFile;
// // // AudioOutputI2S *out;
// AudioOutputI2SNoDAC *out;

const int voltageDividerPin = 1;
float vRef = 3.3;    // Power supply voltage of ESP32-S3 (unit: volts)
float R1 = 200000.0; // Resistance value of the first resistor (unit: ohms)
float R2 = 100000.0;

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

void initPSRAM()
{
    psramInit();
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());
}

void initGyro()
{
    gyroInit();

    xTaskCreatePinnedToCore(
        gyroAndAccelReadTask, /* Task function. */
        "Task3",              /* name of task. */
        10000,                /* Stack size of task */
        NULL,                 /* parameter of the task */
        3,                    /* priority of the task */
        NULL,                 /* Task handle to keep track of created task */
        1);
}

void initFS()
{
    if (!LittleFS.begin())
    {
        log_d("Failed to mount LittleFS");
        return;
    }
}

Audio audio;

void initAudio(){
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(21);
    audio.connecttoFS(LittleFS,"/example3.wav");
     // audio.forceMono(true);

    // audio.i2s_mclk_pin_select(3);
    // audio.setI2SCommFMT_LSB(true);
}



const int frequency = 440;   // frequency of square wave in Hz
const int amplitude = 500;   // amplitude of square wave
const int sampleRate = 8000; // sample rate in Hz

// i2s_data_bit_width_t bps = I2S_DATA_BIT_WIDTH_16BIT;
// i2s_mode_t mode = I2S_MODE_STD;
// i2s_slot_mode_t slot = I2S_SLOT_MODE_MONO;

const int halfWavelength = (sampleRate / frequency); // half wavelength of square wave

int32_t sample = amplitude; // current sample value
int count = 0;

// I2SClass i2s;

// https://aliexpress.ru/item/1005007552403202.html?sku_id=12000041264231643&spm=a2g2w.productlist.search_results.0.27e25c64JaDElS

void setup(void)
{
    // USBSerial.begin(115200);
    Serial.begin(115200);
    // Wire.begin(IIC_SDA, IIC_SCL);
    // Wire1.begin(17, 18);

    log_d("LuLu2 hello");

    initPSRAM();
    initFS();
    InitDisplay();    
    // initGyro();
    // playGif("/eye3.gif");
    // initAudio();
    pinMode(voltageDividerPin, INPUT);

    
    
   

    

    

    // out = new AudioOutputI2S();
    // out = new AudioOutputI2SNoDAC();

    // out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    // // // out->SetBitsPerSample(16);
    // // out->SetRate(16000);
    // // // out->set
    // // // out->SetGain(4.0);
    // // // out->SetOutputModeMono(true);
    // out->begin();

    

    // pinMode(2, INPUT);

    // xTaskCreatePinnedToCore(
    //     touchReadTask, /* Task function. */
    //     "Task2",       /* name of task. */
    //     10000,         /* Stack size of task */
    //     NULL,          /* parameter of the task */
    //     2,             /* priority of the task */
    //     NULL,          /* Task handle to keep track of created task */
    //     1);

    // sendCommand(COMMAND_SET_TAIL_SPEED,2);
    // delay(2000);
    // sendCommand(COMMAND_SET_TAIL_SPEED,4);
    // delay(2000);
    // sendCommand(COMMAND_SET_TAIL_SPEED,0);
    // delay(2000);

    // i2s.setPins(I2S_BCLK,I2S_LRC,I2S_DOUT);
    // i2s.setInverted(false,true);
    // if (!i2s.begin(mode, sampleRate, bps, slot))
    // {
    //     Serial.println("Failed to initialize I2S!");
    //     while (1)
    //         delay(1000); // do nothing
    // }
    // Загружаем WAV в память

    // if (loadWAVToMemory("/example2.wav"))
    // { // Замените на путь к вашему файлу
    //     // Используем AudioFileSourcePROGMEM для воспроизведения из памяти
    //     log_d("loaded wav");
    //     log_d("%d", wavSize);
    //     // i2s.playWAV(wavData,wavSize);
    //     progMemFile = new AudioFileSourcePROGMEM(wavData, wavSize);
    //     wav = new AudioGeneratorWAV();
    //     log_d("inited wav");
    //     wav->begin(progMemFile, out);
        
    // }
}

void loop()
{
    // if (count % halfWavelength == 0)
    // {
    //     // invert the sample every half wavelength count multiple to generate square wave
    //     sample = -1 * sample;
    // }

    // i2s.write(sample); // Right channel
    // i2s.write(sample); // Left channel

    // // increment the counter for the next sample
    // count++;
    
    audio.loop();
    // log_d("loop");
    // delay(1000);
    // log_d("WAV plaing\n");
    // if (wav->isRunning())
    // {

    //     if (!wav->loop())
    //         wav->stop();
    // }
    // else   
    // {
    //     log_d("WAV done\n");
    //     delay(1000);
    // }
    // int adcValue = analogRead(voltageDividerPin);

    // // Convert to voltage
    // float voltage = (float)adcValue * (vRef / 4095.0);

    // // Apply the voltage divider formula to calculate the actual voltage
    // float actualVoltage = voltage * ((R1 + R2) / R2);

    // // Print the actual voltage
    // log_d("adcValue %d, Actual Voltage: %d V",adcValue,actualVoltage);

    // delay(1000);
}