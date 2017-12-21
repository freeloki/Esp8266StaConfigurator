#include "Esp8266EasyConfigurator.h"

#define NODE_CONFIG "node-configuration"


Esp8266EasyConfigurator::Esp8266EasyConfigurator() {


  isStarted = false;
  tryCount = 0;
  ssid="";
  password="";
  server = new ESP8266WebServer(SERVER_PORT);
}

Esp8266EasyConfigurator::~Esp8266EasyConfigurator() {
}

bool Esp8266EasyConfigurator::startServer() {

  writeInfoToSerial("startServer() called.");
   if (!isStarted) {
    server->on("/",[this](){
    this->refreshIndex();
    });
    server->on("/node-configuration", HTTP_POST, std::bind(&Esp8266EasyConfigurator::handleConfigurations, this));
    server->on("/save-configuration-and-restart",HTTP_POST,std::bind(&Esp8266EasyConfigurator::saveConfigAndRestart,this));
    server->onNotFound(std::bind(&Esp8266EasyConfigurator::handleNotFound,this));
    server->begin();
    
    String serverInfo = "HTTP Server started in [ " + getMode() + " ] mode ";
    Serial.println(serverInfo);
    isStarted = true;
  }
  return isStarted;
}


bool Esp8266EasyConfigurator::stopServer() {

  writeInfoToSerial("stopServer() called.");
  return false;
}

bool Esp8266EasyConfigurator::isServerStarted() {
  writeInfoToSerial("isServerStarted() called.");
  return this->isStarted;
}


void Esp8266EasyConfigurator::writeInfoToSerial(String msg) {
  
  if (EASY_CONFIG_DEBUG && Serial) {
    Serial.println(msg);
  }
}


void Esp8266EasyConfigurator::loop() {

  if (isStarted) {
    server->handleClient();
  }

}

void Esp8266EasyConfigurator::handleConfigurations() {

  if (server->hasArg(NODE_CONFIG)) {	
    Serial.println("Configuration received.");
    String configJsonMsg = server->arg(NODE_CONFIG);
    configJsonMsg.trim();
    char charBuffer[configJsonMsg.length()];
    configJsonMsg.toCharArray(charBuffer, configJsonMsg.length());
    const char *charPtr = configJsonMsg.c_str();
    StaticJsonBuffer<250> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(charPtr);
    if (!root.success()) {
      Serial.println("parseObject() failed");
      server->send (400, "text/plain", "Json parse failure." );
      return;
    } else {
      String ssid = root[NODE_CONFIG]["ssid"];
      String password = root[NODE_CONFIG]["password"];
      
      if (!WiFi.enableSTA(true)) {
        server->send (200, "text/plain", "Configuration received successfully. Restarting node..." );
        Serial.println("SSID:");
        Serial.println(ssid);
        Serial.println("PASSWORD:");
        Serial.println(password);
        WiFi.begin(ssid.c_str(),password.c_str());
      } else {
        server->send (500, "text/plain", "Configuration failure. Please try again." );
      }
    }
  } else {
    int count = server->args();
    Serial.println("Count ");
    Serial.println(count);
    server->send (400, "text/plain", "Bad request. Please try again." );
  }

}

void Esp8266EasyConfigurator::handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += ( server->method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";

  for ( uint8_t i = 0; i < server->args(); i++ ) {
    message += " " + server->argName ( i ) + ": " + server->arg ( i ) + "\n";
  }
  server->send ( 404, "text/plain", message );

}

void Esp8266EasyConfigurator::refreshIndex() {

      setupPage = "<!DOCTYPE html>\n"
      "<html>\n"
      "<script>\n"
    "function confirmDialog() {\n"
    "return confirm(\"Do you want to apply configs ? \");}\n"
    "</script>\n"
      "<style>\n"
      "form {\n"
      "    border: 3px solid #f1f1f1;\n"
      "    width: 50%;\n"
      "    margin: 0 auto;\n"
      "}\n"
      "\n"
      "input[type=text], input[type=password] {\n"
      "    width: 100%;\n"
      "    padding: 12px 20px;\n"
      "    margin: 8px 0;\n"
      "    display: inline-block;\n"
      "    border: 1px solid #ccc;\n"
      "    box-sizing: border-box;\n"
      "}\n"
      "\n"
      "button {\n"
      "    background-color: #4CAF50;\n"
      "    color: white;\n"
      "    padding: 14px 20px;\n"
      "    margin: 8px 0;\n"
      "    border: none;\n"
      "    cursor: pointer;\n"
      "    width: 100%;\n"
      "}\n"
      "\n"
      ".container {\n"
      "    padding: 16px;\n"
      "}\n"
      "\n"
      ".center {\n"
      "    text-align:center;\n"
      "}\n"
      "</style>\n"
      "<body>\n"
      "<h2 class=\"center\">ESP8266 Wifi Configuration</h2>\n"
      "<form action=\"save-configuration-and-restart\" method=\"post\" onsubmit=\"return confirmDialog();\">\n"
      "<div class=\"container\">\n"
      "  <label for=\"ssid\"><b>SSID:</b></label>\n"
      "  <input type=\"text\" placeholder=\"SSID\" name=\"ssid\" id=\"ssid\" value=\"";
      setupPage+= ssid;
      setupPage+="\">\n"
      "  <label for=\"password\"><b>Password:</b></label>\n"
      "  <input type=\"password\" placeholder=\"Enter Password\" name=\"password\"  id=\"password\" value=\"";
      setupPage+= password;
      setupPage+="\">\n"
      "  <button type=\"submit\">Save Configurations And Restart</button>\n"
      "<h3 class=\"center\"><a href=\"https://www.codegeni.us/#/\" target=\"_blank\">www.codegeni.us</a></h3>\n"
      "<h3 class=\"center\">Powered by Yavuz ERZURUMLU</h3>\n"
      "</div>\n"
      "</form>\n"
      "</body>\n"
      "</html>";
      server->send(200, "text/html", setupPage);
}

