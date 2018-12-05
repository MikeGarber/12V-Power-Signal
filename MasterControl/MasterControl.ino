/*
 Name:		MasterControl.ino
 Created:	8/28/2018 3:30:14 PM
 Author:	TheDukeOfHighwayJ
*/

#include <HandleSerialParams.h>
HandleSerialParams paramHandlr(6);

#include <Wire.h>
#include <Adafruit_MCP23017.h>

Adafruit_MCP23017 mcp;
#define OUT1	3 //PA1
#define OUT2	4 //PA2
#define OUT3	5 //PA3
#define OUT4	6 //PA4
int OUTIndexes[] = {OUT1, OUT2, OUT3, OUT4};
#define INP1	8+6 //PB6
#define INP2	8+4 //PB5	//note INPapped
#define INP3	8+5 //PB4	//note INPapped
#define INP4	8+3 //PB3
int INPIndexes[] = {INP1, INP2, INP3, INP4};

void mcpInit()
{
		mcp.begin();      // use default address 0
 	for (int i = 0; i < sizeof(OUTIndexes)/sizeof(OUTIndexes[0]); i++)
	{
		 mcp.pinMode(OUTIndexes[i], OUTPUT);
	}
 	for (int i = 0; i < sizeof(INPIndexes)/sizeof(INPIndexes[0]); i++)
	{
		 mcp.pinMode(INPIndexes[i], INPUT);
		 mcp.pullUp(INPIndexes[i], HIGH);
	}
}
bool startup = true;
bool runing = false;
void setup() {  
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);

	paramHandlr.AddParam("startup mcp", "s", &startup, HandleSerialParams::eParamType::pBool);
	paramHandlr.DumpVars();
}

// the loop function runs over and over again until power down or reset
void loop() {

	paramHandlr.CheckAndHandleSerial();
	if (startup && !runing)
	{
		mcpInit();
		runing = true;
	}
	if (runing)
	{
		delay(1500);
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	
		Serial.print("Inputs=");
		for (int i = 0; i < sizeof(INPIndexes)/sizeof(INPIndexes[0]); i++)
		{
			Serial.print(mcp.digitalRead(INPIndexes[i]));  Serial.print("  ");
		}

		static int count=0;
		int outPattern[] = {0, 1, 2, 3};	
		for (int i = 0; i < sizeof(OUTIndexes)/sizeof(OUTIndexes[0]); i++)
			mcp.digitalWrite(OUTIndexes[i], 0);
		Serial.printf("OUTPUT %d ON\n", OUTIndexes[outPattern[count]]);
		mcp.digitalWrite(OUTIndexes[outPattern[count]], 1);

		count = (count+1)%4;
		Serial.println("  ");
	}
}
