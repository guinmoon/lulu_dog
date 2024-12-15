#include "Arduino_GFX_Library.h"
#include <Wire.h>
#include <AnimatedGIF.h>
#include "LittleFS.h"

void GIFDraw(GIFDRAW *pDraw);
bool loadGIFToMemory(const char *filename);
void playInfinite(void *pvParameters);
void InitDisplay();
void playGif(const char *fname);
void printOnDisplay(char *text);
void setVoltageBuf(float voltage);
void showSleepAnimation();
void stopGif();
void showSleepAnimation();
void stopSleepAnimation();