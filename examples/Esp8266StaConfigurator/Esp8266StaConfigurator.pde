#include "Esp8266EasyConfigurator.h"


// define webserver instance
Esp8266EasyConfigurator *easyPage = new Esp8266EasyConfigurator();


void setup() {
  // start serial.
  Serial.begin(115200);
}

void loop() {
  // some test :)
  easyPage->startServer();
  delay(1000);

  easyPage->isServerStarted();
  delay(1000);

  easyPage->stopServer();
  delay(1000);

}
