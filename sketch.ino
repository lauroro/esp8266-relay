/*
 * Normally open relays assumed.
 * HIGH (bool:true)   close the circuit -> turn on
 * LOW  (bool: false) open the circuit  -> turn off
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>


#define RELAYS_NUM      4
#define RELAY_0         5   //D1 (GPIO 5)
#define RELAY_1         4   //D2 (GPIO 4)
#define RELAY_2         14  //D5 (GPIO 14)
#define RELAY_3         12  //D6 (GPIO 12)


//Wifi params
const char* ssid = "your-ssid-here";
const char* password = "your-passwd-here";


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


// State variables for each relay
bool r0 = false;
bool r1 = false;
bool r2 = false;
bool r3 = false;

String boolToState(bool b){
  if(b) return "on";
  else  return "off";
}


void resolveConnect(uint32_t clientID){
  String msg = "r0";
  msg += boolToState(r0); 
  ws.text(clientID, msg);
  msg = "r1";
  msg += boolToState(r1);  
  ws.text(clientID, msg);
  msg = "r2";
  msg += boolToState(r2);  
  ws.text(clientID, msg);
  msg = "r3";
  msg += boolToState(r3);  
  ws.text(clientID, msg);
}



void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String message = (char*)data;
    Serial.println(message);
    if(message.indexOf("toggle-r0") >= 0){
      r0 = !r0;
      if(!r0)
        digitalWrite(RELAY_0, LOW);
      else
        digitalWrite(RELAY_0, HIGH);
      String msg = "r0";
      msg += boolToState(r0);
      ws.textAll(msg);
    }
    else if(message.indexOf("toggle-r1") >= 0){
      r1 = !r1;
      if(!r1)
        digitalWrite(RELAY_1, LOW);
      else
        digitalWrite(RELAY_1, HIGH);
      String msg = "r1";
      msg += boolToState(r1);
      ws.textAll(msg);    
    }
    else if(message.indexOf("toggle-r2") >= 0){
      r2 = !r2;
      if(!r2)
        digitalWrite(RELAY_2, LOW);
      else
        digitalWrite(RELAY_2, HIGH);
      String msg = "r2";
      msg += boolToState(r2);  
      ws.textAll(msg);    
    }
    else if(message.indexOf("toggle-r3") >= 0){
      r3 = !r3;
      if(!r3)
        digitalWrite(RELAY_3, LOW);
      else
        digitalWrite(RELAY_3, HIGH); 
      String msg = "r3";
      msg += boolToState(r3);
      ws.textAll(msg);    
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      //Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      resolveConnect(client->id());
      break;
    case WS_EVT_DISCONNECT:
      //Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}




void setup() {
  Serial.begin(115200);

  //Set relay pins as output
  pinMode(RELAY_0, OUTPUT);
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);

  //N.O. configurations: HIGH -> close the circuit, LOW -> open the circuit
  digitalWrite(RELAY_0, LOW);
  digitalWrite(RELAY_1, LOW);
  digitalWrite(RELAY_2, LOW);
  digitalWrite(RELAY_3, LOW);

  
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());

  ws.onEvent(onEvent);
  server.addHandler(&ws);

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });
  server.serveStatic("/", LittleFS, "/");

  server.on("/relays", HTTP_GET, [](AsyncWebServerRequest *request){
    String relays = "";
    relays.reserve(2000);
    for(uint8_t i=0; i < RELAYS_NUM; i++) {
      relays += "<div class='button-div'><h2>Relay ";
      relays += i;
      relays += "</h2><button class='toggle-button' id='r";
      relays += i;
      relays += "'>";
      relays += "</button></div>";
    }
    request->send(200, "text/plain", relays);
  });

  // Start server
  server.begin();
}

void loop() {}
