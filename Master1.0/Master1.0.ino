/*
 Name:		Master1.ino
 Created:	10/15/2018 7:46:11 PM
 Author:	TheDukeOfHighwayJ
*/

// the setup function runs once when you press reset or power the board
#include <HandleSerialParams.h>
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
#include <Adafruit_MCP23017.h>
#include <Wire.h>

ESP8266WebServer server(80); //Server on port 80

#include "IO.h"
#include "OTAsetup.h"
#include "Analog.h"
#include "MainPage.h"
#include "index.h"

#define LED 2  //On board LED

HandleSerialParams paramHandlr(6);

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

void handleAuxPage() {
//	Serial.println("** handleAux ***");
 String s = AUX1_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

/* send voltage strings according to (from the client code):
	{"ADCValuePwr", "ADCValueChdN", "ADCValueChgngP", "ADCValueChgngN", "ADCValueUV", "ADCValueUnused"..
...	and then ...
	 "ChargedLED","ChargingLED","UnderVLED"
*/
void handleADC() {
	String payload;
	int n = sizeof(anaInpSel)/sizeof(anaInpSel[0]);
	float *vals = new float[n];
	for (int i=0; i<n; i++)
	{
		SelectAnaChannel(anaInpSel[i]);
		analogRead(A0);

		vals[i] = (((float)analogRead(A0))/71.5);		//conv to volts
		payload += vals[i];
		payload += " ";
	}
	// "ChargedLED",
	if (vals[getIndexFromEnum(aCharged_PalsoPwr)] - vals[getIndexFromEnum(aCharged_N)] > 2.0)
		payload += "ON"; else payload += "OFF"; 
	payload += " ";
	
	//"ChargingLED",
	if (vals[getIndexFromEnum(aCharging_P)] - vals[getIndexFromEnum(aCharging_N)] > 2.0)
		payload += "ON"; else payload += "OFF"; 
	payload += " ";
	
	//"UnderVLED"
	if (vals[getIndexFromEnum(aUnderV)] >.25)
		payload += "ON"; else payload += "OFF"; 
	payload += " ";
////	Serial.println(payload);
	server.send(200, "text/plane", payload); //Send ADC value only to client ajax request
	delete vals;
}

//==============================================================
//                  SETUP
//==============================================================
void setup() {
  initIO();
  Serial.begin(115200);
  Serial.println("\n\nMaster1.0.ino - v1.1");
  WiFi.begin(ssid, password);     //Connect to your WiFi router
    IPAddress ip(192,168,1,199);   
	IPAddress gateway(192,168,1, 1);   
	IPAddress subnet(255,255,255,0);   
	WiFi.config(ip,gateway,subnet);

  Serial.println("");
  anaPinSetup();
  //Onboard LED port Direction output
  pinMode(LED,OUTPUT); 

	//paramHandlr.AddParam("func char (0 is none)", 'F', &func, HandleSerialParams::eParamType::pChar);
	//paramHandlr.AddParam("slaveID(200-254)", 'S', &slaveID, HandleSerialParams::eParamType::pChar);
	//paramHandlr.DumpVars();

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
  server.on("/gotoAux", handleAuxPage);      //Which routine to handle at root location. This is display page
  server.on("/readADC", handleADC);
  SetIOserverHandlers();

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

	for (int i = 0; i < sizeof(INPIndexes)/sizeof(INPIndexes[0]); i++)
	{
		if (inputForces[i] == 0)
		{
			int newI = !mcp.digitalRead(INPIndexes[i]);	//active low
			if (newI != inputs[i])	
				handleIStateChange(i, newI);
		}
	}
}
