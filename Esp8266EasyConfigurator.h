#ifndef _ESP8266_EASY_CONFIGURATOR
#define _ESP8266_EASY_CONFIGURATOR

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ArduinoJson.h>

#define SERVER_PORT 80

#define EASY_CONFIG_DEBUG true

class Esp8266EasyConfigurator {

  public:

    Esp8266EasyConfigurator();
    ~Esp8266EasyConfigurator();
    bool startServer();
    bool stopServer();
    bool isServerStarted();
    void loop();
    void printPrettyInfo();
    void setWiFiToAP(String accessPointName);
    String getMode();


  private:

    void handleConfigurations();
    void handleNotFound();
    void saveConfigAndRestart();
    void refreshIndex();
    void writeInfoToSerial(String msg);
    void tryToConnectWifi(int tryCount);
    

    ESP8266WebServer *server;
    String setupPage;
    bool isStarted;
    int tryCount;
    String ssid;
    String password;
    String accessPointName;
};

#endif

