#include <SoftwareSerial.h>
#include "MasterSlaveComms.h"

#define DBUG_PIN 4

#include <HandleSerialParams.h>
HandleSerialParams paramHandlr(6);


void setup() {
	Serial.begin(HARDWAREBAUD);
	setupMasterSlaveComms(false); //slave

	pinMode(DBUG_PIN, OUTPUT);
	digitalWrite(DBUG_PIN, HIGH);
	pinMode(LED_BUILTIN, OUTPUT);
	
	Serial.print("\nSlave0, TPIN is "); Serial.println(TPIN);
	paramHandlr.AddParam("myID('a'thru'z')", 'I', &myID, HandleSerialParams::eParamType::pChar);
	paramHandlr.AddParam("delta", 'd', &delta, HandleSerialParams::eParamType::pLong);
	paramHandlr.DumpVars();
}

unsigned long lastTime=millis();

void loop() {
	paramHandlr.CheckAndHandleSerial();
	enumPacktConditition res = HandleSSerial();
	if (res == eNotForMe)
	{
		Serial.print("eNotForMe, receivedSChars[0]="); Serial.println(receivedSChars[0]); 
	}
	if (res == eBadChecksum)
	{
		Serial.println("eBadChecksum"); 
	}
	if (res == eForMeGood)
	{
		//Serial.println(" GOOD FOR ME"); //Serial.println(receivedSChars); 
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
		digitalWrite(DBUG_PIN, !digitalRead(DBUG_PIN));
	}
		
	if (millis() >= lastTime + 200)
	{
		lastTime=millis();
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	}
}
