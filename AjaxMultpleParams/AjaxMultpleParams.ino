#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "OTAsetup.h"

#include "index.h" //Our HTML webpage contents with javascripts
#include "Analog.h"

#define LED 2  //On board LED

//SSID and Password of your WiFi router
const char* ssid = "GarberPark";		//case sensative
const char* password = "Spicey123";

ESP8266WebServer server(80); //Server on port 80

void handleRoot() {
	Serial.println("** handleRoot ***");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleAuxPage() {
	Serial.println("** handleAux ***");
 String s = AUX_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleADC() {
	readAnaValues();
	String adcValue = String(anaValues[0]);
	for (int i = 1; i < ANA_SIZE; i++){
		adcValue += " ";
		adcValue += anaValues[i];
	}
	server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
  Serial.println("\n\nAjaxMultpleParams.ino - v1.?");
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  anaPinSetup();
  //Onboard LED port Direction output
  pinMode(LED,OUTPUT); 

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
}