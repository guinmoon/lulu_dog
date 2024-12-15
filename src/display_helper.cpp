#include "display_helper.h"
#include "battery_helper.h"
// #include "BmpClass.h"
#include "1.c"
#include "2.c"
#include "3.c"
#define IMG_WIDTH 280
#define IMG_HEIGHT 240

#define DISPLAY_WIDTH 280

Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCK, LCD_MOSI);

Arduino_GFX *gfx = new Arduino_ST7789(bus, LCD_RST /* RST */,
                                      1 /* rotation */, true /* IPS */, LCD_WIDTH, LCD_HEIGHT, 0, 20, 0, 0);
AnimatedGIF gif;
// File gifFile;
uint8_t *gifData = nullptr;
int32_t gifSize = 0;
TaskHandle_t Task1;
char voltageBuf[15];

bool play = true;
bool wake = false;
// static BmpClass bmpClass;

// static void bmpDrawCallback(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
// {
//   // Serial.printf("Draw pos = %d, %d. size = %d x %d\n", x, y, w, h);
//   gfx->draw16bitRGBBitmap(x, y, bitmap, w, h);
// }

void stopSleepAnimation(){
    wake = true;
    gfx->displayOn();
}

void showSleepAnimation()
{

    while (!wake)
    {
        
        gfx->displayOn();
        gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)sleep1.pixel_data, IMG_WIDTH, IMG_HEIGHT);
        printOnDisplay(voltageBuf);
        delay(1000);
        if (wake)
            break;
        gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)sleep2.pixel_data, IMG_WIDTH, IMG_HEIGHT);
        printOnDisplay(voltageBuf);
        delay(1000);
        if (wake)
            break;
        gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)sleep3.pixel_data, IMG_WIDTH, IMG_HEIGHT);        
        printOnDisplay(voltageBuf);
        delay(3000);
        if (wake)
            break;
        gfx->displayOff();
        delay(7000);
    }
    wake = false;
}

void playGif(const char *fname)
{
    play = false;
    delay(100);
    if (gifData != nullptr)
        free(gifData);
    // gifData = nullptr;
    // gifSize = 0;

    if (!loadGIFToMemory(fname))
    {
        log_d("Failed to load GIF to memory");
        return;
    }

    if (!gif.open(gifData, gifSize, GIFDraw))
    {
        log_d("Failed to open GIF from memory");
        free(gifData);
        return;
    }
    play = true;
    xTaskCreatePinnedToCore(
        playInfinite, /* Task function. */
        "Task1",      /* name of task. */
        10000,        /* Stack size of task */
        NULL,         /* parameter of the task */
        1,            /* priority of the task */
        &Task1,       /* Task handle to keep track of created task */
        0);
    gfx->fillScreen(BLACK);
}

void InitDisplay()
{
    if (!gfx->begin())
    {
        log_d("gfx->begin() failed!");
    }
    gfx->fillScreen(BLACK);

    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);

    gfx->setCursor(10, 10);
    gfx->setTextColor(RED);
    gfx->println("Hello World!");
}

void fillScreen()
{
    gfx->fillScreen(BLACK);
}

void printOnDisplay(char *text)
{
    //
    gfx->setCursor(10, 10);
    gfx->setTextColor(RED);
    gfx->fillRect(10, 10, 80, 20, 0);
    gfx->println(text);
}



void setVoltageBuf(float voltage)
{
    sprintf(voltageBuf, "B: %f V", voltage);
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
    // gfx->setCursor(10, 10);
    // gfx->setTextColor(RED);
    // gfx->println(voltageBuf);
}

bool loadGIFToMemory(const char *filename)
{
    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        log_d("Failed to open GIF file");
        return false;
    }

    gifSize = file.size();
    log_d("Used PSRAM: %d", ESP.getPsramSize() - ESP.getFreePsram());
    gifData = (uint8_t *)ps_malloc(gifSize);
    log_d("GifSize: %d Used PSRAM: %d", gifSize, ESP.getPsramSize() - ESP.getFreePsram());
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());
    if (!gifData)
    {
        log_d("Failed to allocate memory for GIF");
        file.close();
        return false;
    }

    file.read(gifData, gifSize);
    file.close();
    return true;
}

void stopGif(){
    play = false;
}

void playInfinite(void *pvParameters)
{
    int iter = 0;
    while (play)
    {
        int res = gif.playFrame(true, NULL);
        if (iter == 4)
        {
            printOnDisplay(voltageBuf);
            iter = 0;
        }
        if (res == -1)
        {
            log_d("play error");
            return;
        }
        if (res == 0)
        {
            gif.close();
            gif.open(gifData, gifSize, GIFDraw);
        }
        iter++;
    }
    log_d("play ended");
    vTaskDelete(NULL);
}