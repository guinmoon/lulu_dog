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
int16_t TouchHelper::x[5];
int16_t TouchHelper::y[5];

TouchHelper::TouchHelper(LuLuDog *_luluDog)
{
    luluDog = _luluDog;
}

void TouchHelper::detectLongOrDoubleTap()
{
    unsigned long currentTime = millis();

    if (isPressed && !wasPressed)
    {
        // Начало нового нажатия
        pressStartTime = currentTime;
    }
    else if (!isPressed && wasPressed)
    {
        // Конец нажатия
        released = true;
        longPressActivated = false;
        if (currentTime - lastReleaseTime < doubleTapTimeout)
        {
            // luluDog->ShowMenu();
            this->doubleTapCallback(0,0);
            log_d("Double Tap Detected");
        }
        lastReleaseTime = currentTime;
    }

    if (isPressed && wasPressed && (currentTime - pressStartTime >= longPressThreshold))
    {
        log_d("Long Press Detected");
        if (!longPressActivated)
        {
            this->longPressCallback(0,0);
            // luluDog->luluCharacter->DoSceneReact(x[0], y[0]);
            // luluDog->ShowMenu();
        }
        longPressActivated = true;

        pressStartTime = currentTime + 999999; // Исключаем повторное обнаружение длительного нажатия
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
    uint8_t touched = touch.getPoint(y, x, touch.getSupportTouchPoint());
    if (touched)
    {
        data->state = LV_INDEV_STATE_PR;
        /* Set the coordinates with some debounce */
        if (x[0] >= 0 && y[0] >= 0)
        {
            data->point.x = x[0];
            data->point.y = LCD_HEIGHT - y[0];
            log_d("x:%i y:%i ", x[0], LCD_HEIGHT - y[0]);
            // USBSerial.printf("Data x: %d, Data y: %d\n", touchX, touchY);
        }
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
        if (suspended){
            delay(500);
            continue;
        }
        if (isPressed)
        {
            uint8_t touched = touch.getPoint(y, x, touch.getSupportTouchPoint());
            if (touched)
            {
                released = false;
                for (int i = 0; i < touched; ++i)
                {
                    log_d("x[%i]:%i y[%i]:%i ", i, x[i], i, y[i]);
                    // if (x[i])
                }
            }
        }
        // Вызов функции определения длительного или двойного касания
        detectLongOrDoubleTap();
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

// TouchHelper touchHelper;