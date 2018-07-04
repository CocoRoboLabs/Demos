#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

static const char ssid[] = "...";
static const char password[] = "...";
MDNSResponder mdns;

static void writeLED();

ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
<title>CocoRobo Wi-Fi Module Demo 001</title>
<style>
body { text-align: center;background-color: #ffffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }
.myButton {
  background-color:#44c767;
  -moz-border-radius:28px;
  -webkit-border-radius:28px;
  border-radius:28px;
  border:1px solid #18ab29;
  display:inline-block;
  cursor:pointer;
  color:#ffffff;
  font-family:Arial;
  font-weight:bold;
  font-size:14px;
  padding:16px 31px;
  text-decoration:none;
  text-shadow:0px 1px 0px #2f6627;
  width: 100%;
  margin-bottom: 5px;
}
.myButton:hover {
  background-color:#5cbf2a;
}
.myButton:active {
  position:relative;
  top:1px;
}

.headerA {
  padding-bottom:15px;
}
</style>
<script>
var websock;
function start() {
  websock = new WebSocket('ws://' + window.location.hostname + ':81/');
  websock.onopen = function(evt) { console.log('websock open'); };
  websock.onclose = function(evt) { console.log('websock close'); };
  websock.onerror = function(evt) { console.log(evt); };
  websock.onmessage = function(evt) {
    console.log(evt);
    var e = document.getElementById('LEDStatus');
    if (evt.data === 'blue') {
      e.style.color = 'black';
    }
    else if (evt.data === 'red') {
      e.style.color = 'black';
    }
    else if (evt.data === 'off') {
      e.style.color = 'black';
    }
    else {
      console.log('unknown event');
    }
  };
}
function buttonclick(e) {
  websock.send(e.id);
}
</script>
</head>
<body onload="javascript:start();">
<h1>CocoRobo RGB LED Real-time Control Demo</h1>
<div class="headerA" id="LEDStatus"><b>Press any button:</b></div>
<table width="100%">
  <tr>
    <td width="33.333%"><button class="myButton" id="blue"  type="button" onclick="buttonclick(this);">Blue</button></td>
    <td width="33.333%"><button class="myButton" id="red"  type="button" onclick="buttonclick(this);">Red</button></td>
    <td width="33.333%"><button class="myButton" id="off" type="button" onclick="buttonclick(this);">Turn Off</button></td>
  </tr>
</table>
<div style="padding-top:20px;">CocoRobo LTD. 2018</div>
</body>
</html>
)rawliteral";

int LEDStatus;

// Commands sent through Web Socket
const char CBLU[] = "blue";
const char CRED[] = "red";
const char COFF[] = "off";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{  
  // Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
  switch(type) {
    case WStype_DISCONNECTED:
      // Serial.printf("[%u] Disconnected!\r\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        // Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        if (LEDStatus = 1) {
          webSocket.sendTXT(num, CBLU, strlen(CBLU));
        } else if (LEDStatus = 2) {
          webSocket.sendTXT(num, CRED, strlen(CRED));
        } else if (LEDStatus = 3) {
          webSocket.sendTXT(num, COFF, strlen(COFF));
        }
      }
      break;
    case WStype_TEXT:
      // Serial.printf("[%u] get Text: %s\r\n", num, payload);
      if (strcmp(CBLU, (const char *)payload) == 0) {
        writeLED(1);
      }
      else if (strcmp(CRED, (const char *)payload) == 0) {
        writeLED(2);
      }
      else if (strcmp(COFF, (const char *)payload) == 0) {
        writeLED(3);
      }
      else {
        // Serial.println("Unknown command");
      }
      // send data to all connected clients
      webSocket.broadcastTXT(payload, length);
      break;
    case WStype_BIN:
      // Serial.printf("[%u] get binary length: %u\r\n", num, length);
      hexdump(payload, length);

      // echo data back to browser
      webSocket.sendBIN(num, payload, length);
      break;
    default:
      // Serial.printf("Invalid WStype [%d]\r\n", type);
      break;
  }
}

void handleRoot()
{
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void writeLED(int nLED) {
  LEDStatus = nLED;
  switch(nLED) {
    case(1):
      Serial.write(50);
      break;
    case(2):
      Serial.write(52);
      break;
    case(3):
      Serial.write(54);
      break;
  }
}

/* ======================
   ===== Setup Part =====
   ====================== */

void setup()
{
  pinMode(LEDPIN, OUTPUT);
  writeLED(1);

  Serial.begin(9600);

  //Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\r\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFiMulti.addAP(ssid, password);

  while(WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("espWebSock", WiFi.localIP())) {
    Serial.println("MDNS responder started");
    mdns.addService("http", "tcp", 80);
    mdns.addService("ws", "tcp", 81);
  }
  else {
    Serial.println("MDNS.begin failed");
  }
  Serial.print("Connect to http://espWebSock.local or http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
  server.handleClient();
}
