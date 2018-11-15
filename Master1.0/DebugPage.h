#pragma once

const char DEBUG_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<style>
div.a {
    text-indent: 40px;
}

div.b {
    text-indent: 80px;
}

div.c {
    text-indent: 120px;
}
p0 {
    display: block;
    margin-top: 0px;
    margin-bottom: 0em;
    margin-left: 0;
    margin-right: 0;
}
</style>
</head>

<body>
<!DOCTYPE html>
<html>
<body>
This is a scheme by which short slow msgs can be overlayed on a DC Power line to distributed devices.  This is a single master, multislave arranement and is roughly based on a scheme outlined <a href="http://www.kranenborg.org/ee/picaxe/SerialPower/V3.0/PicaxeSerialPowerNetwork_V3.0.pdf">here<a>.  The Master initiates any comm with the Msg (as bytes):<br><br>
<div class="a"><strong>
	<p0>&lt;dst&gt;&lt;src&gt;&lt;func&gt;[&lt;data1&gt;&lt;data2&gt;]&lt;crc&gt;</p0>
	<p0>  Master Command: <input type="text" id="Command">
<input type="button" onclick="sendCommand()" value="Send"></p0></strong>
</div>
<br>
<div class="b">
		<p0>&lt;dst&gt;&lt;src&gt; = ASCII; suggest starting with 'a'; master could be '~'</p0>
		<p0>&lt;func&gt; = Function.  Some universal functions are:</p0>
</div>
<div class="c">
				<p0>'?' - ACK if you're there (timeout if not)</p0>
				<p0>'1'	 - On</p0>
				<p0>'0' - Off</p0>
</div>
<div class="b">
		<p0>[&lt;data1&gt;&lt;data2&gt;] = optional data as dictated by the &lt;func&gt;. </p0>
		<p0>&lt;crc&gt; = (calculated; do not enter)</p0>
<br>
</div>
After sending the Msg, the output line must be tri-stated (x uSec Delay?) to allow the slave to transmit.  A timeout allows to recognize the slave as being offline.  The Slave must Reply with (as bytes):<br><br>
<div class="a">
	<strong><p0>&lt;src&gt;&lt;dst&gt;&lt;returnStat&gt;[&lt;data1&gt;&lt;data2&gt;]&lt;crc&gt;</p0>
	<p0>Slave Response:  <input type="text" id="Response"></p0></strong>
<br>

</div>
<div class="b">
		<p0>&lt;dst&gt;&lt;src&gt; - ASCII; suggest starting with 'a'; master could be '~'.</p0>
		<p0>&lt;returnStat&gt; - Some universal functions are:</p0>
</div>
<div class="c">
				<p0>'!' - ACK.  Function executed</p0>
				<p0>'x'	 -NAK.  (data info probably follows)</p0>
				<p0>'?'	 -"NoFunct"</p0>
</div>
<div class="b">
		<pF0>[&lt;data1&gt;&lt;data2&gt;] = optional data as dictated by the &lt;func&gt;.</p0>
		<p0>&lt;crc&gt; = (not displayed here).</p0>
</div>
<button onclick="location.href = '/gotoHome';" class="float-left submit-button" >Home</button> 
<button onclick="location.href = '/gotoIO';" class="float-left submit-button" >IO</button><br>
</body>
</html>

</body>
<script>
function sendCommand(){
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
	  var resp = this.responseText;
	  document.getElementById("Response").value = resp;
	}
  };
  var txt = document.getElementById("Command").value;
  xhttp.open("GET", "sendCommand?msg="+txt, true);
  xhttp.send();
}
</script>
</html>
)=====";


void addAsciiChar(char c, short *chksm, String* out)
{	
	if (chksm)
		(*chksm)+=c;
	(*out)+= String(c);
	Serial.println(*out);
}

String out;
bool includeDataSize=false;

void encodeCommand(String * out, const char * inpBuff, int inpBuffSize)
{
	*out ="";
	short chksm=0;
	Serial.print("/************* ENCODE *************/\n");
	for (int i = 0; i < inpBuffSize; i++)
	{	
		if (i==3 && includeDataSize) 
		{
			int len=inpBuffSize-4; //dst/src/fnc/enddelim
			Serial.printf("\nlen=%d (%c %c)", len, 0x30+(len / 10), 0x30+(len % 10));
			addAsciiChar(0x30+(len / 10), &chksm, out);		//msb
			addAsciiChar(0x30+(len % 10), &chksm, out);		//lsb
			Serial.printf("\n"); /*Yes, fall thru*/
		}
//			Serial.printf("i=%d   ", i);
		addAsciiChar(inpBuff[i], &chksm, out);
	}
	chksm %=100;
	addAsciiChar(0x30+(chksm / 10), NULL, out);
	addAsciiChar(0x30+(chksm % 10), NULL, out);		
}

void verifyMsg(String * msg)
{
	Serial.print("/************* DECODE *************/\n");
	const char * chars = msg->c_str();
	int incommingDataLen=0;
	int calcedChksm=0;
	int dataLen =0;
	int i;
	if (includeDataSize)
	{	//if guess i never verified length
		int sentDataLen = String(chars[3]).toInt()*10 + String(chars[4]).toInt();
		Serial.printf("sentDataLen=%d ... expected len=%d\n", sentDataLen, msg->length()-(3/*s/d/f*/+2/*len*/+2/*cksm*/));
	}
	for (i = 0; i < msg->length()-2; i++)
	{
		calcedChksm += chars[i];
	}
	calcedChksm %=100;
	int receivedChksm = String(chars[i]).toInt()*10 + String(chars[i+1]).toInt();
	Serial.printf("rcvd chksm=%d .... calc'd chksm=%d\n", receivedChksm, calcedChksm);
}

void handleDebugCommand()
{
	String i = server.arg("msg"); 
	Serial.printf("i=%s", i.c_str());
	String outStr;
	encodeCommand(&outStr, i.c_str(), i.length());
	verifyMsg(&outStr);
	
	//send command out to slaves, wait for reply

	server.send(200, "text/plane", "xxx-"+i); 

}
