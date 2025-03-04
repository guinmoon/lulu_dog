#include "display_helper.h"
#include "lulu_dog.h"

#include "global_def.h"

// #include "1.c"
// #include "2.c"
// #include "3.c"

Arduino_DataBus *DisplayHelper::bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCK, LCD_MOSI);
Arduino_GFX *DisplayHelper::gfx = new Arduino_ST7789(bus, LCD_RST /* RST */,
                                                     1 /* rotation */, true /* IPS */, LCD_HEIGHT, LCD_WIDTH, 0, 20, 0, 0);
AnimatedGIF DisplayHelper::gif;

bool DisplayHelper::showMatrixAnimation = false;
DigitalRainAnimation<Arduino_GFX> DisplayHelper::matrix_effect = DigitalRainAnimation<Arduino_GFX>();
LuLuDog* DisplayHelper::luluDog;

DisplayHelper::DisplayHelper(LuLuDog *_luluDog)
{
    luluDog = _luluDog;
}

void DisplayHelper::stopSleepAnimation()
{
    wake = true;
    DisplayOn();
}

void DisplayHelper::showSleepAnimation()
{

    // while (!wake)
    // {

    // gfx->displayOn();
    // // gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)sleep1.pixel_data, IMG_WIDTH, IMG_HEIGHT);
    // drawBatteryheart();
    // delay(1000);
    // if (wake)
    //     break;
    // // gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)sleep2.pixel_data, IMG_WIDTH, IMG_HEIGHT);
    // drawBatteryheart();
    // delay(1000);
    // if (wake)
    //     break;
    // // gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)sleep3.pixel_data, IMG_WIDTH, IMG_HEIGHT);
    // drawBatteryheart();
    // delay(3000);
    // if (wake)
    //     break;
    DisplayOff();
    // delay(15000);
    // delay(300);
    // }
    // DisplayOn();
    // wake = false;
}

void DisplayHelper::DisplayOn()
{
    digitalWrite(LCD_BL, HIGH);
    DisplayHelper::gfx->displayOn();
}

void DisplayHelper::DisplayOff()
{
    DisplayHelper::gfx->displayOff();
    digitalWrite(LCD_BL, LOW);
}

void DisplayHelper::InitDisplay()
{
    if (!gfx->begin())
    {
        log_d("gfx->begin() failed!");
    }
    fillScreen();

    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);

    gfx->setCursor(10, 10);
    gfx->setTextColor(RED);
    gfx->println("Hello World!");
    // pTurboBuffer = (uint8_t *)heap_caps_malloc(TURBO_BUFFER_SIZE + (280 * 240), MALLOC_CAP_8BIT);
    // pFrameBuffer = (uint8_t *)heap_caps_malloc(280 * 240 * sizeof(uint16_t), MALLOC_CAP_8BIT);
}

// void *DisplayHelper::GIFAlloc(uint32_t u32Size)
// {
//     // return heap_caps_malloc(u32Size, MALLOC_CAP_SPIRAM);
//     return (uint8_t *)ps_malloc(u32Size);
// } /* GIFAlloc() */

void DisplayHelper::PlayGif(const char *fname)
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

    // gif.begin(GIF_PALETTE_RGB888);
    // gif.begin(GIF_PALETTE_RGB565_BE);

    if (!gif.open(gifData, gifSize, GIFDraw))
    {
        log_d("Failed to open GIF from memory");
        free(gifData);
        return;
    }
    // gif.setDrawType(GIF_DRAW_COOKED);
    // gif.setFrameBuf(pFrameBuffer); // for Turbo+cooked, we need to supply a full sized output framebuffer
    // gif.setTurboBuf(pTurboBuffer);
    // gif.allocFrameBuf(GIFAlloc);
    // gif.allocTurboBuf(GIFAlloc);

    play = true;
    xTaskCreatePinnedToCore(
        this->PlayInfiniteThread, /* Task function. */
        "Task1",                  /* name of task. */
        10000,                    /* Stack size of task */
        this,                     /* parameter of the task */
        2 | portPRIVILEGE_BIT,    /* priority of the task */
        &Task1,                   /* Task handle to keep track of created task */
        0);
    fillScreen();
}

void DisplayHelper::fillScreen()
{
    gfx->fillScreen(BLACK);
}

void DisplayHelper::printOnDisplay(char *text, int x, int y)
{
    //
    gfx->setCursor(x, y);
    gfx->setTextColor(RED);
    gfx->fillRect(x, y, x + 110, y + 20, 0);
    gfx->println(text);
}

void DisplayHelper::drawHeart(int x, int y, uint16_t color)
{
    gfx->fillRect(x + 15, y + 15, 10, 10, color);
    gfx->fillRect(x + 28, y + 15, 10, 10, color);
    gfx->fillRect(x + 22, y + 21, 10, 10, color);
}

