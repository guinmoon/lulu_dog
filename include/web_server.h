#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <FS.h>
#include <LittleFS.h>
// #include <AsyncFsWebServer.h>
#include <esp-fs-webserver.h>

class LuLuDog;

class LuLuWebServer
{
private:
    static LuLuDog* luluDog;
public:
    // static AsyncFsWebServer server;
    static FSWebServer* myWebServer;
    static WebServer* server;
    int testInt = 150;
    float testFloat = 123.456;
    LuLuWebServer(LuLuDog *_luluDog);
    ~LuLuWebServer();
    void Init();
    // static void handleLed(AsyncWebServerRequest *request);
    static void handleLed();
    // static void getFsInfo(fsInfo_t* fsInfo);
    static bool startFilesystem();
    static void ServerThread(void * _this);
};




#endif