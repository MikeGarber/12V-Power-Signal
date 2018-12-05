#pragma once

///Analog channels (on the 4051)
enum aChannels {aCC_Batt=0, aCC_Panl=3};
const aChannels anaInpSel[]= {aCC_Batt, aCC_Panl};
int             anaValues[/*SAME AS ABOVE, don' wanna dynam alloc*/2];

int getIndexFromEnum(aChannels chan) {
	int n =sizeof(anaInpSel) / sizeof(anaInpSel[0]);
	for (int i = 0; i < n; i++)
	{
		if (anaInpSel[i]==chan)
			return i;
	}
	return 99;
}

#define anaPinA 0
#define anaPinB 2
#define anaPinC 14
void anaPinSetup()
{
	pinMode(anaPinA, OUTPUT);
	pinMode(anaPinB, OUTPUT);
	pinMode(anaPinC, OUTPUT);
}

void SelectAnaChannel(int i)
{
		digitalWrite(anaPinA, i & 0x01);
		digitalWrite(anaPinB, i & 0x02);
		digitalWrite(anaPinC, i & 0x04);
		delay(1);
}

int readAnaValues()
{
	int n = sizeof(anaValues)/sizeof(anaValues[0]);
	for (int i=0; i<n; i++)
	{
		SelectAnaChannel(i);
		analogRead(A0);
		anaValues[i]=analogRead(A0);
	}
}

void handleADC() {			//as shown on the mainpage.h
	String payload;
	int n = sizeof(anaInpSel)/sizeof(anaInpSel[0]);
	//Serial.printf("hey bud... there are %d values comming...\n", n);
	float *vals = new float[n];
	for (int i=0; i<n; i++)
	{
		SelectAnaChannel(anaInpSel[i]);
		analogRead(A0);

		vals[i] = (((float)analogRead(A0))/71.5);		//conv to volts
		payload += vals[i];
		payload += " ";
	}
	// "ChargedLED",
////	if (vals[getIndexFromEnum(aCharged_PalsoPwr)] - vals[getIndexFromEnum(aCharged_N)] > 2.0)
////		payload += "ON"; else payload += "OFF"; 
////	payload += " ";
////	
////	//"ChargingLED",
////	if (vals[getIndexFromEnum(aCharging_P)] - vals[getIndexFromEnum(aCharging_N)] > 2.0)
////		payload += "ON"; else payload += "OFF"; 
////	payload += " ";
////	
////	//"UnderVLED"
////	if (vals[getIndexFromEnum(aUnderV)] >.25)
////		payload += "ON"; else payload += "OFF"; 
////	payload += " ";
////////	Serial.println(payload);
	server.send(200, "text/plane", payload); //Send ADC value only to client ajax request
	delete vals;
}
