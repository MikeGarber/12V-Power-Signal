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
void   SetupIOserverHandlers()
{
  server.on("/gotoIO", handleIOPage); 
  server.on("/readIO", handleIO);
  server.on("/clickedIButton", handleclickedIButton);
  server.on("/clickedOButton", handleclickedOButton);
  server.on("/clickedIForceCheck", handleclickedIForceCheck);
  server.on("/clickedOForceCheck", handleclickedOForceCheck);
}

/***************************************/
void loopIO()
{
	for (int i = 0; i < sizeof(INPIndexes)/sizeof(INPIndexes[0]); i++)
	{
		if (inputForces[i] == 0)
		{
			int newI = !mcp.digitalRead(INPIndexes[i]);	//active low
			if (newI != inputs[i])	
				handleIStateChange(i, newI);
		}
	}
}

/*****************************************************************************/
const char IOpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
* {
    box-sizing: border-box;
}

/* Create equal columns that floats next to each other */
.column {
	float:left;
	column-width: 70px;
}

/* Clear floats after the columns */
.row:after {
    content: "";
    display: table;
    clear: both;
}
</style>
</head>
<body>
<div>
<h1>The IO Page</h1>
<br>
<div class="row">
  <div class="column">
    <b><u>I Force</b></u><br>
  	<input type="checkbox" id="IF0" onclick="sendIForce(0)" ><br>
  	<input type="checkbox" id="IF1" onclick="sendIForce(1)" ><br>
  	<input type="checkbox" id="IF2" onclick="sendIForce(2)" ><br>
  	<input type="checkbox" id="IF3" onclick="sendIForce(3)" ><br>
  </div>
  <div class="column">
    <b><u>Inputs</b></u><br>
  	<button onclick="sendIButtonClick(0)" class="float-left submit-button" ><span id="I0">Input0</span></button><br>
	<button onclick="sendIButtonClick(1)" class="float-left submit-button" ><span id="I1">Input1</span></button><br>
	<button onclick="sendIButtonClick(2)" class="float-left submit-button" ><span id="I2">Input2</span></button><br>
	<button onclick="sendIButtonClick(3)" class="float-left submit-button" ><span id="I3">Input3</span></button><br>
  </div>
  <div class="column">
    <b><u>O Force</b></u><br>
  	<input type="checkbox" id="OF0" onclick="sendOForce(0)" ><br>
  	<input type="checkbox" id="OF1" onclick="sendOForce(1)" ><br>
  	<input type="checkbox" id="OF2" onclick="sendOForce(2)" ><br>
  	<input type="checkbox" id="OF3" onclick="sendOForce(3)" ><br>
  </div>
  <div class="column">
    <b><u>Outputs</b></u><br>
  	<button onclick="sendOButtonClick(0)" class="float-left submit-button" ><span id="O0">Output0</span></button><br>
	<button onclick="sendOButtonClick(1)" class="float-left submit-button" ><span id="O1">Output1</span></button><br>
	<button onclick="sendOButtonClick(2)" class="float-left submit-button" ><span id="O2">Output2</span></button><br>
	<button onclick="sendOButtonClick(3)" class="float-left submit-button" ><span id="O3">Output3</span></button><br>
  </div>
<script>
setInterval(function() { getData(); }, 2000); 

function sendIForce(index) {
	var id = "IF" +index;
	var checkBox = document.getElementById(id);
	var state = checkBox.checked;
    var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "clickedIForceCheck?index="+index+"&state="+state, true);
  xhttp.send();
}
function sendOForce(index) {
	var id = "OF" +index;
	var checkBox = document.getElementById(id);
	var state = checkBox.checked;
    var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "clickedOForceCheck?index="+index+"&state="+state, true);
  xhttp.send();
}

function sendIButtonClick(index) {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "clickedIButton?index="+index, true);
  xhttp.send();
}
function sendOButtonClick(index) {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "clickedOButton?index="+index, true);
  xhttp.send();
}
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
	  var resp = this.responseText;
	  var vals = resp.split(" ");
 	  var i;
	  var ifvalsLookup = ["IF0","IF1","IF2","IF3"];
	  for (i = 0; i < ifvalsLookup.length; i++) {
		  var ele = document.getElementById(ifvalsLookup[i]);
		  if (ele)
			 ele.checked = (vals[i]==1);
	  }
	  var ivalsLookup = ["I0","I1","I2","I3"];
	  for (i = 0; i < ivalsLookup.length; i++) {
		  var ele = document.getElementById(ivalsLookup[i]);
		  if (ele) {
			 if (vals[i + ifvalsLookup.length]==1)
			   ele.style.backgroundColor="#f7ff2b";
			else
			   ele.style.backgroundColor="#d6d6d6";
			}
	  }
	  var ofvalsLookup = ["OF0","OF1","OF2","OF3"];
	  for (i = 0; i < ifvalsLookup.length; i++) {
		  var ele = document.getElementById(ofvalsLookup[i]);
		  if (ele)
			 ele.checked = (vals[i + ifvalsLookup.length + ivalsLookup.length]==1);
	  }
	  var ovalsLookup = ["O0","O1","O2","O3"];
	  for (i = 0; i < ovalsLookup.length; i++) {
		  var ele = document.getElementById(ovalsLookup[i]);
		  if (ele) {
			 if (vals[i + ifvalsLookup.length + ivalsLookup.length + ofvalsLookup.length]==1)
			   ele.style.backgroundColor="#f7ff2b";
			else
			   ele.style.backgroundColor="#d6d6d6";
			}
	  }
    }
  };
  xhttp.open("GET", "readIO", true);
  xhttp.send();
}

</script>

</div>
<br><br>
<p><button onclick="location.href = '/';" class="float-left submit-button" >Home</button></p>
</div>
</script>
</body>
</html>
)=====";
