#ifndef CONFIG_HELPER_H
#define CONFIG_HELPER_H

#include "lulu_dog.h"

class ConfigHelper
{
    public:    
    ConfigHelper(LuLuDog *luluDog){
        this->luluDog = luluDog;
    }
    ~ConfigHelper();
    private:
    static LuLuDog *luluDog;
    
};


#endif