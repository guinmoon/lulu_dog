#include <Arduino.h>
#include "pin_config.h"
#include <Wire.h>
#include "LittleFS.h"
#include "display_helper.h"
#include "gyro_helper.h"
#include "battery_helper.h"
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
    init_battery();
    initPSRAM();
    initFS();
    InitDisplay();
    gyroInit();    
    playGif("/eye3.gif");
    // initAudio();
    


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
    
    // audio.loop();
    // log_d("loop");
    // delay(1000);
   
    

    // Print the actual voltage
    // log_d("voltage %f, adcValue %d, Actual Voltage: %f V",voltage,adcValue,actualVoltage);
    // log_d("Actual Voltage: %f V",actualVoltage);
    // float actualVoltage = get_battery_voltage_cached();
    // sprintf(print_buf,"Actual Voltage: %f V",actualVoltage);
    // printOnDisplay(print_buf);

    delay(1000);
}