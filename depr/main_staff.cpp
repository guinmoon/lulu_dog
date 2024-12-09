#include <Arduino.h>
#include "Arduino_GFX_Library.h"
#include "pin_config.h"
#include <Wire.h>
#include <AnimatedGIF.h>
#include "homer_small_car.h"
#include "LittleFS.h"
#include "gif_helper.h"
#include "commands.h"
// #include <CST816_TouchLib.h>
// #include "Arduino_DriveBus_Library.h"
// #include <ESP32Servo.h>

// Servo servo1;
// Servo servo2;
// Servo servo3;
// Servo servo4;
// HWCDC USBSerial;

// Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCK, LCD_MOSI);

// Arduino_GFX *gfx = new Arduino_ST7789(bus, LCD_RST /* RST */,
//                                       3 /* rotation */, true /* IPS */, LCD_WIDTH, LCD_HEIGHT, 0, 0, 0, 20);

// TaskHandle_t Task1;

// AnimatedGIF gif;
// // File gifFile;
// uint8_t *gifData = nullptr;
// int32_t gifSize = 0;
// SPILCD lcd;

// // Глобальные массивы для управления сервоприводами
// int targetPosition[4] = {90, 90, 90, 90};      // Целевые углы для каждого сервопривода
// int currentPosition[4] = {0, 0, 0, 0};         // Текущие позиции
// int moveSpeed[4] = {1, 1, 1, 1};               // Скорости перемещения

// std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
//   std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

// void Arduino_IIC_Touch_Interrupt(void);

// std::unique_ptr<Arduino_IIC> CST816T(new Arduino_CST816x(IIC_Bus, CST816T_DEVICE_ADDRESS,
//                                                          TP_RST, TP_INT, Arduino_IIC_Touch_Interrupt));

// void Arduino_IIC_Touch_Interrupt(void) {
//   CST816T->IIC_Interrupt_Flag = true;
// }

// void receiveEvent(int howMany)
// {
//     Serial.println("On Recive:");
//     char buf[200];
//     while (Wire.available() >= 3)
//     {
//         int servoID = Wire.read();
//         int pos = Wire.read();
//         int spd = Wire.read();
//         sprintf(buf, "Recived: %i %i %i", servoID, pos, spd);
//         Serial.println(buf);
//     }
// }

void setServoPosition(int servoID, int targetPos, int speed)
{
    Wire1.beginTransmission(8); // Адрес ведомого устройства
    Wire1.write(servoID);
    Wire1.write(targetPos);
    Wire1.write(speed);
    Wire1.endTransmission();
    log_d("Sended.");
}

void sendCommand(int command)
{
    Wire1.beginTransmission(8); // Адрес ведомого устройства
    Wire1.write(command);
    Wire1.endTransmission();
    log_d("Sended.");
}

void sendCommand(int command, int arg1)
{
    Wire1.beginTransmission(8); // Адрес ведомого устройства
    Wire1.write(command);
    Wire1.write(arg1);
    Wire1.endTransmission();
    log_d("Sended.");
}

// MDO::DummyTouchSubscriber oTouchSubriber;
// MDO::CST816Touch oTouch;

void touchReadTask(void *params)
{
    while (true)
    {
        int val = digitalRead(2);
        if (val != 0)
        {
            log_d("loop %d", val);
            int choice = random(5);
            switch (choice)
            {
            case 0:
                sendCommand(COMMAND_SET_TAIL_SPEED, 4);
                break;
            case 1:
                sendCommand(COMMAND_SET_TAIL_SPEED, 4);
                sendCommand(COMMAND_SIT, 4);
                playGif("/dog1_r.gif");
                break;
            case 2:
                sendCommand(COMMAND_SET_TAIL_SPEED, 0);
                sendCommand(COMMAND_STAND, 4);
                playGif("/goma-cat-on-top.gif");
                break;
            case 3:
                sendCommand(COMMAND_SET_TAIL_SPEED, 0);
                sendCommand(COMMAND_LAYDOWN, 4);
                playGif("/goma-cat-on-top.gif");
                break;
            case 4:
                sendCommand(COMMAND_SET_TAIL_SPEED, 0);
                sendCommand(COMMAND_STAND, 4);
                playGif("/dog1_r.gif");
                break;
            default:
                sendCommand(COMMAND_SET_TAIL_SPEED, 7);
                sendCommand(COMMAND_STAND, 4);
                playGif("/dog1_r.gif");
                break;
            }
        }
    }
    vTaskDelete(NULL);
}
TaskHandle_t Task2;
// const int inputPin = 35;   // Define GPIO36 as input
// const int outputPin = 19;

