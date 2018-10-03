#pragma once
//Analog channels (on the 4051)
enum aChannels {aCharged_N=6, aCharged_P/*also Pwr*/=2, aCharging_N=3, aCharging_P=1, aUnderV=0, aUnused=4};

/*#define aCharged_N	6
#define aCharged_P	2		//also Pwr
#define aCharging_N	3
#define aCharging_P	1
#define aUnderV		0	
#define aUnused		4*/

#define ANA_SIZE 7
int anaValues[ANA_SIZE]={0,0,0,0,0,0,0};

#define anaPinA 0
#define anaPinB 2
#define anaPinC 14
void anaPinSetup()
{
	pinMode(anaPinA, OUTPUT);
	pinMode(anaPinB, OUTPUT);
	pinMode(anaPinC, OUTPUT);
}

int readAnaValues()
{
	for (int i=0; i<ANA_SIZE; i++)
	{
	Serial.println();
	for (int i=0; i<ANA_SIZE; i++)
	{
		Serial.print("#");
		Serial.print(anaValues[i]);

	}
	Serial.println();
		//digitalWrite(anaPinA, i & 0x01);
		//digitalWrite(anaPinB, i & 0x02);
		//digitalWrite(anaPinC, i & 0x04);
		delay(1);
		anaValues[i]=i+(int)millis() & 0x1f;//analogRead(A0);
//		Serial.print("; "+anaValues[i]);
			//Serial.print(i);
			//Serial.print(" --> ");
			//Serial.print(digitalRead(anaPinC));
			//Serial.print(digitalRead(anaPinB));
			//Serial.print(digitalRead(anaPinA));
			//Serial.print(" -> ");
			//Serial.println(anaValues[i]);
	}
}


void handleADC() {
	readAnaValues();
	Serial.print("\n\n");
	String adcValue = String(anaValues[0]);
	for (int i = 1; i < ANA_SIZE; i++){
		Serial.print(". "); Serial.print(anaValues[i]);
		adcValue += " ";
		adcValue += String(anaValues[i]);
		Serial.print(adcValue);
	}
	Serial.println(adcValue);
	//server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
  Serial.println("\n\nAjaxMultpleParams.ino - debugtest");

  handleADC();

}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
}