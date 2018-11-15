void setup() {
	Serial.begin(115200);
	delay(10);
	Serial.println("\nMaster0.2msgFormatting\n");

}


#define NUMHCHARS 202
char receivedHChars[NUMHCHARS];   // array to store the data from the Hardware Serial

void addAsciiChar(char c, short *chksm, String* msg)
{	
	if (chksm)
		(*chksm)+=c;
	(*msg)+= String(c);
	Serial.println(*msg);
}

void loop() {
	delay(1);

	bool stat = false;
	static byte hdx = 0;
	static byte receivedHCharsSize = 0;
	
	while (Serial.available() > 0) {
		char rc = Serial.read();
		if ((rc != '\n') && (rc != '\r')) {
//			Serial.print("*");//
//			Serial.print(rc, HEX); Serial.print(",");
			receivedHChars[hdx] = rc;
			hdx++;
			if (hdx >= NUMHCHARS)
				hdx = NUMHCHARS - 1;
		}
		else {
			receivedHChars[hdx] = '\0'; // terminate the string
			receivedHCharsSize=hdx+1;
			hdx = 0;
			stat = true;
		}
	}
	if (stat)
	{
		short chksm=0;
		String msg;
		int i;
		Serial.print("/************* ENCODE *************/");
		for (i = 0; i < receivedHCharsSize; i++)
		{	
			addAsciiChar(receivedHChars[i], &chksm, &msg);
		}
		chksm %=100;
		addAsciiChar(0x30+(chksm / 10), NULL, &msg);
		addAsciiChar(0x30+(chksm % 10), NULL, &msg);		

		Serial.print("/************* DECODE *************/\n");
		const char * chars = msg.c_str();
		int calcedChksm=0;
		for (int i=0; i<msg.length()-2; i++)
		{
			calcedChksm += chars[i];
		}
		Serial.printf("last 2 chars (i=%d) are %d %d \n", i, chars[i], chars[i+1]);
		int receivedChksm = String(chars[i-1]).toInt()*10 + String(chars[i]).toInt();
		Serial.printf("apparant DataLen=%d, checksum is %s \n", msg.length()-(3/*s/d/f*/+2/*cksm*/), calcedChksm%100==receivedChksm? "OK" : "NOTOK");
		Serial.printf("sent chksm=%d .... rcvd chksm=%d .... calc'd chksm=%d\n", chksm, receivedChksm, calcedChksm);
	}
}
