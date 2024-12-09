#include "Arduino_GFX_Library.h"
#include "pin_config.h"
#include <Wire.h>
#include <AnimatedGIF.h>
#include "LittleFS.h"

void GIFDraw(GIFDRAW *pDraw);
bool loadGIFToMemory(const char *filename);
void playInfinite(void *pvParameters);
void InitDisplay();
void playGif(const char *fname);