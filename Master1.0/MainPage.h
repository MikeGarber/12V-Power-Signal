#pragma once
const char MAINpage[] PROGMEM = R"=====(
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
	column-width: 50px;
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
<div id="demo">
<h1>The GarberPark web page <br> Master1.0.ino - v1.1</h1>
</div>

<div class="row">
  <div class="column">
  	<strong>
	Pwr:<br>
	Charged_N<br>
	<br>
	Charging_P<br>
	Charging_N<br>
	<br>
    UnderV:<br>
	<br>
    (unused):
	</strong>
  </div>
  <div class="column">
    <br>
    <span id="LEDchgd"></span><br>
	<br>
    <span id=""DELTAchgng"></span><br>
    <span id="LEDchgng"></span><br>
    <br>
    <span id="LEDunder"></span>
  </div>
  <div class="column">
    <span id="ADCValuePwr"></span><br>
    <span id="ADCValueChdN"></span><br>
	<br>
    <span id="ADCValueChgngP"></span><br>
    <span id="ADCValueChgngN"></span><br>
    <br>
    <span id="ADCValueUV"></span><br>
    <br>
    <span id="ADCValueUnused"></span>
  </div>
  <div class="column">
    <br>
    <span id="ChargedLED">Charged</span><br>
	<br>
	<br>
    <span id="ChargingLED">Charging</span><br>
    <br>
    <span id="UnderVLED">UnderVoltage</span>
  </div>
</div>
<br><br>
Master Command: <input type="text" id="Command">
<input type="button" onclick="sendCommand()" value="Send"><br>
Slave Response:  <input type="text" id="Response">
<br>
<button onclick="location.href = '/gotoIO';" class="float-left submit-button" >IO</button>


<script>
function sendCommand(){
	var txt = document.getElementById("Command").value;

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
	  var resp = this.responseText;
	  document.getElementById("Response").value = resp;
	}
  };
  xhttp.open("GET", "sendCommand?msg="+txt, true);
  xhttp.send();
}


setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 2000); //2000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
	  var vals = this.responseText.split(" ");	  
 	  var i;
	  var valsLookup = ["ADCValuePwr","ADCValueChdN","ADCValueChgngP","ADCValueChgngN","ADCValueUV","ADCValueUnused"];
	  <!-- validate length?? -->
	  for (i = 0; i < valsLookup.length; i++) {
		  var ele = document.getElementById(valsLookup[i]);
		  if (ele) {
	         ele.innerHTML = vals[i];
			}
	  }

	  var ledsIDLookup =      ["ChargedLED","ChargingLED","UnderVLED"];
	  var ledsOnColorLookup = ["#47ff56","#f2ff47","#ff5047"];<!--bold grn, yel, red-->
	  var ledsOffColorLookup = ["#baf2ba","#ffffbc","#f2baba"]; <!--pale grn, yel, red-->
	  <!-- validate length?? -->
	  for (i = 0; i < ledsIDLookup.length; i++) {
		  var ele = document.getElementById(ledsIDLookup[i]);
		  if (ele && (vals[valsLookup.length + i]=="ON")) {
			 ele.style.backgroundColor=ledsOnColorLookup[i];
	   		 ele.style.color   =     "#000000";
		  }
		  else {
			 ele.style.backgroundColor="white";
	   		 ele.style.color   =       ledsOffColorLookup[i];
			}
	  }
    }
  };
  xhttp.open("GET", "readADC", true);
  xhttp.send();
}
</script>
</body>
</html>
)=====";
