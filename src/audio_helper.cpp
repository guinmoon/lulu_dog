#include "audio_helper.h"
#include "Audio.h"
#include "LittleFS.h"
#include "global_def.h"
// #include <ESP_I2S.h>

// #include "AudioGeneratorWAV.h"
// #include "AudioFileSourcePROGMEM.h"
// // #include "AudioOutputI2S.h"
// #include "AudioOutputI2SNoDAC.h"

// AudioGeneratorWAV *wav;
// AudioFileSourcePROGMEM *progMemFile;
// // // AudioOutputI2S *out;
// AudioOutputI2SNoDAC *out;

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

uint8_t *wavData = nullptr;
size_t wavSize = 0;
Audio audio;

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

void initAudio()
{
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(22);
    // audio.forceMono(true);

    // audio.i2s_mclk_pin_select(3);
    audio.setI2SCommFMT_LSB(true);
}

void audioThread(void *params)
{

    while (true)
    {
        audio.loop();

        // log_d("%i",audio.isRunning());
        if (!audio.isRunning())
            break;
    }
    vTaskDelete(NULL);
}

void playWav(char *fname)
{
    audio.stopSong();
    audio.connecttoFS(LittleFS, fname);
    xTaskCreatePinnedToCore(
        audioThread, /* Task function. */
        "Task5",     /* name of task. */
        10000,       /* Stack size of task */
        NULL,        /* parameter of the task */
        10,          /* priority of the task */
        NULL,        /* Task handle to keep track of created task */
        1);
}

// if (count % halfWavelength == 0)
// {
//     // invert the sample every half wavelength count multiple to generate square wave
//     sample = -1 * sample;
// }

// i2s.write(sample); // Right channel
// i2s.write(sample); // Left channel

// // increment the counter for the next sample
// count++;