#include "Esp8266EasyConfigurator.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


Esp8266EasyConfigurator::Esp8266EasyConfigurator() {


  isStarted = false;
  tryCount = 0;
  server = new ESP8266WebServer(SERVER_PORT);
}

Esp8266EasyConfigurator::~Esp8266EasyConfigurator() {
}

bool Esp8266EasyConfigurator::startServer() {

  writeInfoToSerial("startServer() called.");
  return false;
}


bool Esp8266EasyConfigurator::stopServer() {

  writeInfoToSerial("stopServer() called.");
  return false;
}


bool Esp8266EasyConfigurator::isServerStarted() {

  writeInfoToSerial("isServerStarted() called.");
  return false;
}


void Esp8266EasyConfigurator::writeInfoToSerial(String msg) {
  
  if (EASY_CONFIG_DEBUG && Serial) {
    Serial.println(msg);
  }
}



