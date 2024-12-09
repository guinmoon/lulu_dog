#include <Arduino.h>
#include "Arduino_GFX_Library.h"
#include "pin_config.h"
#include <Wire.h>
#include <AnimatedGIF.h>
#include "homer_small_car.h"
#include "LittleFS.h"
// #include "HWCDC.h"

// HWCDC USBSerial;

Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCK, LCD_MOSI);

Arduino_GFX *gfx = new Arduino_ST7789(bus, LCD_RST /* RST */,
                                      3 /* rotation */, true /* IPS */, LCD_WIDTH, LCD_HEIGHT, 0, 0, 0, 20);

AnimatedGIF gif;
// File gifFile;
uint8_t *gifData = nullptr;
int32_t gifSize = 0;
// SPILCD lcd;

#define DISPLAY_WIDTH 280

void GIFDraw(GIFDRAW *pDraw)
{
    uint8_t *s;
    uint16_t *d, *usPalette, usTemp[320];
    int x, y, iWidth;

    usPalette = pDraw->pPalette;
    y = pDraw->iY + pDraw->y; // текущая линия
    iWidth = pDraw->iWidth;
    if (iWidth > gfx->width())
        iWidth = gfx->width();
    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) // восстановление фона
    {
        for (x = 0; x < iWidth; x++)
        {
            if (s[x] == pDraw->ucTransparent)
                s[x] = pDraw->ucBackground;
        }
        pDraw->ucHasTransparency = 0;
    }
    // Применение новых пикселей к основному изображению
    if (pDraw->ucHasTransparency) // если используется прозрачность
    {
        uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
        int x, iCount;
        pEnd = s + iWidth;
        x = 0;
        iCount = 0; // подсчет непрозрачных пикселей
        while (x < iWidth)
        {
            c = ucTransparent - 1;
            d = usTemp;
            while (c != ucTransparent && s < pEnd)
            {
                c = *s++;
                if (c == ucTransparent) // завершено, остановка
                {
                    s--; // назад, чтобы обработать как прозрачный
                }
                else // непрозрачный
                {
                    *d++ = usPalette[c];
                    iCount++;
                }
            } // while
            if (iCount) // есть ли непрозрачные пиксели?
            {
                gfx->draw16bitRGBBitmap(pDraw->iX + x, y, usTemp, iCount, 1);
                x += iCount;
                iCount = 0;
            }
            // нет, искать последовательность прозрачных пикселей
            c = ucTransparent;
            while (c == ucTransparent && s < pEnd)
            {
                c = *s++;
                if (c == ucTransparent)
                    iCount++;
                else
                    s--;
            }
            if (iCount)
            {
                x += iCount; // пропустить их
                iCount = 0;
            }
        }
    }
    else
    {
        s = pDraw->pPixels;
        // Преобразование 8-битных пикселей через палитру RGB565
        for (x = 0; x < iWidth; x++)
            usTemp[x] = usPalette[*s++];
        gfx->draw16bitRGBBitmap(pDraw->iX, y, usTemp, iWidth, 1);
    }
}

bool loadGIFToMemory(const char *filename)
{
    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        Serial.println("Failed to open GIF file");
        return false;
    }

    gifSize = file.size();
    gifData = (uint8_t *)malloc(gifSize);

    if (!gifData)
    {
        Serial.println("Failed to allocate memory for GIF");
        file.close();
        return false;
    }

    file.read(gifData, gifSize);
    file.close();
    return true;
}


void setup(void)
{
    USBSerial.begin(115200);
    // USBSerial.setDebugOutput(true);
    // while(!USBSerial);
    USBSerial.println("Arduino_GFX Hello World example");

    // Init Display
    if (!gfx->begin())
    {
        USBSerial.println("gfx->begin() failed!");
    }
    gfx->fillScreen(BLACK);

    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);

    gfx->setCursor(10, 10);
    gfx->setTextColor(RED);
    gfx->println("Hello World!");

    delay(500);

    if (!LittleFS.begin())
    {
        USBSerial.println("Failed to mount LittleFS");
        return;
    }

    if (!loadGIFToMemory("/goma-cat-on-top.gif"))
    {
        USBSerial.println("Failed to load GIF to memory");
        return;
    }

    if (!gif.open(gifData, gifSize, GIFDraw))
    {
        Serial.println("Failed to open GIF from memory");
        free(gifData);
        return;
    }

}

void playInfinite()
{
    while (true)
    {
        int res  = gif.playFrame(true, NULL);
        if (res == -1)
        {
            USBSerial.println("play error");
            return;
        }
        if (res == 0)
        {
            gif.close();
            gif.open(gifData, gifSize, GIFDraw);
            break;
        }
    }
}

void loop()
{
    playInfinite();
    // USBSerial.println("loop");
    // delay(1000);
}