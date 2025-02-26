#include <FS.h>
#include <LittleFS.h>
// #include <AsyncFsWebServer.h>
#include <esp-fs-webserver.h>
#include "web_server.h"
#include "lulu_dog.h"
#include "global_def.h"
#include <WiFi.h>
// #ifndef LED_BUILTIN
// #define LED_BUILTIN 2
// #endif
// const uint8_t ledPin = LED_BUILTIN;

LuLuDog *LuLuWebServer::luluDog;
// AsyncFsWebServer LuLuWebServer::server(80, LittleFS, "myServer");
WebServer *LuLuWebServer::server;
FSWebServer *LuLuWebServer::myWebServer;
bool LuLuWebServer::serverRunning = false;

LuLuWebServer::LuLuWebServer(LuLuDog *_luluDog)
{
    this->luluDog = _luluDog;
    
}

LuLuWebServer::~LuLuWebServer()
{
}

// FILESYSTEM INIT
bool LuLuWebServer::startFilesystem()
{
    if (LittleFS.begin())
    {
        File root = LittleFS.open("/", "r");
        File file = root.openNextFile();
        while (file)
        {
            log_d("FS File: %s, size: %d\n", file.name(), file.size());
            file = root.openNextFile();
        }
        return true;
    }
    else
    {
        log_d("ERROR on mounting filesystem. It will be reformatted!");
        //   LittleFS.format();
        //   ESP.restart();
    }
    return false;
}

// void LuLuWebServer::getFsInfo(fsInfo_t *fsInfo)
// {
//     fsInfo->fsName = "LittleFS";
//     fsInfo->totalBytes = LittleFS.totalBytes();
//     fsInfo->usedBytes = LittleFS.usedBytes();
// }

void LuLuWebServer::handleLed()
{
    // If new led state is specified - http://xxx.xxx.xxx.xxx/led?val=1
    // if (myWebServer->webserver->hasArg("val"))
    // {
    //     int value = myWebServer->webserver->arg("val").toInt();
    //     digitalWrite(ledPin, value);
    // }
    // // else simple toggle the actual state
    // else
    // {
    //     digitalWrite(ledPin, !digitalRead(ledPin));
    // }
    String reply = "LED is now ";
    // reply += digitalRead(ledPin) ? "OFF" : "ON";
    myWebServer->webserver->send(200, "text/plain", reply);
}

void LuLuWebServer::Init()
{
    server = new WebServer(80);
    myWebServer = new FSWebServer(LittleFS, *server);
    startFilesystem();

    // Try to connect to flash stored SSID, start AP if fails after timeout
    IPAddress myIP = myWebServer->startWiFi(2000, "LuLu_RoboDog", "123456789");

    // Add custom page handlers to webserver
    myWebServer->addHandler("/led", HTTP_GET, handleLed);

    // Start webserver
    if (myWebServer->begin())
    {
        log_d("ESP Web Server started on IP Address: %s", myIP.toString().c_str());
        log_d("Open /setup page to configure optional parameters");
        log_d("Open /edit page to view and edit files");
        serverRunning = true;
        xTaskCreatePinnedToCore(
            this->ServerThread, /* Task function. */
            "Task15",           /* name of task. */
            10000,              /* Stack size of task */
            this,               /* parameter of the task */
            tskIDLE_PRIORITY,   /* priority of the task */
            NULL,               /* Task handle to keep track of created task */
            1);
    }
}

void LuLuWebServer::Deinit()
{
    serverRunning = false;
    myWebServer->webserver->stop();

    delete myWebServer;
    delete server;

    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
}

void LuLuWebServer::ServerThread(void *_this)
{
    while (serverRunning)
    {
        myWebServer->run();
        delay(50);
    }
    vTaskDelete(NULL);
}
// void LuLuWebServer::Init() {
//     // pinMode(ledPin, OUTPUT);
//     // Serial.begin(115200);
//     delay(1000);
//     if (startFilesystem()) {
//         log_d("LittleFS filesystem ready!");
//         // File config = server.getConfigFile("r");
//         // if (config) {
//         //     DynamicJsonDocument doc(config.size() * 1.33);
//         //     deserializeJson(doc, config);
//         //     testInt = doc["Test int variable"];
//         //     testFloat = doc["Test float variable"];
//         // }
//         // log_d("Stored \"testInt\" value: %d\n", testInt);
//         // log_d("Stored \"testFloat\" value: %3.2f\n", testFloat);
//     }
//     else
//         log_d("LittleFS error!");

//     IPAddress myIP = server.startWiFi(15000, "LuLu_RoboDog", "123456789");
//     WiFi.setSleep(WIFI_PS_NONE);
//     // server.addOptionBox("Custom options");
//     // server.addOption("Test int variable", testInt);
//     // server.addOption("Test float variable", testFloat);
//     // server.setSetupPageTitle("Simple Async ESP FS WebServer");

//     // Enable ACE FS file web editor and add FS info callback function
//     server.enableFsCodeEditor();
//     #ifdef ESP32
//     server.setFsInfoCallback(getFsInfo);
//     #endif

//     server.on("/led", HTTP_GET, handleLed);

//     // Start server
//     server.init();
//     log_d("Async ESP Web Server started on IP Address: ");
//     log_d("%s",myIP.toString().c_str());
//     log_d(
//         "This is \"simpleServer.ino\" example.\n"
//         "Open /setup page to configure optional parameters.\n"
//         "Open /edit page to view, edit or upload example or your custom webserver source files."
//     );

// }
