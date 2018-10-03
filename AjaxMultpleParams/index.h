const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<div id="demo">
<h1>The Garberpark web page </h1>
</div>
<div>

<p>Pwr&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;: <span id="ADCValue6">x&nbsp; &nbsp; &nbsp; </span> 
 <br /> Charged_N is : <span id="ADCValue2">x</span></p>
<p><br /> 
  Charging_N is : <span id="ADCValue3">x</span> <br /> Charging_P is : <span id="ADCValue1">x</span></p>
<p><br /> 
  UnderV is : <span id="ADCValue0">x</span></p>
</div>
<p>&nbsp;</p>
<p>&nbsp;</p>
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

