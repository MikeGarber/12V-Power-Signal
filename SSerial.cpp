#include <SoftwareSerial.h>
SoftwareSerial mySerial(7,6); // RX,…
#define HARDWAREBAUD 115200
#define SOFTWAREBAUD 2400

const byte numSChars = 68;
char receivedSChars[numSChars];   // array to store the data from the Software Serial
void HandleSSerial()	//handle the time msg from the 8266
{
	bool dst = false;
	static byte ndx = 0;
	bool stringAvail = false;
	while (mySerial.available() > 0) {
		char rc = mySerial.read();
		if ((rc != '\n') && (rc != '\r')) {
			receivedSChars[ndx] = rc;
			ndx++;
			if (ndx >= numSChars)
				ndx = numSChars - 1;
		}
		else {
			receivedSChars[ndx] = '\0'; // terminate the string
			ndx = 0;
			stringAvail = true;
			break;
		}
	}
	if (!stringAvail)
		return;
	DecodeSSerial();
}

void DecodeSSerial()		//such as 11:05:15 AM CDT 6/23/2017 5:15 20:35
{
	Serial.println(receivedSChars);
	if (strlen(receivedSChars) > 10) {	//looks like the println in the 8266 does both a cr & lf
		;
	}
}
