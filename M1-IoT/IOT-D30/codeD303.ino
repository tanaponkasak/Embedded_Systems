#include <WiFi.h>
#include <HTTPClient.h>

#define WIFI_SSID "iPhoneOhm"
#define WIFI_PASS "2444666668888888"
#define WebHooksKey "c6Dw2mHZ9znYfzd0i5Im4U_wZEYKTCtNmV19i1dyUvI"
#define WebHooksEventName "ohm"

#define testSwitch1 22
#define testSwitch2 23
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  pinMode(testSwitch1, INPUT_PULLUP);
  pinMode(testSwitch2, INPUT_PULLUP);
  randomSeed(analogRead(33));
}
void loop() {
  if (digitalRead(testSwitch1) == LOW) {
    String serverName = "http://maker.ifttt.com/trigger/" +
                        String(WebHooksEventName) + "/with/key/" + String(WebHooksKey);
    String httpRequestData = "value1=" + String("Door Open Alarm");
    Serial.println("Server Name :" + serverName);
    Serial.println("json httpRequestData :" + httpRequestData);
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
      if (httpResponseCode == 200)
        Serial.println("Successfully sent");
      else
        Serial.println("Failed!");
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
  if (digitalRead(testSwitch2) == LOW) {
    String serverName = "http://maker.ifttt.com/trigger/" +
                        String(WebHooksEventName) + "/with/key/" + String(WebHooksKey);
    String httpRequestData = "value1=" + String("Intruders Alarm");
    Serial.println("Server Name :" + serverName);
    Serial.println("json httpRequestData :" + httpRequestData);
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
      if (httpResponseCode == 200)
        Serial.println("Successfully sent");
      else
        Serial.println("Failed!");
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
}


