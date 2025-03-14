#ifndef CONFIG_HELPER_H
#define CONFIG_HELPER_H
#include "global_def.h"
#include "ArduinoJson.h"
#include <LittleFS.h>

// 1KB RAM
class LuLuDog;

class ConfigHelper
{
public:
    bool EnableAudio = AUDIO_ON;
    bool EnableMove = MOVE_ON;
    bool EnableSleep = SLEEP_ON;
    bool EnableDeepSleep = DEEP_SLEEP_ON;
    bool EnableWifi = WIFI_ON;
    DynamicJsonDocument *config; 
    ConfigHelper(LuLuDog *luluDog)
    {
        this->luluDog = luluDog;
    }
    bool LoadConfig(char *config_path);
    bool SetProperty(char *key, bool value);
    void SaveConfig();
    ~ConfigHelper();

private:
    char *_config_path;
    LuLuDog *luluDog;
};

#endif