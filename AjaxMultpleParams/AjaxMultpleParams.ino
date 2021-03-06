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
//	Serial.println("** handleRoot ***");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleAuxPage() {
//	Serial.println("** handleAux ***");
 String s = AUX_page; //Read HTML contents
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
	Serial.println(payload);
	server.send(200, "text/plane", payload); //Send ADC value only to client ajax request
	delete vals;
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
  Serial.println("\n\nAjaxMultpleParams.ino - v2.?");
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