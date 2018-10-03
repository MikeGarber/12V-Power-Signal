

//Analog channels (on the 4051)
enum aChannels {aCharged_N=6, aCharged_P/*also Pwr*/=2, aCharging_N=3, aCharging_P=1, aUnderV=0, aUnused=5};

/*#define aCharged_N	6
#define aCharged_P	2		//also Pwr
#define aCharging_N	3
#define aCharging_P	1
#define aUnderV		0	
#define aUnused		5*/
int anaValues[5];

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
	for (int i=0; i<5; i++)
	{
		digitalWrite(anaPinA, i & 0x01);
		digitalWrite(anaPinB, i & 0x02);
		digitalWrite(anaPinC, i & 0x04);
		anaValues[i]=analogRead(A0);
			//Serial.print(i);
			//Serial.print(" --> ");
			//Serial.print(digitalRead(anaPinC));
			//Serial.print(digitalRead(anaPinB));
			//Serial.print(digitalRead(anaPinA));
			//Serial.print(" -> ");
			//Serial.println(anaValues[i]);
	}
}



// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	anaPinSetup();
}

// the loop function runs over and over again until power down or reset
void loop() {
static unsigned long anaTime =0;
#define anaInterval 1000

	if (millis() > anaTime + anaInterval)
	{
		anaTime = millis();
		Serial.println("-----------");
		readAnaValues();
	}
}
