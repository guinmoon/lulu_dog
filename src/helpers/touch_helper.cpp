#include "touch_helper.h"
#include "global_def.h"
#include "lulu_dog.h"
#include "TouchDrvCSTXXX.hpp"

// TouchDrvCSTXXX touch;
// int16_t x[5], y[5];
// bool isPressed = false;

// unsigned long lastTouchTime = 0;
// bool longPressDetected = false;
// bool doubleTapDetected = false;

// bool wasPressed = false;

// unsigned long pressStartTime = 0;
// unsigned long lastReleaseTime = 0;

// // Константы для настройки временных интервалов
// const unsigned long longPressThreshold = 1500; // 1000 мс (1 секунда)
// const unsigned long doubleTapTimeout = 800;

bool TouchHelper::isPressed = false;
TouchDrvCSTXXX TouchHelper::touch;
int16_t TouchHelper::x[6];
int16_t TouchHelper::y[6];

TouchHelper::TouchHelper(LuLuDog *_luluDog)
{
    luluDog = _luluDog;
}

void TouchHelper::detectLongOrDoubleTap(int x, int y)
{
    unsigned long currentTime = millis();

    if (!longPressActivated && !singleDetected && released && currentTime - lastReleaseTime > longPressThreshold + 200)
    {

        log_d("currentTime - lastReleaseTime: %lu \n%lu %lu", currentTime - lastReleaseTime, currentTime, lastReleaseTime);
        singleDetected = true;
        luluDog->dogEvents->EmitDogEvent(luluDog->dogEvents->BuildTouchEvent(x, y, 1));
    }
    if (isPressed && !wasPressed)
    {
        // Начало нового нажатия
        pressStartTime = currentTime;
    }
    else if (!isPressed && wasPressed)
    {
        // Конец нажатия
        released = true;
        if (longPressActivated)
        {
            // Это нужно чтобы после долгого нажатия не срабатывало 1-го  нажатие
            singleDetected = true;
            isPressed = false;
            wasPressed = false;
            released = false;
            pressStartTime = currentTime + 99999;
        }
        longPressActivated = false;

        if (currentTime - lastReleaseTime < doubleTapTimeout)
        {
            // Обнаружено двойное нажатие
            singleDetected = true;
            isPressed = false;
            wasPressed = false;
            luluDog->dogEvents->EmitDogEvent(luluDog->dogEvents->BuildTouchEvent(x, y, 2));
        }
        else
        {
            singleDetected = false; // Сбрасываем, так как второго касания не было
            // luluDog->dogEvents->EmitDogEvent(luluDog->dogEvents->BuildTouchEvent(x, y, 1));
        }
        lastReleaseTime = currentTime;
    }

    if (isPressed && wasPressed && !longPressActivated &&
        (currentTime - pressStartTime >= longPressThreshold))
    {

        luluDog->dogEvents->EmitDogEvent(luluDog->dogEvents->BuildTouchEvent(x, y, LONG_PRESS_T_COUNT));
        longPressActivated = true;
        // Исключаем повторное обнаружение длительного нажатия
        pressStartTime = currentTime + 99999;
    }

    wasPressed = isPressed;
}

void TouchHelper::TouchReadThread(void *_this)
{
    ((TouchHelper *)_this)->TouchReadTask();
    vTaskDelete(NULL);
}

void TouchHelper::LVGLTouchpadRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    uint8_t touched = touch.getPoint(y, x, /*touch.getSupportTouchPoint()*/ 1);
    if (touched && x[0] >= 0 && y[0] >= 0)
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = x[0];
        data->point.y = LCD_HEIGHT - y[0];
        log_d("x:%i y:%i ", x[0], LCD_HEIGHT - y[0]);
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

void TouchHelper::TouchReadTask()
{
    while (true)
    {
        if (suspended)
        {
            delay(500);
            continue;
        }

        if (isPressed)
        {
            uint8_t touched = touch.getPoint(y, x, /*touch.getSupportTouchPoint()*/ 1);
            if (touched)
            {
                released = false;
                log_d("touch event");
            }
        }
        // Вызов функции определения длительного или двойного касания
        detectLongOrDoubleTap(x[0], y[0]);
        isPressed = false;
        delay(30);
    }
}

void TouchHelper::InitTouch()
{

    pinMode(TP_RST, OUTPUT);
    digitalWrite(TP_RST, LOW);
    delay(30);
    digitalWrite(TP_RST, HIGH);
    delay(50);
    touch.setPins(TP_RST, TP_INT);
    bool result = touch.begin(Wire, CST816_SLAVE_ADDRESS, IIC_SDA, IIC_SCL);
    if (result == false)
    {
        while (1)
        {
            log_d("Failed to initialize CST series touch, please check the connection...");
            delay(1000);
        }
    }
    log_d("Touch Model : %i", touch.getModelName());
    attachInterrupt(TP_INT, []()
                    { isPressed = true; }, FALLING);

    xTaskCreatePinnedToCore(
        this->TouchReadThread, /* Task function. */
        "Task5",               /* name of task. */
        10000,                 /* Stack size of task */
        this,                  /* parameter of the task */
        tskIDLE_PRIORITY,      /* priority of the task */
        NULL,                  /* Task handle to keep track of created task */
        1);
}

void TouchHelper::switchToLVGLTask(bool toLvgl)
{
    if (toLvgl)
    {
        suspended = true;
    }
    else
    {
        suspended = false;
        isPressed = false;
    }
}

// TouchHelper touchHelper;