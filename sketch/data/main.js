// ----------- GENERAL ----------------

/*
* <ESP-IP-ADDRESS>[/?name=name-value&relays=relays-value]
*
* // Fields
* name-value : everything
* relays-value : all || r0 || r1 || r2 || r3 || everything else -> default 
*/

// Defaults
var deviceName = "Relays Controller";
var relays = "all";


var queryString = decodeURIComponent(window.location.search); //parsing
queryString = queryString.substring(1); 
var queries = queryString.split("&"); 
for (var i = 0; i < queries.length; i++) { 
  var values = queries[i].split("=");
  if(values[0] == "name")
    deviceName = values[1];
  if(values[0] == "relays")
    if(values[1] == "r0" || values[1] == "r1" || values[1] == "r2" || values[1] == "r3")
      relays = values[1];
} 

document.getElementById("device-name").textContent = deviceName;



var gateway = "ws://"+window.location.hostname+"/ws";
var websocket;

window.addEventListener('load', function(){
  // ----- Prepare relays buttons -----
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("buttons-container").innerHTML = this.responseText;
    }
  };
  var query = "/"+relays;
  xhttp.open('GET', query, true);
  xhttp.send();

  /* 
  * An event handler is called within the context of the element that launched it.
  * The identifier 'this' will refer to the DOM element that is clicked. 
  */
  setTimeout(function(){
    // listener
    buttons = Array.from(document.getElementsByClassName("toggle-button"));
    for(var i = 0; i < buttons.length; i++){
      buttons[i].addEventListener('click', function(){
        websocket.send("toggle-" + this.id);
      });
    }
    // Starts websocket
    initWebSocket();
  },300);
});



// ------------------ WebSocket ----------------------

function initWebSocket() {
  console.log('Trying to open a WebSocket connection...');
  websocket = new WebSocket(gateway);
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}

function onOpen(event) {
  console.log('Connection opened');
}

function onClose(event) {
  console.log('Connection closed');
  setTimeout(initWebSocket, 1000);
}

function onMessage(event) {
  // expected message: rXon | rXoff
  var message = event.data;
  var id = message.slice(0,2);
  if(relays == "all" || id == relays){
    var button = document.getElementById(id);
    if(message.includes("on")){
      button.style.backgroundImage = "url('on.svg')";
    }
    else if(message.includes("off")){
      button.style.backgroundImage = "url('off.svg')";
    }
  }
}
