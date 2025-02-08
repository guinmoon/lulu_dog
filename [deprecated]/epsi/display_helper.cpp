#include "display_helper.h"
#include "lulu_dog.h"
#include <Arduino.h>
#include "global_def.h"




TFT_eSPI* DisplayHelper::gfx = new TFT_eSPI(LCD_WIDTH, LCD_HEIGHT);

//  TFT_eSPI* DisplayHelper::gfx;
AnimatedGIF DisplayHelper::gif;

DisplayHelper::DisplayHelper(LuLuDog* _luluDog) {
    luluDog = _luluDog;
}

void DisplayHelper::stopSleepAnimation() {
    wake = true;
    DisplayOn();
}

void DisplayHelper::showSleepAnimation() {
    DisplayOff();
}

void DisplayHelper::DisplayOn() {
    digitalWrite(LCD_BL, HIGH);
    gfx->writecommand(TFT_DISPON);
}

void DisplayHelper::DisplayOff() {
    gfx->writecommand(TFT_DISPOFF);
    digitalWrite(LCD_BL, LOW);
}

void DisplayHelper::InitDisplay() {
    // DisplayOn();
    gfx->init();
    gfx->setRotation(1);
    fillScreen();

    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);

    // gfx->setCursor(10, 10);
    // gfx->setTextColor(TFT_RED, TFT_BLACK);    
    // drawBatteryheart();
    gfx->fillRect( 15, 15, 10, 10, TFT_BLUE);
    gfx->println("Hello World!");
}

void DisplayHelper::PlayGif(const char* fname) {
    play = false;
    delay(100);
    if (gifData != nullptr)
        free(gifData);

    if (!loadGIFToMemory(fname)) {
        log_d("Failed to load GIF to memory");
        return;
    }

    if (!gif.open(gifData, gifSize, GIFDraw)) {
        log_d("Failed to open GIF from memory");
        free(gifData);
        return;
    }

    play = true;
    xTaskCreatePinnedToCore(this->PlayInfiniteThread, "Task1", 10000, this, 2 | portPRIVILEGE_BIT, &Task1, 0);
    fillScreen();
}

void DisplayHelper::fillScreen() {
    gfx->fillScreen(TFT_BLACK);
}

void DisplayHelper::printOnDisplay(char* text, int x, int y) {
    gfx->setCursor(x, y);
    gfx->setTextColor(TFT_RED, TFT_BLACK);
    gfx->fillRect(x, y, x + 110, y + 20, TFT_BLACK);
    gfx->println(text);
}

void DisplayHelper::drawHeart(int x, int y, uint16_t color) {
    gfx->fillRect(x + 15, y + 15, 10, 10, color);
    gfx->fillRect(x + 28, y + 15, 10, 10, color);
    gfx->fillRect(x + 22, y + 21, 10, 10, color);
}

void DisplayHelper::drawBatteryheart() {
    float volt = luluDog->batteryHelper->get_battery_voltage();
    int heartColor = TFT_RED;
    if (luluDog->batteryHelper->isCharging()) {
        heartColor = TFT_GREEN;
    }
    if (volt <= 3.1)
        drawHeart(0, 0, TFT_BLACK);
    else
        drawHeart(0, 0, heartColor);
    if (volt <= 3.5)
        drawHeart(30, 0, TFT_BLACK);
    else
        drawHeart(30, 0, heartColor);
    if (volt <= 3.7)
        drawHeart(60, 0, TFT_BLACK);
    else
        drawHeart(60, 0, heartColor);
}

void DisplayHelper::setVoltageBuf(float voltage) {
    if (luluDog->batteryHelper->isCharging()) {
        sprintf(voltageBuf, "B: %f V Charging", voltage);
    } else {
        sprintf(voltageBuf, "B: %f V", voltage);
    }
}

void DisplayHelper::GIFDraw(GIFDRAW* pDraw) {
    uint8_t* s;
    uint16_t* usPalette = pDraw->pPalette;
    int y = pDraw->iY + pDraw->y;
    int iWidth = pDraw->iWidth > gfx->width() ? gfx->width() : pDraw->iWidth;
    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) {
        for (int x = 0; x < iWidth; x++) {
            if (s[x] == pDraw->ucTransparent)
                s[x] = pDraw->ucBackground;
        }
        pDraw->ucHasTransparency = 0;
    }
    if (pDraw->ucHasTransparency) {
        uint8_t* pEnd = s + iWidth;
        int x = 0;
        int iCount = 0;
        while (x < iWidth) {
            s = pDraw->pPixels + x;
            gfx->drawPixel(pDraw->iX + x, y, usPalette[*s++]);
            x++;
        }
    } else {
        for (int x = 0; x < iWidth; x++) {
            gfx->drawPixel(pDraw->iX + x, y, usPalette[*s++]);
        }
    }
}

bool DisplayHelper::loadGIFToMemory(const char* filename) {
    fs::File file = LittleFS.open(filename, "r");
    if (!file) {
        log_d("Failed to open GIF file");
        return false;
    }

    gifSize = file.size();
    gifData = (uint8_t*)ps_malloc(gifSize);
    if (!gifData) {
        log_d("Failed to allocate memory for GIF");
        file.close();
        return false;
    }

    file.read(gifData, gifSize);
    file.close();
    return true;
}

void DisplayHelper::StopGif() {
    play = false;
}

void DisplayHelper::PlayInfiniteThread(void* _this) {
    ((DisplayHelper*)_this)->PlayInfiniteTask();
    vTaskDelete(NULL);
}

void DisplayHelper::PlayInfiniteTask() {
    int iter = 0;
    while (play) {
        int res = gif.playFrame(true, NULL);
        if (iter == 4) {
            drawBatteryheart();
            iter = 0;
        }
        if (res == -1) {
            log_d("play error");
            vTaskDelete(NULL);
            return;
        }
        if (res == 0) {
            gif.close();
            gif.open(gifData, gifSize, GIFDraw);
        }
        iter++;
    }
    log_d("play ended");
}