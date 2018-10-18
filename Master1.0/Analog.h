#pragma once

///Analog channels (on the 4051)
/*#define aCharged_N	6
#define aCharged_P	2		//also Pwr
#define aCharging_N	3
#define aCharging_P	1
#define aUnderV		0	
#define aUnused		4*/
enum aChannels {aCharged_PalsoPwr=6, aCharged_N=2, aCharging_N=3, aCharging_P=1, aUnderV=0, aUnused=4};

/* get here to send voltage strings according to (from the client code):
	{"ADCValuePwr", "ADCValueChdN", "ADCValueChgngP", "ADCValueChgngN", "ADCValueUV", "ADCValueUnused"};	*/
const aChannels anaInpSel[]= {aCharged_PalsoPwr, aCharged_N, aCharging_P, aCharging_N, aUnderV, aUnused};

int getIndexFromEnum(aChannels chan) {
	int n =sizeof(anaInpSel) / sizeof(anaInpSel[0]);
	for (int i = 0; i < n; i++)
	{
		if (anaInpSel[i]==chan)
			return i;
	}
	return 99;
}

#define ANA_SIZE 7
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
//int readAnaValues()
//{
//	for (int i=0; i<ANA_SIZE; i++)
//	{
//		SelectAnaChannel(i);
//		analogRead(A0);
//		anaValues[i]=analogRead(A0);
//	}
//}

void handleADC() {
	String payload;
	int n = sizeof(anaInpSel)/sizeof(anaInpSel[0]);
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
	if (vals[getIndexFromEnum(aCharged_PalsoPwr)] - vals[getIndexFromEnum(aCharged_N)] > 2.0)
		payload += "ON"; else payload += "OFF"; 
	payload += " ";
	
	//"ChargingLED",
	if (vals[getIndexFromEnum(aCharging_P)] - vals[getIndexFromEnum(aCharging_N)] > 2.0)
		payload += "ON"; else payload += "OFF"; 
	payload += " ";
	
	//"UnderVLED"
	if (vals[getIndexFromEnum(aUnderV)] >.25)
		payload += "ON"; else payload += "OFF"; 
	payload += " ";
////	Serial.println(payload);
	server.send(200, "text/plane", payload); //Send ADC value only to client ajax request
	delete vals;
}
