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
<h1>The GarberPark web page </h1>
Master1.1.ino - v1.1<br> <br> <br> 
</div>
<button onclick="location.href = '/gotoIO';" class="float-left submit-button" >IO</button>
<button onclick="location.href = '/gotoDebug';" class="float-left submit-button" >Debug</button>
<br>
<br>
<div class="row">
  <div class="column">
  	<strong>
	Battery(v):<br>
	Pannel(v)<br>
	</strong>
  </div>
  <div class="column">
    <span id="ADCValueBattery"></span><br>
    <span id="ADCValuePanel"></span><br>
  </div>
  <div class="column">
     <span id="Battery%"></span><br>
  </div>
</div>

<br><br>

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
  <div></div>


<br><br><br>
  <button onclick="location.href = '/gotoIO';" class="float-left submit-button" >IO</button>
  <button onclick="location.href = '/gotoDebug';" class="float-left submit-button" >Debug</button>
<script>
setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getAnalogData();
  getIOData();
}, 2000); //2000mSeconds update rate

function getAnalogData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
	  var vals = this.responseText.split(" ");	  
 	  var i;
	  var valsLookup = ["ADCValueBattery","ADCValuePanel"];
	  <!-- validate length?? -->
	  for (i = 0; i < valsLookup.length; i++) {
		  var ele = document.getElementById(valsLookup[i]);
		  if (ele) {
	         ele.innerHTML = vals[i];
			}
	  }
    }
  };
  xhttp.open("GET", "readADC", true);
  xhttp.send();
}


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
function getIOData() {
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
</body>
</html>
)=====";
