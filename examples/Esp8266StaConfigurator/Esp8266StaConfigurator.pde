#include "Esp8266EasyConfigurator.h"


// define webserver instance
Esp8266EasyConfigurator *easyPage = new Esp8266EasyConfigurator();


void setup() {
  // start serial.
  Serial.begin(115200);
   // some test :)
  easyPage->startServer();
  delay(1000);


//
String accessPoint = "ESP8266 - " + String(ESP.getChipId(),HEX);
//Serial.printf("\nAPName[ %s ] \n", accessPoint.c_str());

 // easyPage->setWiFiToAP(accessPoint);
}

void loop() {

  easyPage->loop();
  delay(5000);
  easyPage->printPrettyInfo();
 
}