void setup(void)
{
    USBSerial.begin(115200);
    // Wire.begin(13, 14);
    Wire1.begin(17, 18);

    log_d("Arduino_GFX Hello World example");

    // Init Display
    InitDisplay();

    // delay(500);

    if (!LittleFS.begin())
    {
        log_d("Failed to mount LittleFS");
        return;
    }

    psramInit();
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());

    // ESP32PWM::allocateTimer(0);
    // servo1.setPeriodHertz(50);
    // servo1.attach(11, 500, 2300);

    // Wire.begin(IIC_SDA, IIC_SCL, 400000);
    // Wire.setClock(400000);
    // while (!oTouch.begin(Wire, &oTouchSubriber,TP_INT,0x15,TP_RST)) {
    // 	USBSerial.println("Touch screen initialization failed..");
    // 	delay(1000);
    // }
    // MDO::CST816Touch::device_type_t eDeviceType;
    // if (oTouch.getDeviceType(eDeviceType)) {
    // 	USBSerial.print("Device is of type: ");
    // 	USBSerial.println(MDO::CST816Touch::deviceTypeToString(eDeviceType));
    // }
    // while (CST816T->begin() == false) {
    //     USBSerial.println("CST816T initialization fail");
    //     delay(2000);
    // }
    // CST816T->begin();
    // CST816T->IIC_Write_Device_State(CST816T->Arduino_IIC_Touch::Device::TOUCH_DEVICE_INTERRUPT_MODE,
    //                               CST816T->Arduino_IIC_Touch::Device_Mode::TOUCH_DEVICE_INTERRUPT_PERIODIC);

    // playGif("/goma-cat-on-top.gif");
    playGif("/dog1_r.gif");
    // ESP32PWM::allocateTimer(0);
    // ESP32PWM::allocateTimer(1);
    // ESP32PWM::allocateTimer(2);
    // ESP32PWM::allocateTimer(3);

    // // Настройка сервоприводов
    // servo1.setPeriodHertz(50); // стандартные сервоприводы работают на 50 Гц
    // servo2.setPeriodHertz(50);
    // servo3.setPeriodHertz(50);
    // servo4.setPeriodHertz(50);

    // // servo1.attach(10, 500, 2400);  // Подключение сервопривода к пину 13
    // // servo2.attach(11, 500, 2400);  // Подключение сервопривода к пину 14
    // // servo3.attach(17, 500, 2400); // Подключение сервопривода к пину 15
    // // servo4.attach(18, 500, 2400);
    // servo1.attach(2, 500, 2400);  // Подключение сервопривода к пину 13
    // servo2.attach(3, 500, 2400);  // Подключение сервопривода к пину 14
    // servo3.attach(17, 500, 2400); // Подключение сервопривода к пину 15
    // servo4.attach(18, 500, 2400);
    // pinMode(inputPin, INPUT);
    // pinMode(outputPin, OUTPUT);
    // digitalWrite(outputPin, HIGH);
    pinMode(2, INPUT);

    // xTaskCreate(
    //     touchReadTask, /* Task function. */
    //     "Task2",      /* name of task. */
    //     20000,        /* Stack size of task */
    //     NULL,         /* parameter of the task */
    //     2,            /* priority of the task */
    //     &Task2);
    xTaskCreatePinnedToCore(
        touchReadTask, /* Task function. */
        "Task2",       /* name of task. */
        10000,         /* Stack size of task */
        NULL,          /* parameter of the task */
        2,             /* priority of the task */
        NULL,          /* Task handle to keep track of created task */
        1);
    // sendCommand(COMMAND_SET_TAIL_SPEED,2);
    // delay(2000);
    // sendCommand(COMMAND_SET_TAIL_SPEED,4);
    // delay(2000);
    // sendCommand(COMMAND_SET_TAIL_SPEED,0);
    // delay(2000);
}

