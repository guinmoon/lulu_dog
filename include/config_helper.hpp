#ifndef CONFIG_HELPER_H
#define CONFIG_HELPER_H
#include "global_def.h"
#include "ArduinoJson.h"
#include <LittleFS.h>


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
    bool LoadConfig(char *config_path)
    {
        _config_path = config_path;
        File config_file = LittleFS.open(config_path, "r");
        if (config_file)
        {
            config =  new DynamicJsonDocument(config_file.size() * 1.33);
            deserializeJson(*config, config_file);

            EnableAudio = (*config)["dog"]["audio"]["enable"].as<bool>();
            EnableMove = (*config)["dog"]["move"]["enable"].as<bool>();
            EnableSleep = (*config)["dog"]["sleep"]["enable"].as<bool>();            
            EnableWifi = (*config)["dog"]["wifi"]["enable"].as<bool>();
            log_d("Config loaded");
            log_d("EnableAudio: %d, EnableMove: %d, EnableSleep: %d, EnableDeepSleep: %d, EnableWifi: %d", EnableAudio,
                 EnableMove, EnableSleep, EnableDeepSleep, EnableWifi);            
            return true;
        }
        return false;
    }
    bool SetProperty(char *key, bool value);
    void SaveConfig()
    {
        File config_file = LittleFS.open(_config_path, "w");
        serializeJsonPretty(*config, config_file);
        config_file.close();
    }
    ~ConfigHelper();

private:
    char *_config_path;
    LuLuDog *luluDog;
};

#endif