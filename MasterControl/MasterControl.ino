/*
 Name:		MasterControl.ino
 Created:	8/28/2018 3:30:14 PM
 Author:	TheDukeOfHighwayJ
*/

#include <Wire.h>
#include <Adafruit_MCP23017.h>

Adafruit_MCP23017 mcp;
#define RY1	1 //PA1
#define RY2	2 //PA2
#define RY3	3 //PA3
#define RY4	4 //PA4
int RyIndexes[] = {RY1, RY2, RY3, RY4};
#define SW1	8+6 //PB6
#define SW2	8+5 //PB5
#define SW3	8+4 //PB4
#define SW4	8+3 //PB3
int SwIndexes[] = {SW1, SW2, SW3, SW4};

void setup() {  
	Serial.begin(115200);
	mcp.begin();      // use default address 0
	pinMode(LED_BUILTIN, OUTPUT);
 	for (int i = 0; i < 4; i++)
	{
		 mcp.pinMode(RyIndexes[i], OUTPUT);
	}
 	for (int i = 0; i < 4; i++)
	{
		 mcp.pinMode(SwIndexes[i], INPUT);
		 mcp.pullUp(SwIndexes[i], HIGH);
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
	delay(500);
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	static int count=0;
	int outPattern[] = {0, 1, 2, 4,};
	for (int i = 0; i < 4; i++)
	{
		Serial.print(mcp.digitalRead(SwIndexes[i]));  Serial.print("  ");
		mcp.digitalWrite(RyIndexes[i], outPattern[count & 0x0f]);
	}
	count++;
	Serial.println("  ");
  
}
