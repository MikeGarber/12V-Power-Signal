#pragma once
#if defined(__AVR__)
// AVR specific code here
#define RPIN 3
#define TPIN 2	//2 is LED_BUILTIN
#elif defined(ESP8266)
#define RPIN D7
#define TPIN D8	//2 is LED_BUILTIN
#endif

#define DBUG_PIN 4

enum BussState {ePower, eListen} state;  //master
void masterTPinMode(BussState s) //TRUE
{
	state = s;
	if (state==ePower){
		pinMode(TPIN, OUTPUT);
		digitalWrite(TPIN, HIGH);
	}
	else
		pinMode(TPIN, INPUT);
}


#define HARDWAREBAUD  115200
#define SOFTWAREBAUD 2400

SoftwareSerial  * mySerial;
void setupSerial()
{
	Serial.begin(HARDWAREBAUD);
}

char myID='~';
void setupMasterSlaveComms(bool master)
{
	if (master)
	{
		myID='~';
		mySerial = new SoftwareSerial(RPIN, TPIN, true, false, true);
	}
	else
	{
		myID='a';
		mySerial = new SoftwareSerial(RPIN, TPIN, true/*inverseRx*/, true/*inverseTx*/, true/*pullup*/);
	}
	mySerial->begin(SOFTWAREBAUD);		// set the data rate for the SoftwareSerial port

}
/***************************** RECEVING ************************************/
#define NUMSCHARS 24
char receivedSChars[NUMSCHARS];   // array to store the data from the Software Serial
#define TERMINATOR_CHAR (char)0xff
enum enumPacktConditition
{
	eNone,				//no packet ready
	eNotForMe,			//but not for me ;
	eForMeGood,			//for me and good!
//	eForMeGoodHandled,	//for me and good, and handled!
	eBadChecksum,		// bad integrity!
};

/*** for commands *****/
#define ACK_Q '?'
#define ON   '1'
#define OFF  '0'

enumPacktConditition verifyPacket(char* receivedSChars, int len)
{
	short calcedChksm=0;
	int i=0; 
	for (;i<len-2; i++)		//len includes checksum
	{
		calcedChksm += receivedSChars[i];
	}
	if (calcedChksm%100 != String(receivedSChars[i]).toInt()*10 + String(receivedSChars[i+1]).toInt())
	{	Serial.print("  -i,len, calcedChksm=");Serial.print(i);Serial.print(",");Serial.print(len);Serial.print(",");Serial.println(calcedChksm);
		return eBadChecksum;
	}
	if (receivedSChars[0] == myID)
		return eForMeGood;
	return eNotForMe;
}

unsigned long TxTimeoutStarttime=0;
unsigned long RxTimeoutStarttime=0;
#define RXTIMEOUTDELTA (unsigned long)30
enumPacktConditition HandleSSerial()	
{
	static byte ndx = 0;
	bool firstTime = true;
	bool started = false;
	while (mySerial->available() > 0) {
		started = true;
		unsigned long now = millis();
		if (firstTime) 
		{firstTime=false; 
///		Serial.print("**IN(");Serial.print(now);Serial.print(") ");
		}
		char rc = mySerial->read();
		if (rc==0)
			{Serial.print("null, ndx="); Serial.println(ndx);
			continue;}
///	if (rc<0x20 || rc> 0x7f) {Serial.print("[");Serial.print(rc, HEX);Serial.print("]"); }
///		else Serial.print(rc);
		if ((RxTimeoutStarttime + RXTIMEOUTDELTA < now) && ndx)
		{
			Serial.print("(");Serial.print(now); Serial.print(")  RxTIMEMOUT! delt="); Serial.print(millis()-RxTimeoutStarttime); Serial.print("  ndx="); Serial.println(ndx);
			ndx = 0;	//its been awhile....
		}
		RxTimeoutStarttime = now;		//hold off any timeout or further transmissions
		TxTimeoutStarttime = now;		//hold off any timeout or further transmissions

		if (ndx >= NUMSCHARS)	{Serial.println("!"); ndx = NUMSCHARS-1;  }//overrun!!!!!!
		if (rc == TERMINATOR_CHAR)
		{
			receivedSChars[ndx] = 0;	//null termination, if anybody cares (LIKE BELOW)
			enumPacktConditition stat = verifyPacket(receivedSChars, ndx);//dont include 
			ndx = 0;
		///	Serial.println("**OUTDONE");
			return stat;
		}
		receivedSChars[ndx++]=rc;
	}
///	if (started) Serial.println("**OUT");
	return eNone;
}

/*************************** SENDING ***************************/
//void addAsciiChar(char c, short *chksm, String* msg)
//{	
//	if (chksm)
//		(*chksm)+=c;
//	(*msg)+= String(c);
//	Serial.println(*msg);
//}

void setDataAndCheckSumAndTerminator(char * xmitBuf, char * data, byte dataLen)
{	//ASSUMES ASCII DATA
	memcpy(&xmitBuf[3], data, dataLen); 
	short chksm=0;
	for (int i=0; i<dataLen+3; i++)
		chksm+=xmitBuf[i];
	chksm %=100;
	xmitBuf[3+dataLen]=0x30+(chksm / 10);
	xmitBuf[4+dataLen]=0x30+(chksm % 10);
	xmitBuf[5+dataLen]=TERMINATOR_CHAR;
	xmitBuf[6+dataLen]=0;
}

void sendReply(char retStat, char* data, byte dataLen)
{
	delay(1);
	char * xmitBuf = new char[dataLen+6];
	xmitBuf[0] = receivedSChars[1];	//swap src and dest
	xmitBuf[1] = receivedSChars[0];	//swap src and dest
	xmitBuf[2] = retStat;
	setDataAndCheckSumAndTerminator(xmitBuf, data, dataLen);
//	Serial.print("sendReply - ");		Serial.println(xmitBuf); 
	mySerial->stopListening();
	mySerial->write(xmitBuf);
	mySerial->listen();
	delete xmitBuf;
}

bool cmdSentWaitingForReply=false;
void SendCmd(char targID, char fnc, char* data, byte dataLen)
{
	digitalWrite(DBUG_PIN, HIGH);
	char * xmitBuf = new char[dataLen+7];
	xmitBuf[0] = targID;
	xmitBuf[1] = myID;
	xmitBuf[2] = fnc;
	setDataAndCheckSumAndTerminator(xmitBuf, data, dataLen);
//	Serial.print("SendCmd - ");		Serial.println(xmitBuf); 
	mySerial->stopListening();
	noInterrupts();
	mySerial->write(xmitBuf);
	interrupts();
	TxTimeoutStarttime=millis();			//start timing after end/done
	digitalWrite(DBUG_PIN, LOW);
	mySerial->listen();
	cmdSentWaitingForReply = true;
	delete xmitBuf;
}

/*** for replies *****/
#define ACK '!'
#define NAK 'x'
#define NAKNOFUNCT "NoFunct"
