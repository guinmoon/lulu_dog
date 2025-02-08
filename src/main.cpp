#include "lulu_dog.h"
#include "hw_init.h"

static LuLuDog luluDog;

void setup(void)
{

    HWInit();
    luluDog.Init();
}

void loop()
{
    delay(1000);
}