void loop()
{

    // log_d("oTouch.control: %d", oTouch.control());
    // log_d("loop() running on core %d", xPortGetCoreID());
    // int32_t touchX = CST816T->IIC_Read_Device_Value(CST816T->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
    // int32_t touchY = CST816T->IIC_Read_Device_Value(CST816T->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

    // USBSerial.printf("Touch X:%d Y:%d\n", touchX, touchY);
    // sendCommand(COMMAND_SIT);
    // delay(2000);
    // sendCommand(COMMAND_STAND);
    // delay(2000);
    // setServoPosition(0, 180, 3);

    // log_d(/*"Total heap: %d", */ESP.getHeapSize());
    // log_d(/*"Free heap: %d", */ESP.getFreeHeap());
    // log_d(/*"Total PSRAM: %d",*/ ESP.getPsramSize());
    // log_d(/*"Free PSRAM: %d", */ESP.getFreePsram());
    // tone(BUZZER_PIN, 800);
    // delay(200);
    // noTone(BUZZER_PIN);
    // playInfinite();
    // log_d("loop");
    // byte n = SUART.available();
    // if (n != 0)
    // {
    //     char ch = SUART.read();
    //     USBlog_d(ch);
    // }
    // for (int angle = 0; angle <= 180; angle++)
    // {
    //     servo1.write(angle);
    //     servo2.write(angle);
    //     servo3.write(angle);
    //     servo4.write(angle);

    //     delay(10 ); // Задержка для плавного поворота
    // }

    // for (int angle = 180; angle >= 0; angle--)
    // {
    //     servo1.write(angle);
    //     servo2.write(angle);
    //     servo3.write(angle);
    //     servo4.write(angle);

    //     delay(10); // Задержка для плавного поворота
    // }
    // servo1.write(0);
    // delay(1000);
    // servo1.write(180);

    // int val = digitalRead(2);
    // if (val != 0)
    // {
    //     log_d("loop %d", val);
    //     int choice = random(5);
    //     switch (choice)
    //     {
    //     case 0:
    //         sendCommand(COMMAND_SET_TAIL_SPEED, 4);
    //         break;
    //     case 1:
    //         sendCommand(COMMAND_SET_TAIL_SPEED, 4);
    //         sendCommand(COMMAND_SIT, 4);
    //         playGif("/dog1_r.gif");
    //         break;
    //     case 2:
    //         sendCommand(COMMAND_SET_TAIL_SPEED, 0);
    //         sendCommand(COMMAND_STAND, 4);
    //         playGif("/goma-cat-on-top.gif");
    //         break;
    //     case 3:
    //         sendCommand(COMMAND_SET_TAIL_SPEED, 0);
    //         sendCommand(COMMAND_LAYDOWN, 4);
    //         playGif("/goma-cat-on-top.gif");
    //         break;
    //     case 4:
    //         sendCommand(COMMAND_SET_TAIL_SPEED, 0);
    //         sendCommand(COMMAND_STAND, 4);
    //         playGif("/dog1_r.gif");
    //         break;
    //     default:
    //         sendCommand(COMMAND_SET_TAIL_SPEED, 7);
    //         sendCommand(COMMAND_STAND, 4);
    //         playGif("/dog1_r.gif");
    //         break;
    //     }
    // }
    log_d("loop");
    delay(1000);
}