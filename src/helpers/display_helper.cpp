#include "display_helper.h"
#include "lulu_dog.h"
#include <eyes_drawer.h>
#include "global_def.h"

// Arduino_DataBus *DisplayHelper::bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCK, LCD_MOSI);
// Arduino_GFX *DisplayHelper::gfx = new Arduino_ST7789(bus, LCD_RST /* RST */,
//                                                      1 /* rotation */, true /* IPS */, LCD_HEIGHT, LCD_WIDTH, 0, 20, 0, 0);

LGFX_MyDisplay *DisplayHelper::gfx;
LuLuEyes *DisplayHelper::luluEyes;
LGFX_Sprite *DisplayHelper::eyesSprite;
LGFX_Sprite *DisplayHelper::batterySprite;

AnimatedGIF DisplayHelper::gif;

bool DisplayHelper::showMatrixAnimation = false;
DigitalRainAnimation<LGFX_MyDisplay> DisplayHelper::matrix_effect = DigitalRainAnimation<LGFX_MyDisplay>();
LuLuDog *DisplayHelper::luluDog;
int DisplayHelper::xOffset = 0;
int DisplayHelper::yOffset = 0;

DisplayHelper::DisplayHelper(LuLuDog *_luluDog)
{
    luluDog = _luluDog;
    gfx = new LGFX_MyDisplay();
    batterySprite = new LGFX_Sprite(gfx);
    batterySprite->setPsram(true);
    batterySprite->createSprite(120, 40);
}

void DisplayHelper::stopSleepAnimation()
{
    wake = true;
    DisplayOn();
}

void DisplayHelper::showSleepAnimation()
{
    DisplayOff();
}

void DisplayHelper::DisplayOn()
{
    gfx->endWrite();
    gfx->setBrightness(255);
    digitalWrite(LCD_BL, HIGH);
    // DisplayHelper::gfx->displayOn();
}

void DisplayHelper::DisplayOff()
{
    // DisplayHelper::gfx->displayOff();
    gfx->endWrite();
    gfx->setBrightness(0);
    digitalWrite(LCD_BL, LOW);
}

// void Arduino_ST7789::displayOn(void)
// {
//   _bus->sendCommand(0x11);
//   delay(120);
// }

// void Arduino_ST7789::displayOff(void)
// {
//   _bus->sendCommand(0x10);
//   delay(120);
// }

void DisplayHelper::InitDisplay()
{
    // gfx->init();
    gfx->setRotation(1);

    if (!gfx->init())
    {
        log_d("gfx->begin() failed!");
    }
    fillScreen();

    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);

    gfx->setCursor(10, 10);
    gfx->setTextColor(TFT_RED);
    gfx->println("Hello World!");

    luluEyes = new LuLuEyes();    
    eyesSprite = new LGFX_Sprite(gfx);
    eyesSprite->setPsram(true);    
    eyesSprite->createSprite(gfx->width(), gfx->height());        

    luluEyes->begin(gfx->width(), gfx->height(), eyesSprite); // screen-width, screen-height, max framerate

    // Define some automated eyes behaviour
    luluEyes->setAutoblinker(ON, 3, 2); // Start auto blinker animation cycle -> bool active, int interval, int variation -> turn on/off, set interval between each blink in full seconds, set range for random interval variation in full seconds
    luluEyes->setIdleMode(ON, 2, 2);
    luluEyes->setSpacebetween(40);
    pauseEyes = true;
    xTaskCreatePinnedToCore(
        this->StartEyesUpdateThread, /* Task function. */
        "Task1",                     /* name of task. */
        10000,                       /* Stack size of task */
        this,                        /* parameter of the task */
        2 | portPRIVILEGE_BIT,       /* priority of the task */
        &Task1,                      /* Task handle to keep track of created task */
        0);
    
    // pTurboBuffer = (uint8_t *)heap_caps_malloc(TURBO_BUFFER_SIZE + (280 * 240), MALLOC_CAP_8BIT);
    // pFrameBuffer = (uint8_t *)heap_caps_malloc(280 * 240 * sizeof(uint16_t), MALLOC_CAP_8BIT);
}

void DisplayHelper::setIdleMode(bool enable){
    if (enable){
        this->luluEyes->setIdleMode(ON, 2, 2);
    }else{
        this->luluEyes->setIdleMode(OFF, 2, 2);
    }
}

void DisplayHelper::pauseResumeEyes(bool pause)
{
    this->pauseEyes = pause;
}

void DisplayHelper::StartEyesUpdateThread(void *_this)
{
    ((DisplayHelper *)_this)->EyesUpdateTask();
    vTaskDelete(NULL);
}

void DisplayHelper::EyesUpdateTask()
{
    while (true)
    {
        if (pauseEyes)
        {
            delay(100);
            continue;
        }
        luluEyes->update();
        delay(30);
    }
}

void DisplayHelper::SetEyePosition(int x, int y)
{
    // eyeLxNext = getScreenConstraint_X() / 2;
    //         eyeLyNext = 0;
    luluEyes->eyeLxNext = x;
    luluEyes->eyeLyNext = y;
}
// void *DisplayHelper::GIFAlloc(uint32_t u32Size)
// {
//     // return heap_caps_malloc(u32Size, MALLOC_CAP_SPIRAM);
//     return (uint8_t *)ps_malloc(u32Size);
// } /* GIFAlloc() */

