const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
* {
    box-sizing: border-box;
}

/* Create two equal columns that floats next to each other */
.column {
	float:left;
	column-width: 100px;
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
<h1>The GarberPark web page V1.1</h1>
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
	</strong>
  </div>
  <div class="column">
    <span id="ADCValue6"></span><br>
    <span id="ADCValue2"></span><br>
	<br>
    <span id="ADCValue1"></span><br>
    <span id="ADCValue3"></span><br>
    <br>
    <span id="ADCValue0"></span>
<!--    <br>Some text..-->
  </div>
  <div class="column">
    <br>
    <span id="ChargedLED"></span><br>
	<br>
	<br>
	<br>
    <span id="ChargingLED"></span><br>
    <br>
    <span id="UnderVLED"></span>
<!--    <br>Some text..-->
  </div>
</div>

<p></p><p></p>
<button onclick="location.href = '/gotoAux';" class="float-left submit-button" >Auxx</button>

<script>
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
	  for (i = 0; i < vals.length; i++) {
		  var id="ADCValue"+i;
		  var ele = document.getElementById(id);
		  if (ele) {
	         ele.innerHTML = vals[i];
			 ele.style.backgroundColor="white";
		     if (vals[i]>200)
			   ele.style.backgroundColor="red";
			}
		  //else
		  //   alert("ele #" + i + " doesnt exist");
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
/*****************************************************************************/
const char AUX_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>

<div id="demoAux">
<h1>Some Aux Page</h1>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p><button onclick="location.href = '/';" class="float-left submit-button" >Home</button></p>
</div>

</script>
</body>
</html>
)=====";

