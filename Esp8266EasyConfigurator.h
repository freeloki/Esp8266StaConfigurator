#ifndef _ESP8266_EASY_CONFIGURATOR
#define _ESP8266_EASY_CONFIGURATOR

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define SERVER_PORT 80

#define EASY_CONFIG_DEBUG true

class Esp8266EasyConfigurator {

  public:

    Esp8266EasyConfigurator();
    ~Esp8266EasyConfigurator();
    bool startServer();
    bool stopServer();
    bool isServerStarted();


  private:

    void handleConfigurations();
    void handleNotFound();
    void saveConfigAndRestart();
    void refreshIndex();
    void writeInfoToSerial(String msg);

    ESP8266WebServer *server;
    String setupPage;
    bool isStarted;
    int tryCount;
};

#endif

