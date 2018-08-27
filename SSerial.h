#pragma once
//#include <SoftwareSerial.h>		//mySerial already set up

#define HARDWAREBAUD 115200
#define SOFTWAREBAUD 2400
void setupSerial()
{
	mySerial.begin(SOFTWAREBAUD);		// set the data rate for the SoftwareSerial port
	Serial.begin(HARDWAREBAUD);
}

const byte numSChars = 24;
char receivedSChars[numSChars];   // array to store the data from the Software Serial
bool HandleSSerial()	
{
	bool dst = false;
	static byte ndx = 0;
	bool stringAvail = false;
	while (mySerial.available() > 0) {
		char rc = mySerial.read();
		if ((rc != '\n') && (rc != '\r')) {
			receivedSChars[ndx] = rc;
			Serial.print(rc);
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
	return stringAvail;
}
