#pragma once
Adafruit_MCP23017 mcp;
#define OUT1	3 //PA1
#define OUT2	4 //PA2
#define OUT3	5 //PA3
#define OUT4	6 //PA4
const int OUTIndexes[] = {OUT1, OUT2, OUT3, OUT4};
#define INP1	8+6 //PB6
#define INP2	8+4 //PB5	//note INPapped
#define INP3	8+5 //PB4	//note INPapped
#define INP4	8+3 //PB3
const int INPIndexes[] = {INP1, INP2, INP3, INP4};

int outputForces[4] = {0,0,0,0};	
int inputs[4] = {0,0,0,0};	
int inputForces[4] = {0,0,0,0};	
int outputs[4] = {0,0,0,0};	

void initIO()
{
	mcp.begin();      // use default address 0
 	for (int i = 0; i < sizeof(OUTIndexes)/sizeof(OUTIndexes[0]); i++)
	{
		mcp.pinMode(OUTIndexes[i], OUTPUT);
		mcp.digitalWrite(OUTIndexes[i], 0);
	}
 	for (int i = 0; i < sizeof(INPIndexes)/sizeof(INPIndexes[0]); i++)
	{
		mcp.pinMode(INPIndexes[i], INPUT);
		mcp.pullUp(INPIndexes[i], HIGH);
	}
}
void handleOStateChange(int indx, int newOstate)
{
	mcp.digitalWrite(OUTIndexes[indx], newOstate);
	outputs[indx] = newOstate;
	Serial.printf("Output #%d is now %d\n", indx, newOstate);
}
void handleIStateChange(int indx, int newIState)
{
	inputs[indx] = newIState;
	Serial.printf("Input #%d is now %d\n", indx, newIState);
}

void handleIO() {
	String payload;
	int n = sizeof(inputForces)/sizeof(inputForces[0]);
	for (int i=0; i<n; i++)
	{
		payload += inputForces[i];
		payload += " ";
	}
	n = sizeof(inputs)/sizeof(inputs[0]);
	for (int i=0; i<n; i++)
	{
		payload += inputs[i];
		payload += " ";
	}
	n = sizeof(outputForces)/sizeof(outputForces[0]);
	for (int i=0; i<n; i++)
	{
		payload += outputForces[i];
		payload += " ";
	}
	n = sizeof(outputs)/sizeof(outputs[0]);
	for (int i=0; i<n; i++)
	{
		payload += outputs[i];
		payload += " ";
	}
//////	Serial.println(payload);
	server.send(200, "text/plane", payload); //Send ADC value only to client ajax request
}

void handleclickedIButton()
{
	int i = server.arg("index").toInt(); 
	if (inputForces[i] == 0)	return;
	handleIStateChange(i, !inputs[i]);
}

void handleclickedOButton()
{
	int i = server.arg("index").toInt(); 
	if (outputForces[i] == 0)	return;
	handleOStateChange(i, !outputs[i]);
}

void handleclickedIForceCheck()
{
	int i = server.arg("index").toInt(); 
	String st = server.arg("state"); 
	inputForces[i]= st=="true"?1:0;
	Serial.printf("IForce index= %d state=%d (%s)\n", i, inputForces[i], st.c_str());
}
void handleclickedOForceCheck()
{
	int i = server.arg("index").toInt(); 
	String st = server.arg("state"); 
	outputForces[i]= st=="true"?1:0;
	Serial.printf("OForce index= %d state=%d (%s)\n", i, outputForces[i], st.c_str());
}

/***************************************/
void   SetIOserverHandlers()
{
  server.on("/gotoIO", handleIOPage); 
  server.on("/readIO", handleIO);
  server.on("/clickedIButton", handleclickedIButton);
  server.on("/clickedOButton", handleclickedOButton);
  server.on("/clickedIForceCheck", handleclickedIForceCheck);
  server.on("/clickedOForceCheck", handleclickedOForceCheck);
}

