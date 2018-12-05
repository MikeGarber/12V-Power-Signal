/*
 Name:		Master1.ino
 Created:	10/15/2018 7:46:11 PM
 Author:	TheDukeOfHighwayJ
*/


#include <ESP8266WebServerSecure.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>

ESP8266WebServer server(80); //Server on port 80

#include "IO.h"
#include "OTAsetup.h"
#include "DebugPage.h"
#include "Analog.h"
#include "MainPage.h"

//SSID and Password of your WiFi router
const char* ssid = "GarberPark";		//case sensative
const char* password = "Spicey123";

void handleRoot() {
//	Serial.println("** handleRoot ***");
 String s = MAINpage; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleIOPage() {
//	Serial.println("** handleIO ***");
 String s = IOpage; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleDebugPage() {
//	Serial.println("** handleAux ***");
 String s = DEBUG_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

#include <HandleSerialParams.h>
HandleSerialParams paramHandlr(6, true);
int var1;	//gotta have something for paramHandlr
//==============================================================
//                  SETUP
//==============================================================
void setup() {
  initIO();
  Serial.begin(115200);
  Serial.println("\n\n*** Master1.1.ino - 12/5/18 ***\n");
  anaPinSetup();
  
  pinMode(LED_BUILTIN, OUTPUT); 

	//paramHandlr.AddParam("func char (0 is none)", 'F', &func, HandleSerialParams::eParamType::pChar);
	paramHandlr.AddParam("var1", "V1", &var1, HandleSerialParams::eParamType::pInt);
	paramHandlr.DumpVars();

  WiFi.begin(ssid, password);     //Connect to your WiFi router
    IPAddress ip(192,168,1,199);   
	IPAddress gateway(192,168,1, 1);   
	IPAddress subnet(255,255,255,0);   
	WiFi.config(ip,gateway,subnet);

// Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/gotoHome", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/gotoDebug", handleDebugPage);      //Which routine to handle at root location. This is display page
  server.on("/readADC", handleADC);
  server.on("/sendCommand", handleDebugCommand);
  SetupIOserverHandlers();

  server.begin();                  //Start server
  OTAsetup();
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
	ArduinoOTA.handle();
	server.handleClient();          //Handle client requests
	paramHandlr.CheckAndHandleSerial();
	loopIO();
}
