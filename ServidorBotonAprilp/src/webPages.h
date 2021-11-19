const char MAIN_PAGE[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <style>
    </style>
</head>
<body onload="selectDelay();"> 
    <button type="range"  onclick = "location.href = 'delay.html'" >
        activar tiempo de encendido
    </button>   
</body>
<script>
    function selectDelay(){
        const Http = new XMLHttpRequest();
        const url='http://192.168.4.1/del';
        const data = false ; 
        Http.onreadystatechange = function () {
            if (this.readyState != 4) return;
            if (this.status == 200) {
            }
        }
        Http.open("POST", url,true);
        Http.setRequestHeader('Content-Type', 'text/html');
        Http.send(data);    
    }
</script>
</html>)=====" ;  

const char delayHMTL[] =R"=====( 
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<style type="text/css">
    #delayTimeButton {
    position: relative;
    height: 200px;
    background: #ccc;
    outline: none;
    -webkit-appearance: slider-vertical;
    }
    #timeDelayView {
        font-size: 90%;
    }
	#sizeview{
      font-size: 200%;  
	}
</style>
<body onload="selectDelay()";>
    <div class="slider-wrapper">
        <input type="range" min="0" max="180" id="delayTimeButton" >  
    </div>
    <label for ="timeDelayView" id="sizeview">
    segundos:<output id = "timeDelayView" name="delaysec" for="timeDelayButton"> 38  </output> 
	</label>
</body>
<script> 
    var connection = new WebSocket('ws://'+'192.168.4.1'+':81/', ['arduino']); 
    connection.onopen = function () 
	{};
	connection.onerror = function (error) { console.log('WebSocket Error ', error);};
	connection.onmessage = function (e)   { console.log('Server: ', e.data);};
    var input = document.getElementById('delayTimeButton');
    var output = document.getElementById('timeDelayView');
        output.innerHTML = input.value ; 
        input.oninput = function() {
            output.innerHTML = this.value;
            connection.send(this.value) ; 
       }
    function selectDelay(){
        const Http = new XMLHttpRequest();
        const url='http://192.168.4.1/del';
        const data = true ; 
        Http.onreadystatechange = function () {
            if (this.readyState != 4) return;
            if (this.status == 200) {
            }
        }
        Http.open("POST", url,true);
        Http.setRequestHeader('Content-Type', 'text/html');
        Http.send(data);    
    }
</script>
</html>)=====" ; 


