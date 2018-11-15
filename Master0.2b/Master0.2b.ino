#include <SoftwareSerial.h>

char func = '1';
char  slaveID='b';

#include "MasterSlaveComms.h"
#include <HandleSerialParams.h>
HandleSerialParams paramHandlr(6);


void setup() {
	Serial.begin(HARDWAREBAUD);
	delay(10);
	Serial.println("\nMaster0.2b");

	setupMasterSlaveComms(true); //master

	pinMode(DBUG_PIN, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	
	paramHandlr.AddParam("func char (0 is none)", 'F', &func, HandleSerialParams::eParamType::pChar);
	paramHandlr.AddParam("slaveID(200-254)", 'S', &slaveID, HandleSerialParams::eParamType::pChar);
	paramHandlr.DumpVars();
}


#define WAITFORRESPONSE 20 //ms
#define REPEATTIME 50 //ms
void loop() {
	delay(1);
	paramHandlr.CheckAndHandleSerial();
	enumPacktConditition res = HandleSSerial();
	if (res == eForMeGood)		//reply!
	{
		masterTPinMode(ePower);		//back to being a power source
	}

	if (millis() >= TxTimeoutStarttime + WAITFORRESPONSE)
	{
		masterTPinMode(ePower);		//back to being a power source
	}
	if (millis() >= TxTimeoutStarttime + REPEATTIME)
	{
		if (func != '0')
		{
			if (func & 0x01)
			{
				unsigned char buf[10]={"HiMom"};
				SendCmd(slaveID, func, (char*)buf, 5);
			}
			else
				SendCmd(slaveID, func, NULL, 0);

			digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
			digitalWrite(DBUG_PIN, !digitalRead(DBUG_PIN));
			masterTPinMode(eListen);		//back to being a power source
		}
	}
}
