//https://diyprojects.io/esp8266-web-server-part-1-store-web-interface-spiffs-area-html-css-js/#.W5AqV2gvzIU
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <FS.h>

#define ssid      "GarberPark"      // WiFi SSID
#define password  "Spicey123"      // WiFi password
#define DHTTYPE   DHT22       // DHT type (DHT11, DHT22)
#define DHTPIN    D4          // Broche du DHT / DHT Pin
const uint8_t GPIOPIN[4] = {D5,D6,D7,D8};  // Led
float   t = 0 ;
float   h = 0 ;
float   pa = 0;

// Création des objets / create Objects
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
ESP8266WebServer server ( 80 );

void setup() {
  for ( int x = 0 ; x < 5 ; x++ ) {
    pinMode(GPIOPIN[x], OUTPUT);
  }
  
  Serial.begin ( 115200 );
  // Initialisation du BMP180 / Init BMP180
  if ( !bmp.begin() ) {
    Serial.println("BMP180 KO!");
    while (1);
  } else {
    Serial.println("BMP180 OK");
  }

  WiFi.begin ( ssid, password );
  // Attente de la connexion au réseau WiFi / Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  // Connexion WiFi établie / WiFi connexion is OK
  Serial.println ( "" );
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  if (!SPIFFS.begin())
  {
    // Serious problem
    Serial.println("SPIFFS Mount failed");
  } else {
    Serial.println("SPIFFS Mount succesfull");
  }

  server.serveStatic("/img", SPIFFS, "/img");
  server.serveStatic("/", SPIFFS, "/index.html");

  server.begin();
  Serial.println ( "HTTP server started" );

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  t = dht.readTemperature();
  h = dht.readHumidity();
  pa = bmp.readPressure() / 100.0F;
  delay(100);
}