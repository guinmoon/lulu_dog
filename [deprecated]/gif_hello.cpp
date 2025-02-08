#include <Arduino.h>
#include "Arduino_GFX_Library.h"
#include "pin_config.h"
#include <Wire.h>
#include <AnimatedGIF.h>
#include "homer_small_car.h"
// #include "HWCDC.h"

// HWCDC USBSerial;

Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCK, LCD_MOSI);

Arduino_GFX *gfx = new Arduino_ST7789(bus, LCD_RST /* RST */,
                                      0 /* rotation */, true /* IPS */, LCD_WIDTH, LCD_HEIGHT, 0, 20, 0, 0);

AnimatedGIF gif;
// SPILCD lcd;

#define DISPLAY_WIDTH 240

void setup(void) {
  USBSerial.begin(115200);
  // USBSerial.setDebugOutput(true);
  // while(!USBSerial);
  USBSerial.println("Arduino_GFX Hello World example");

  // Init Display
  if (!gfx->begin()) {
    USBSerial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(BLACK);

  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);

  gfx->setCursor(10, 10);
  gfx->setTextColor(RED);
  gfx->println("Hello World!");

  delay(5000);  // 5 seconds
}


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
      while(x < iWidth)
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





void loop() {
  if (gif.open((uint8_t *)homer_car_small, sizeof(homer_car_small), GIFDraw))
  {
    Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    while (gif.playFrame(true, NULL))
    {
    }
    gif.close();
  }
}