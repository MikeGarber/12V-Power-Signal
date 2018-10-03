const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<div id="demo">
<h1>The Garberpark web page without refresh</h1>
	<button type="button" onclick="sendData(1)">LED ON</button>
	<button type="button" onclick="sendData(0)">LED OFF</button><BR>
</div>
<div>
	ADC Value is : <span id="ADCValue">0</span><br>
    LED State is : <span id="LEDState">NA</span>
</div>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p><button type="button" onclick="auxPage()">Aux</button></p>
<script>
function sendData(led) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("LEDState").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "setLED?LEDstate="+led, true);
  xhttp.send();
}

function auxPage() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "gotoAux", true);
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
      document.getElementById("ADCValue").innerHTML =
      this.responseText;
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
<p><button type="button" onclick="homePage()">Home</button></p>
</div>

function homePage() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "gotoHome", true);
  xhttp.send();
}
</script>
</body>
</html>
)=====";

