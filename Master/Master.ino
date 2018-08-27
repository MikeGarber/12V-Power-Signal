/*
 Name:		Master.ino
 Created:	7/25/2018 9:06:57 AM
 Author:	TheDukeOfHighwayJ
*/

#include "../8266SoftwareSerial.h"
#define RPIN 0
#define TPIN 2
#define DBUG_PIN 4

SoftwareSerial mySerial(RPIN, TPIN, true, false);
#include "SSerial.h"

void setup() {
	pinMode(DBUG_PIN, OUTPUT);
	Serial.begin(HARDWAREBAUD);
	mySerial.begin(SOFTWAREBAUD);		// set the data rate for the SoftwareSerial port

}

void TPinMode(bool state) //TRUE
{
	if (state){
		pinMode(TPIN, OUTPUT);
		digitalWrite(TPIN, HIGH);
	}
	else
		pinMode(TPIN, INPUT);


}
unsigned long lastTime=millis();
// the loop function runs over and over again until power down or reset
void loop() {
	if (HandleSSerial())
	{		//  ---- DecodeSSerial() -----
		Serial.println(receivedSChars);
		if (strlen(receivedSChars) > 10)	//looks like the println in the 8266 does both a cr & lf
		{
			;
		}
		TPinMode(true);
	}

	if (millis() >= lastTime + 50)
	{
			TPinMode(true);
	}
	if (millis() >= lastTime + 100)
	{
		lastTime=millis();
		digitalWrite(DBUG_PIN, HIGH);
		char buf[]={"hi mom\n"};
		mySerial.stopListening();
		mySerial.write(buf, 7);
		digitalWrite(DBUG_PIN, LOW);
		TPinMode(true);	//allow for hearing reply
	}
}
