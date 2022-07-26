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
  xhttp.open('GET', '/relays', true);
  xhttp.send();

    /* 
* An event handler is called within the context of the element that launched it.
* The identifier 'this' will refer to the DOM element that is clicked. 
*/
  setTimeout(function(){
    buttons = Array.from(document.getElementsByClassName("toggle-button"));
    for(var i = 0; i < buttons.length; i++){
      buttons[i].addEventListener('click', function(){
        websocket.send("toggle-" + this.id);
      });
    }

    // ----- Starts websocket -----
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
  var button = document.getElementById(message.slice(0,2));
  if(message.includes("on")){
    button.style.backgroundImage = "url('on.svg')";
  }
  else if(message.includes("off")){
    button.style.backgroundImage = "url('off.svg')";
  }
}
