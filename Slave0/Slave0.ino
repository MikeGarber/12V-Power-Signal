#include <SoftwareSerial.h>
#define RPIN 10
#define TPIN 9	//2 is LED_BUILTIN
#define DBUG_PIN 4

SoftwareSerial  * mySerial;

char myID='b';
int int2=0;

#include "pSSerial.h"
#include <HandleSerialParams.h>
HandleSerialParams paramHandlr(6);


void setup() {
	Serial.begin(HARDWAREBAUD);
	delay(10);
	Serial.println("\nSlave0");

	mySerial = new SoftwareSerial(RPIN, TPIN, true/*inverseRx*/, true/*inverseTx*/, true/*pullup*/);
	mySerial->begin(SOFTWAREBAUD);		// set the data rate for the SoftwareSerial port

	pinMode(DBUG_PIN, OUTPUT);
	digitalWrite(DBUG_PIN, HIGH);
	pinMode(LED_BUILTIN, OUTPUT);
	
	paramHandlr.AddParam("myID('a'thru'z')", 'I', &myID, HandleSerialParams::eParamType::pChar);
	paramHandlr.DumpVars();
}

unsigned long lastTime=millis();

void loop() {
	delay(1);
	paramHandlr.CheckAndHandleSerial();
	enumPacktConditition res = HandleSSerial();
	if (res == eForMeGood)
	{
		delay(5);		//let master settle, goto tristate
//		Serial.print("receivedSChars="); Serial.println(receivedSChars); 
		digitalWrite(DBUG_PIN, LOW);
		switch (receivedSChars[2])		//func
		{
			case '1':
				sendReply(ACK, NULL, 0);
				break;
			case '2':
				sendReply(ACK, "Hello", 5);
				break;
			default:
				char data[] = NAKNOFUNCT;
				sendReply(NAK, NAKNOFUNCT, strlen(data));
		}
		digitalWrite(DBUG_PIN, HIGH);
	}
		
	if (millis() >= lastTime + 100)
	{
		lastTime=millis();
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	}
}
