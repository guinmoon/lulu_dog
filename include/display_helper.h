#include "Arduino_GFX_Library.h"
#include <Wire.h>
#include <AnimatedGIF.h>
#include "LittleFS.h"

void GIFDraw(GIFDRAW *pDraw);
bool loadGIFToMemory(const char *filename);
void playInfinite(void *pvParameters);
void InitDisplay();
void PlayGif(const char *fname);
void printOnDisplay(char *text, int x, int y);
void setVoltageBuf(float voltage);
void showSleepAnimation();
void StopGif();
void showSleepAnimation();
void stopSleepAnimation();
void drawBatteryheart();
void fillScreen();
void DisplayOff();
void DisplayOn();