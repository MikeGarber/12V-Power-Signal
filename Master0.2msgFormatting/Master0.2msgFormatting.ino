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
	{	short chksm=0;
		String msg;
		int i;
		Serial.print("/************* ENCODE *************/");
		for (i = 0; i < receivedHCharsSize; i++)
		{	
			switch (i) {
				case 3: // time to figure out data length
					{int len=receivedHCharsSize-4; //dst/src/fnc/enddelim
					Serial.printf("\nlen=%d (%c %c)", len, 0x30+(len / 10), 0x30+(len % 10));
					addAsciiChar(0x30+(len / 10), &chksm, &msg);		//msb
					addAsciiChar(0x30+(len % 10), &chksm, &msg);		//lsb
					Serial.printf("\n"); /*Yes, fall thru*/}
				default:
//					Serial.printf("i=%d   ", i);
					addAsciiChar(receivedHChars[i], &chksm, &msg);
			}
		}
		chksm %=100;
		addAsciiChar(0x30+(chksm / 10), NULL, &msg);
		addAsciiChar(0x30+(chksm % 10), NULL, &msg);		

		Serial.print("/************* DECODE *************/\n");
		const char * chars = msg.c_str();
		int incommingDataLen=0;
		int calcedChksm=0;
		int dataLen =0;

		int sentDataLen = String(chars[3]).toInt()*10 + String(chars[4]).toInt();
		Serial.printf("sentDataLen=%d ... expected len=%d\n", sentDataLen, msg.length()-(3/*s/d/f*/+2/*len*/+2/*cksm*/));
		for (i = 0; i < msg.length()-2; i++)
		{
			calcedChksm += chars[i];
		}
		int receivedChksm = String(chars[i]).toInt()*10 + String(chars[i+1]).toInt();
		Serial.printf("sent chksm=%d .... rcvd chksm=%d .... calc'd chksm=%d\n", chksm, receivedChksm, calcedChksm);
	}
}
