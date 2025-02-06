#ifndef AUDIO_HELPER_H
#define AUDIO_HELPER_H


#include <ESP_I2S.h>

// void PlayWav(char *fname);
// void InitAudio();

class AudioHelper
{
private:
public:
    const int frequency = 440;   // frequency of square wave in Hz
    const int amplitude = 500;   // amplitude of square wave
    const int sampleRate = 8000; // sample rate in Hz

    I2SClass i2s;

    i2s_data_bit_width_t bps = I2S_DATA_BIT_WIDTH_16BIT;
    i2s_mode_t mode = I2S_MODE_STD;
    i2s_slot_mode_t slot = I2S_SLOT_MODE_MONO;

    const int halfWavelength = (sampleRate / frequency); // half wavelength of square wave

    int32_t sample = amplitude; // current sample value
    int count = 0;

    // I2SClass i2s;

    uint8_t *wavData = nullptr;
    size_t wavSize = 0;

    AudioHelper();
    bool loadWAVToMemory(const char *filename);
    void InitAudio();
    static void audioThread(void *params);
    void PlayWav(char *fname);
    void AudioTask();
};

// extern AudioHelper audioHelper;

#endif