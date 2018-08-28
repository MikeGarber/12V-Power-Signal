#include <SoftwareSerial.h>
#define RPIN 0
#define TPIN 5	//2 is LED_BUILTIN
#define DBUG_PIN 4

/*typedef */enum BussState {ePower, eListen} state;

void TPinMode() //TRUE
{
	if (state==ePower){
		pinMode(TPIN, OUTPUT);
		digitalWrite(TPIN, HIGH);
	}
	else
		pinMode(TPIN, INPUT);
}
SoftwareSerial  * mySerial;
const char myID='~';
char func = '1';
char  slaveID='b';

#include "pSSerial.h"
#include <HandleSerialParams.h>
HandleSerialParams paramHandlr(6);


void setup() {
	Serial.begin(HARDWAREBAUD);
	delay(10);
	Serial.println("\nMaster0.2b");

	mySerial = new SoftwareSerial(RPIN, TPIN, true, false, true);
	mySerial->begin(SOFTWAREBAUD);		// set the data rate for the SoftwareSerial port

	pinMode(DBUG_PIN, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	
	paramHandlr.AddParam("func char (0 is none)", 'F', &func, HandleSerialParams::eParamType::pChar);
	paramHandlr.AddParam("slaveID(200-254)", 'S', &slaveID, HandleSerialParams::eParamType::pChar);
	paramHandlr.DumpVars();
}


#define WAITFORRESPONSE 20 //ms
#define REPEATTIME 1000 //ms
void loop() {
	delay(1);
	paramHandlr.CheckAndHandleSerial();
	enumPacktConditition res = HandleSSerial();
	if (res == eForMeGood)		//reply!
	{
		delay(5);
		state = ePower; TPinMode();		//back to being a power source
	}

	if (millis() >= TxTimeoutStarttime + WAITFORRESPONSE)
	{
		state = ePower; TPinMode();		//back to being a power source
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
			delay(1);
			state = eListen; TPinMode();		//back to being a power source
		}
	}
}