void Esp8266EasyConfigurator::saveConfigAndRestart(){

  ssid = server->arg("ssid");
  password = server->arg("password");
  if(ssid.length() > 0 && password.length() > 0 ){

      server->send (200, "text/plain", "Configuration received successfully. Restarting node..." );
      Serial.println("SSID:");
      Serial.println(ssid);
      Serial.println("Passwd:");
      Serial.println(password);
      
      writeInfoToSerial("Wifi mode:");
      Serial.println(WiFi.getMode());
      if(WiFi.getMode() != WIFI_STA){
      WiFi.mode(WIFI_STA);
      }
      WiFi.begin(ssid.c_str(),password.c_str());
      this->tryToConnectWifi(10);
     
      
  } else {
	  server->send (500, "text/plain", "Configuration failure. Check your parameters and try again." );
  }
}

void Esp8266EasyConfigurator::tryToConnectWifi(int tryCount){
	
	int tried = 0;
	 
	 if(tried < tryCount){
		while (WiFi.status() != WL_CONNECTED ) {
	
			  delay(500);
			  Serial.print(".");
              tried++;
	   }
     }else {
	    Serial.println("Max Tryout Reached. Restarting on AP Mode");
	    WiFi.mode(WIFI_AP);
	    delay(500);
	    Serial.println("Setting AP MODE.");
	      while(WiFi.getMode() != WIFI_AP){
	           Serial.print(".");
	           delay(200);
	     }


    Serial.println("Restarting...");
	ESP.restart();
	
    }
  
 }
 
 void Esp8266EasyConfigurator::printPrettyInfo(){
		   
		  if (Serial) {

			int mode = WiFi.getMode();
			if(mode == WIFI_OFF){
			   Serial.println("WIFI_OFF");
			}else if(mode == WIFI_STA){
			   Serial.println("WIFI_STA");
			}else if(mode == WIFI_AP){
			   Serial.println("WIFI_AP");
			}else if(mode == WIFI_AP_STA){
			   Serial.println("WIFI_AP_STA");
			}
			
			Serial.println("WiFi SSID:");
			Serial.println(WiFi.SSID());
       
			Serial.println("WiFi MAC:");
			byte mac[6];
			WiFi.macAddress(mac);
			Serial.print(mac[5],HEX);
			Serial.print(":");
			Serial.print(mac[4],HEX);
			Serial.print(":");
		    Serial.print(mac[3],HEX);
			Serial.print(":");
			Serial.print(mac[2],HEX);
			Serial.print(":");
			Serial.print(mac[1],HEX);
			Serial.print(":");
			Serial.println(mac[0],HEX);
 
        
        if(WiFi.getMode() == WIFI_STA){
			Serial.println("Local IP:");
			Serial.println(WiFi.localIP());
		}else if(WiFi.getMode() == WIFI_AP){
			Serial.println(WiFi.softAPIP());
		}
		
	}
	 
	 
	 WiFi.printDiag(Serial);
 }

void Esp8266EasyConfigurator::setWiFiToAP(String accessPointName){
	
	//if(WiFi.getMode != WIFI_AP){
	WiFi.mode(WIFI_AP);
	WiFi.softAP(accessPointName.c_str());
	
	Serial.println("Setting Access Point...");
	while(WiFi.getMode() != WIFI_AP){
		Serial.print(".");
		delay(200);
	}
	
	Serial.println("Done!");
   //}
}

String Esp8266EasyConfigurator::getMode(){
				Serial.println("WiFi Mode:");
			int mode = WiFi.getMode();
			if(mode == WIFI_OFF){
			   Serial.println("WIFI_OFF");
			   return "WIFI_OFF";
			}else if(mode == WIFI_STA){
			   Serial.println("WIFI_STA");
			   return "WIFI_STA";
			}else if(mode == WIFI_AP){
			   Serial.println("WIFI_AP");
			   return "WIFI_AP";
			}else if(mode == WIFI_AP_STA){
			   Serial.println("WIFI_AP_STA");
			   return "WIFI_AP_STA";
		   }
}