void DisplayHelper::drawBatteryheart()
{
    // printOnDisplay(voltageBuf);
    float volt = luluDog->batteryHelper->get_battery_voltage();
    int heartColor = RED;
    if (luluDog->batteryHelper->isCharging())
    {
        heartColor = GREEN;
        // printOnDisplay(voltageBuf,10,50);
    }
    if (volt <= 3.1)
        drawHeart(0, 0, BLACK);
    else
        drawHeart(0, 0, heartColor);
    if (volt <= 3.5)
        drawHeart(30, 0, BLACK);
    else
        drawHeart(30, 0, heartColor);
    if (volt <= 3.7)
        drawHeart(60, 0, BLACK);
    else
        drawHeart(60, 0, heartColor);
}

void DisplayHelper::setVoltageBuf(float voltage)
{
    if (luluDog->batteryHelper->isCharging())
    {
        sprintf(voltageBuf, "B: %f V Charging", voltage);
    }
    else
    {
        sprintf(voltageBuf, "B: %f V", voltage);
    }
}

// void GIFDraw_turbo(GIFDRAW *pDraw)
// {
//   uint16_t *pPixels = (uint16_t*)pDraw->pPixels;
//   int xWidth = pDraw->iWidth;
//   int yPos =  pDraw->iY + pDraw->y;

//   // Используем более эффективный метод для Arduino_GFX
//   gfx->startWrite();
//   for (int x = 0; x < xWidth; x++) {
//     gfx->writePixel( pDraw->iX + x, yPos, pPixels[x]);
//   }
//   gfx->endWrite();
// }

void DisplayHelper::GIFDraw_24bit(GIFDRAW *pDraw)
{
    uint8_t *s;
    uint16_t *usPalette;
    uint8_t rgb24[320 * 3]; // Массив для хранения 24-битных данных
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

    // Преобразование и применение пикселей к основному изображению
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
            int offset = 0;
            while (c != ucTransparent && s < pEnd)
            {
                c = *s++;
                if (c == ucTransparent) // завершено, остановка
                {
                    s--; // назад, чтобы обработать как прозрачный
                }
                else // непрозрачный
                {
                    uint16_t color = usPalette[c];
                    // Преобразование RGB565 в RGB888
                    rgb24[offset++] = ((color >> 11) & 0x1F) << 3; // Red
                    rgb24[offset++] = ((color >> 5) & 0x3F) << 2;  // Green
                    rgb24[offset++] = (color & 0x1F) << 3;         // Blue
                    iCount++;
                }
            } // while

            if (iCount) // есть ли непрозрачные пиксели?
            {
                gfx->draw24bitRGBBitmap(pDraw->iX + x, y, rgb24, iCount, 1);
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
        int offset = 0;
        for (x = 0; x < iWidth; x++)
        {
            uint16_t color = usPalette[*s++];
            // Преобразование RGB565 в RGB888
            rgb24[offset++] = ((color >> 11) & 0x1F) << 3; // Red
            rgb24[offset++] = ((color >> 5) & 0x3F) << 2;  // Green
            rgb24[offset++] = (color & 0x1F) << 3;         // Blue
        }
        gfx->draw24bitRGBBitmap(pDraw->iX, y, rgb24, iWidth, 1);
    }
}

uint16_t DisplayHelper::usTemp[320];

void DisplayHelper::GIFDraw(GIFDRAW *pDraw)
{
    uint8_t *s = pDraw->pPixels;
    uint16_t *usPalette = pDraw->pPalette;
    int y = pDraw->iY + pDraw->y;
    int iWidth = pDraw->iWidth;
    if (pDraw->iWidth>gfx->width())
        iWidth = gfx->width();    

    // Handle background restoration
    if (pDraw->ucDisposalMethod == 2)
    {
        uint8_t ucBackground = pDraw->ucBackground;
        for (int x = 0; x < iWidth; x++)
        {
            if (s[x] == pDraw->ucTransparent)
                s[x] = ucBackground;
        }
        pDraw->ucHasTransparency = 0;
    }

    // Optimize transparent handling
    if (pDraw->ucHasTransparency)
    {
        uint8_t ucTransparent = pDraw->ucTransparent;
        uint16_t *d = usTemp;
        int x = 0;
        while (x < iWidth)
        {
            // Count non-transparent pixels
            int count = 0;
            while (x + count < iWidth && s[x + count] != ucTransparent)
            {
                usTemp[count] = usPalette[s[x + count]];
                count++;
            }
            
            // Draw non-transparent block
            if (count > 0)
            {
                gfx->draw16bitRGBBitmap(pDraw->iX + x, y, usTemp, count, 1);
                x += count;
            }
            
            // Skip transparent pixels
            while (x < iWidth && s[x] == ucTransparent)
                x++;
        }        
    }
    else
    {
        // Direct conversion for non-transparent images
        for (int x = 0; x < iWidth; x++)
        {
            usTemp[x] = usPalette[s[x]];
        }
        gfx->draw16bitRGBBitmap(pDraw->iX, y, usTemp, iWidth, 1);
    }
}

