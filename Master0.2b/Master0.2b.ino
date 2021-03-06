#include <SoftwareSerial.h>

char func = '1';
char  slaveID='a';

#include "MasterSlaveComms.h"
#include <HandleSerialParams.h>
HandleSerialParams paramHandlr(6);

bool tstModeListen = false;
long loopDelay = 5;
void setup() {
	Serial.begin(HARDWAREBAUD);
	delay(10);
	Serial.println("\nMaster0.2b");

	setupMasterSlaveComms(true); //master

	pinMode(DBUG_PIN, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	
	
	paramHandlr.AddParam("tstModeListen", 'M', &tstModeListen, HandleSerialParams::eParamType::pBool);
	paramHandlr.AddParam("delay", 'D', &loopDelay, HandleSerialParams::eParamType::pLong);
	paramHandlr.AddParam("func char (0 is none)", 'F', &func, HandleSerialParams::eParamType::pChar);
	paramHandlr.AddParam("slaveID(200-254)", 'S', &slaveID, HandleSerialParams::eParamType::pChar);
	paramHandlr.DumpVars();
}

#define WAITFORRESPONSE 30 //ms
#define REPEATTIME 500 //ms
unsigned long mSec = millis();

void loop() {
	if (tstModeListen)
		masterTPinMode(eListen);
	else 
		masterTPinMode(ePower);
	if (!tstModeListen && (mSec+loopDelay <= millis()))
	{
		mSec = millis();
		paramHandlr.CheckAndHandleSerial();
		enumPacktConditition res = HandleSSerial();
		if (res == eForMeGood)		//reply!
		{
			cmdSentWaitingForReply = false;
			masterTPinMode(ePower);		//back to being a power source
///			Serial.print(" Reply!\n");
		}
		else if (res != eNone)
		{
			Serial.print(" ??\n");
		}

		if (cmdSentWaitingForReply && (millis() >= TxTimeoutStarttime + WAITFORRESPONSE))
		{
///			Serial.print(" ??TIMEOUT\n");
			masterTPinMode(ePower);		//back to being a power source
			cmdSentWaitingForReply=false;
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
}
