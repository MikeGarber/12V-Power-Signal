#pragma once
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>

<p>Display some text when the checkbox is checked:</p>

Checkbox: <input type="checkbox" id="myCheck"  onclick="myFunction()">

<p id="text" style="display:none">Checkbox is xxxCHECKED!</p>

<script>
function myFunction() {
    var checkBox = document.getElementById("myCheck");
    var state = checkBox.checked;
    var msg = "test_"+state;
	var text=document.getElementById("text");
    if (checkBox.checked == true){
        text.style.display = "block";
    } else {
       text.style.display = "none";
    }
    text.innerHTML=msg;
}
</script>

</body>
</html>
)=====";