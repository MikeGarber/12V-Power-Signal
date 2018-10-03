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
int anaValues[ANA_SIZE]={0, 0, 0, 0, 0, 0, 0};

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
	int t1 = int(millis());
Serial.print(t1);  Serial.print("   "); Serial.print(t1 & 0x1f); Serial.print(" -- ");
	for (int i=0; i<ANA_SIZE; i++)
	{
		digitalWrite(anaPinA, i & 0x01);
		digitalWrite(anaPinB, i & 0x02);
		digitalWrite(anaPinC, i & 0x04);
		delay(1);
		anaValues[i]=(i*10)+ (t1 & 0xff);//analogRead(A0);
		Serial.print("  ");  Serial.print(anaValues[i]);
			//Serial.print(i);
			//Serial.print(" --> ");
			//Serial.print(digitalRead(anaPinC));
			//Serial.print(digitalRead(anaPinB));
			//Serial.print(digitalRead(anaPinA));
			//Serial.print(" -> ");
			//Serial.println(anaValues[i]);
	}
}

