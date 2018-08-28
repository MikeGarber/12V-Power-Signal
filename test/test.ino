/*
 Name:		test.ino
 Created:	7/24/2018 4:12:47 PM
 Author:	TheDukeOfHighwayJ

******** Toggling a line with optional tristste time at the edges.
*/

#include <HandleSerialParams.h>
HandleSerialParams paramHandlr(6);

#define pin 10
int tsateRising=0;
int tsateFalling=0;
char r;

void setup() {
	Serial.begin(115200);

	byte inp=78;
	char ms = inp/10+0x30;
	char ls = inp %10+0x30;
	Serial.print("ms-"); ; Serial.println(ms);
	Serial.print("ls-"); Serial.println(ls);

	char * receivedSChars = {"34"};
	int ndx=1;
	byte currDataSize = (receivedSChars[ndx-1]-0x30)*10 + (receivedSChars[ndx]-0x30);
	Serial.print("currDataSize-"); Serial.println(currDataSize);

	//pinMode(pin, OUTPUT);
	paramHandlr.AddParam("Test char r ", 'r', &r, HandleSerialParams::eParamType::pChar);
	//paramHandlr.AddParam("FallingEdge Tristate(0 ...)", 'f', &tsateFalling, HandleSerialParams::eParamType::pInt); 
	paramHandlr.DumpVars();


	//		pinMode(pin, INPUT);
}

// the loop function runs over and over again until power down or reset
bool state = false;
void loop() {

	delay(2);
	paramHandlr.CheckAndHandleSerial();

	//if ( (tsateRising>0  && (state==0)) ||
	//     (tsateFalling>0 && (state!=0)) )
	//static bool tst1=true;
	//{
	//	int t = tsateRising;
	//	if (state==0)
	//		t=tsateFalling;
	//	//for (int i=0; i<t ; i++ )
	//	//{
	//	//	Serial.print("x");
	//	//}
	//	for (int i=0; i<t; i++ )
	//	{
	//		pinMode(pin, INPUT);
	//	}
	//	pinMode(pin, OUTPUT);
	//}

	//////pinMode(pin, INPUT);
	//////pinMode(pin, INPUT);
	//////pinMode(pin, INPUT);
	//////pinMode(pin, INPUT);
	////if (!state)
	////{
	////	pinMode(pin, INPUT);
	////	pinMode(pin, OUTPUT);
	////}
	//digitalWrite(pin, state);
	//state = !state;
}
