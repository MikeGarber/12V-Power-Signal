#define TPIN D8	//2 is LED_BUILTIN
#include <HandleSerialParams.h>

#define DBUG_PIN 4

enum BussState {ePower, eListen} state;  //master
void masterTPinMode(BussState s) //TRUE
{
	state = s;
	if (state==ePower){
		pinMode(TPIN, OUTPUT);
		digitalWrite(TPIN, HIGH);
	}
	else
		pinMode(TPIN, INPUT);
}


HandleSerialParams paramHandlr(6);


void setup() {
	Serial.begin(115200);
	delay(10);
	Serial.println("\ntest");

	pinMode(TPIN, OUTPUT);
	pinMode(DBUG_PIN, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	
	paramHandlr.AddParam("func char (0 is none)", 'F', &func, HandleSerialParams::eParamType::pChar);
	paramHandlr.AddParam("slaveID(200-254)", 'S', &slaveID, HandleSerialParams::eParamType::pChar);
	paramHandlr.DumpVars();
}


unsigned long lasttime = millis();

void loop() {
	delay(1);
	paramHandlr.CheckAndHandleSerial();


	{
		delay(5);
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
			delay(1);
			masterTPinMode(eListen);		//back to being a power source
		}
	}
}
