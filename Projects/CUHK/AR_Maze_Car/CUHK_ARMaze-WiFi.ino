#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);

const int led = 2;

void handleRoot();
void handleLED();
void handleNotFound();

void setup(void){
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  pinMode(led, OUTPUT);

  wifiMulti.addAP("CCRB", "c0c0r0b02017");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/fw", HTTP_GET, handleForward);
  server.on("/bw", HTTP_GET, handleBackward);
  server.on("/lt", HTTP_GET, handleLeft);
  server.on("/rt", HTTP_GET, handleRight);
  server.on("/st", HTTP_GET, handleStop);
  
  server.onNotFound(handleNotFound);
  
  // server.on("/LED", HTTP_POST, handleLED);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}

/* void handleRoot() {
  server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}*/

void handleForward() {
  digitalWrite(led,!digitalRead(led));
  Serial.write(50);
  // server.sendHeader("Location","/LED");
  server.send(200);
}

void handleBackward() {
  digitalWrite(led,!digitalRead(led));
  Serial.write(51);
  // server.sendHeader("Location","/LED");
  server.send(200);
}

void handleLeft() {
  digitalWrite(led,!digitalRead(led));
  Serial.write(52);
  // server.sendHeader("Location","/LED");
  server.send(200);
}

void handleRight() {
  digitalWrite(led,!digitalRead(led));
  Serial.write(53);
  // server.sendHeader("Location","/LED");
  server.send(200);
}

void handleStop() {
  digitalWrite(led,!digitalRead(led));
  Serial.write(54);
  // server.sendHeader("Location","/LED");
  server.send(200);
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}