bool DisplayHelper::loadGIFToMemory(const char *filename)
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

void DisplayHelper::StopGif()
{
    play = false;
}

void DisplayHelper::PlayInfiniteThread(void *_this)
{
    ((DisplayHelper *)_this)->PlayInfiniteTask();
    vTaskDelete(NULL);
}

void DisplayHelper::PlayInfiniteTask()
{
    int iter = 0;
    while (play)
    {
        int res = gif.playFrame(true, NULL);
        if (iter == 4)
        {

            drawBatteryheart();
            iter = 0;
        }
        if (res == -1)
        {
            log_d("play error");
            vTaskDelete(NULL);
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
}

void DisplayHelper::MatrixAnimationThread(void* _this){
    while(showMatrixAnimation){
        matrix_effect.loop();
        delay(50);
    }
    vTaskDelete(NULL);
}

void DisplayHelper::ShowMatrixAnimation()
{
    showMatrixAnimation = true;
    xTaskCreatePinnedToCore(
        this->MatrixAnimationThread, /* Task function. */
        "Task1",                  /* name of task. */
        10000,                    /* Stack size of task */
        this,                     /* parameter of the task */
        2 | portPRIVILEGE_BIT,    /* priority of the task */
        &Task1,                   /* Task handle to keep track of created task */
        0);   
    
}

void DisplayHelper::StopMatrixAnimation(){
    showMatrixAnimation = false;
}

void DisplayHelper::InitMatrixAnimation()
{
    matrix_effect.init(gfx);    
}



void DisplayHelper::LvglDispFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    if (luluDog->lvglHelper->lvglExit){
        log_d("LVGL EXIT");
        lv_disp_flush_ready(disp);
        return;
    }
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
    gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
    log_d("%i, %i, %i, %i, %i, %i", gfx, area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

    lv_disp_flush_ready(disp);
}




//OK
// void  DisplayHelper::GIFDraw(GIFDRAW *pDraw)
// {
//     uint8_t *s;
//     uint16_t *d, *usPalette/*, usTemp[320]*/;
//     int x, y, iWidth;

//     iWidth = pDraw->iWidth;
//     if (iWidth > LCD_WIDTH)
//         iWidth = LCD_WIDTH;
//     usPalette = pDraw->pPalette;
//     y = pDraw->iY + pDraw->y; // current line

//     s = pDraw->pPixels;
//     if (pDraw->ucDisposalMethod == 2)
//     { // restore to background color
//         for (x = 0; x < iWidth; x++)
//         {
//             if (s[x] == pDraw->ucTransparent)
//                 s[x] = pDraw->ucBackground;
//         }
//         pDraw->ucHasTransparency = 0;
//     }
//     // Apply the new pixels to the main image
//     if (pDraw->ucHasTransparency)
//     { // if transparency used
//         uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
//         int x, iCount;
//         pEnd = s + iWidth;
//         x = 0;
//         iCount = 0; // count non-transparent pixels
//         while (x < iWidth)
//         {
//             c = ucTransparent - 1;
//             d = usTemp;
//             while (c != ucTransparent && s < pEnd)
//             {
//                 c = *s++;
//                 if (c == ucTransparent)
//                 {        // done, stop
//                     s--; // back up to treat it like transparent
//                 }
//                 else
//                 { // opaque
//                     *d++ = usPalette[c];
//                     iCount++;
//                 }
//             } // while looking for opaque pixels
//             if (iCount /*&& y > 30*/)
//             { // any opaque pixels?
//                 gfx.pushImage((pDraw->iX + x) + xOffset, y + yOffset, iCount, 1, (uint16_t *)usTemp);
//                 x += iCount;
//                 iCount = 0;
//             }
//             // no, look for a run of transparent pixels
//             c = ucTransparent;
//             while (c == ucTransparent && s < pEnd)
//             {
//                 c = *s++;
//                 if (c == ucTransparent)
//                     iCount++;
//                 else
//                     s--;
//             }
//             if (iCount)
//             {
//                 x += iCount; // skip these
//                 iCount = 0;
//             }
//         }
//     }
//     else
//     {
//         s = pDraw->pPixels;
//         // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
//         for (x = 0; x < iWidth; x++)
//             usTemp[x] = usPalette[*s++];
//         gfx.pushImage((pDraw->iX + x) + xOffset, y + yOffset, iWidth, 1, (uint16_t *)usTemp);
//     }
// }
// DisplayHelper displayHelper;