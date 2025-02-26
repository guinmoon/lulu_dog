#include "config_helper.hpp"
#include "lulu_dog.h"

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
