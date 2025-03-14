#include "config_helper.hpp"
#include "lulu_dog.h"

bool ConfigHelper::LoadConfig(char *config_path)
{
    _config_path = config_path;
    File config_file = LittleFS.open(config_path, "r");
    if (config_file)
    {
        config = new DynamicJsonDocument(config_file.size() * 1.33);
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
    config = new DynamicJsonDocument(10000 * 1.33);
    return false;
}


bool ConfigHelper::SetProperty(char *key, bool value)
{
    
    if (strcmp(key, "audio") == 0)
    {
        EnableAudio = value;
        (*config)["dog"]["audio"]["enable"] = EnableAudio;
    }
    else if (strcmp(key, "move") == 0)
    {
        EnableMove = value;
        (*config)["dog"]["move"]["enable"] = EnableMove;
    }
    else if (strcmp(key, "sleep") == 0)
    {
        EnableSleep = value;
        (*config)["dog"]["sleep"]["enable"] = EnableSleep;
    }
    else if (strcmp(key, "wifi") == 0)
    {
        EnableWifi = value;
        (*config)["dog"]["wifi"]["enable"] = EnableWifi;
        if (EnableWifi)
        {
            if (!luluDog->fsWebServer->serverRunning)
                luluDog->fsWebServer->Init();
        }
        else
        {
            if (luluDog->fsWebServer->serverRunning)
                luluDog->fsWebServer->Deinit();
        }
    }
    else
    {
        log_d("Invalid key: %s", key);
        return false;
    }
    SaveConfig();
    return true;
}

void ConfigHelper::SaveConfig()
{
    File config_file = LittleFS.open(_config_path, "w");
    if (!config_file)
        config_file = LittleFS.open(_config_path, "w", true);
    serializeJsonPretty(*config, config_file);
    config_file.close();
    luluDog->MemInfo();
}