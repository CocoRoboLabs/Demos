#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

int inBytes[3];

const char *SERVER_WIFI_SSID = "...";
const char *SERVER_WIFI_PASS = "...";

void setupWiFi()
{
  Serial.print("Connecting to WiFi ");
  WiFi.begin(SERVER_WIFI_SSID, SERVER_WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected");
}

void setup() {
  Serial.begin(115200);
  setupWiFi();

}

void loop() {
  HTTPClient http;

  while (Serial.available() < 1) {}
  for (int n = 0; n < 1; n++)
  {
    inBytes[n] = Serial.read();
  }

  if ( inBytes[0] == 1 ) {
    //Let is try a GET request first
    http.begin("http://maker.ifttt.com/trigger/cocomod_bt_comm/with/key/kaqcI2jTckZ_URCwxoHnVweVbVrUdfDiCW0VzEPEOnd");
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)
    {
      Serial.print("HTTP response code ");
      Serial.println(httpCode);
      String response = http.getString();
      Serial.println(response);
    }
    else
    {
      Serial.println("Error in HTTP request");
    }
    http.end();
  }
  else if (inBytes[1] == 0) {
    Serial.println("On hold.");
  }
}
