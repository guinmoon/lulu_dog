#include "audio_helper.h"
#include <ESP_I2S.h>
#include "LittleFS.h"
#include "global_def.h"


// AudioHelper audioHelper;

AudioHelper::AudioHelper(){}




bool AudioHelper::loadWAVToMemory(const char *filename)
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

void AudioHelper::InitAudio()
{
    i2s.setPins( I2S_BCLK,I2S_LRC, I2S_DOUT);
    if (!i2s.begin(I2S_MODE_STD, 44100, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO, I2S_STD_SLOT_BOTH))
    {
        log_d("Failed to initialize I2S bus!");
        return;
    }
}

void AudioHelper::AudioTask(){
    i2s.playWAV(wavData,wavSize);
    log_d("Wav finished");
}

void AudioHelper::audioThread(void * _this)
{

    ((AudioHelper*)_this)->AudioTask();
    vTaskDelete(NULL);
}

void AudioHelper::PlayWav(char *fname)
{
    // audio.stopSong();
    if (!AUDIO_ON)
        return;
    delay(100);
    bool res = loadWAVToMemory(fname);
    if (!res)
        return;
    // audio.connecttoFS(LittleFS, fname);
    xTaskCreatePinnedToCore(
        this->audioThread, /* Task function. */
        "Task5",     /* name of task. */
        10000,        /* Stack size of task */
        this,        /* parameter of the task */
        tskIDLE_PRIORITY,          /* priority of the task */
        NULL,        /* Task handle to keep track of created task */
        1);
}

