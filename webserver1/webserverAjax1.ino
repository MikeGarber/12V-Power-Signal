/*
 * ESP8266 NodeMCU AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "index.h" //Our HTML webpage contents with javascripts

#define LED 2  //On board LED

//SSID and Password of your WiFi router
const char* ssid = "GarberPark";		//case sensative
const char* password = "Spicey123";

ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
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

#define anaPinA 0
#define anaPinB 2
#define anaPinC 14
void anaPinSetup()
{
	pinMode(anaPinA, OUTPUT);
	pinMode(anaPinB, OUTPUT);
	pinMode(anaPinC, OUTPUT);
	digitalWrite(anaPinA, 0);
	digitalWrite(anaPinB, 0);
	digitalWrite(anaPinC, 0);
}
void handleADC() {
 int a = analogRead(A0);
 String adcValue = String(a);
 //Serial.print("val = "); Serial.println(adcValue);
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

void handleLED() {
 String ledState = "OFF";
 String t_state = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
 Serial.println(t_state);
 if(t_state == "1")
 {
  digitalWrite(LED,LOW); //LED ON
  ledState = "ON"; //Feedback parameter
 }
 else
 {
  digitalWrite(LED,HIGH); //LED OFF
  ledState = "OFF"; //Feedback parameter  
 }
 
 server.send(200, "text/plane", ledState); //Send web page
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
  
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
  server.on("/setLED", handleLED);
  server.on("/readADC", handleADC);

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}