void DisplayHelper::PlayGif(const char *fname)
{
    playGif = false;
    pauseResumeEyes(true);
    delay(100);
    if (gifData != nullptr)
        free(gifData);
    // gifData = nullptr;
    // gifSize = 0;

    if (!loadGIFToMemory(fname))
    {
        log_d("Failed to load GIF to memory play = false");
        drawBatteryheart();
        playGif = false;
        return;
    }

    // gif.begin(GIF_PALETTE_RGB888);
    gif.begin(GIF_PALETTE_RGB565_BE);

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

    playGif = true;
    xTaskCreatePinnedToCore(
        this->PlayInfiniteThread, /* Task function. */
        "Task1",                  /* name of task. */
        10000,                    /* Stack size of task */
        this,                     /* parameter of the task */
        2 | portPRIVILEGE_BIT,    /* priority of the task */
        &Task1,                   /* Task handle to keep track of created task */
        0);
    // fillScreen();
}

uint16_t DisplayHelper::usTemp[280];

// Turbo COOKED
//  void DisplayHelper::GIFDraw(GIFDRAW *pDraw)
//  {
//      gfx->startWrite();
//      // if (pDraw->y == 0)
//      // { // set the memory window (once per frame) when the first line is rendered
//          gfx->setAddrWindow(pDraw->iX, pDraw->iY, pDraw->iWidth, pDraw->iHeight);
//      // }
//      // For all other lines, just push the pixels to the display. We requested 'COOKED'big-endian RGB565 and
//      gfx->writePixels((uint16_t *)pDraw->pPixels, pDraw->iWidth);
//      gfx->endWrite();
//  }

// //ARUINO_GFX
void DisplayHelper::GIFDraw(GIFDRAW *pDraw)
{
    uint8_t *s = pDraw->pPixels;
    uint16_t *usPalette = pDraw->pPalette;
    int y = pDraw->iY + pDraw->y;
    int iWidth = pDraw->iWidth;
    if (iWidth > LCD_WIDTH)
    {
        iWidth = LCD_WIDTH;
    }
    gfx->startWrite();
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
                gfx->setAddrWindow((pDraw->iX + x) + xOffset, y + yOffset, count, 1);
                gfx->writePixels((uint16_t *)usTemp, count);
                // gfx->pushImage(pDraw->iX + x, y, count, 1,(uint16_t *)usTemp);
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
        // gfx->pushImage(pDraw->iX, y,  iWidth, 1,(uint16_t *)usTemp);
        gfx->setAddrWindow(pDraw->iX + xOffset, y + yOffset, iWidth, 1);
        gfx->writePixels(usTemp, iWidth);
    }
    gfx->endWrite();
}

void DisplayHelper::MemInfo()
{
    log_d("Used PSRAM: %d", ESP.getPsramSize() - ESP.getFreePsram());
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());
}

bool DisplayHelper::loadGIFToMemory(const char *filename)
{
    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        log_d("Failed to open GIF file %s ", filename);
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
    playGif = false;
    
}

void DisplayHelper::fillScreen()
{
    // gfx->fillScreen(BLACK);
    gfx->clear();
}

void DisplayHelper::printOnDisplay(char *text, int x, int y)
{
    //
    gfx->setCursor(x, y);
    gfx->setTextColor(TFT_RED);
    gfx->fillRect(x, y, x + 110, y + 20, 0);
    gfx->println(text);
}

void DisplayHelper::drawHeart(int x, int y, uint16_t color)
{
    batterySprite->fillRect(x + 15, y + 15, 10, 10, color);
    batterySprite->fillRect(x + 28, y + 15, 10, 10, color);
    batterySprite->fillRect(x + 22, y + 21, 10, 10, color);
}

void DisplayHelper::drawBatteryheart()
{
    // printOnDisplay(voltageBuf);
    float volt = luluDog->batteryHelper->get_battery_voltage();
    int heartColor = TFT_RED;
    if (luluDog->batteryHelper->isCharging())
    {
        heartColor = TFT_GREEN;
        // printOnDisplay(voltageBuf,10,50);
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
    
    batterySprite->pushSprite(0,0);
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

void DisplayHelper::PlayInfiniteThread(void *_this)
{
    ((DisplayHelper *)_this)->PlayInfiniteTask();
    vTaskDelete(NULL);
}

void DisplayHelper::PlayInfiniteTask()
{
    int iter = 0;
    while (playGif)
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
            log_d("play ended reopen");
            gif.close();
            gif.open(gifData, gifSize, GIFDraw);
        }
        iter++;
    }
    log_d("play ended");
}

void DisplayHelper::MatrixAnimationThread(void *_this)
{
    while (showMatrixAnimation)
    {
        matrix_effect.loop();
        delay(70);
    }
    vTaskDelete(NULL);
}

void DisplayHelper::ShowMatrixAnimation()
{
    if (!matrixEffectInited)
    {
        matrixEffectInited = true;
        InitMatrixAnimation();
        // delay(300);
    }
    showMatrixAnimation = true;
    xTaskCreatePinnedToCore(
        this->MatrixAnimationThread, /* Task function. */
        "Task1",                     /* name of task. */
        10000,                       /* Stack size of task */
        this,                        /* parameter of the task */
        2 | portPRIVILEGE_BIT,       /* priority of the task */
        &Task1,                      /* Task handle to keep track of created task */
        0);
}

void DisplayHelper::StopMatrixAnimation()
{
    showMatrixAnimation = false;
}

void DisplayHelper::InitMatrixAnimation()
{
    matrix_effect.init(gfx);
}

void DisplayHelper::LvglDispFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    if (luluDog->lvglHelper->lvglExit)
    {
        log_d("LVGL EXIT");
        lv_disp_flush_ready(disp);
        sleep(300);
        return;
    }
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    if (w <= 0 && h <= 0)
    {
        lv_disp_flush_ready(disp);
        return;
    }
    gfx->startWrite();
    gfx->setAddrWindow(area->x1, area->y1, w, h);
    gfx->writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
    gfx->endWrite();
    lv_disp_flush_ready(disp);
}

// DisplayHelper displayHelper;