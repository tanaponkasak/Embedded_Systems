#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHTesp.h"

#define DHT22_Pin 15
#define sw1 4
#define sw2 21

char auth[] = "gOVELUAYorH-U-5YCExxEemXYjjhcl1S";
const char* ssid = "iPhoneOhm";
const char* password = "2444666668888888";
DHTesp dht;

WidgetLED LED1(V2);
WidgetLED LED2(V3);
BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  dht.setup(DHT22_Pin, DHTesp::DHT22); // Connect DHT sensor to GPIO 15
  pinMode(sw1, INPUT_PULLDOWN);
  pinMode(sw2, INPUT_PULLDOWN);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void myTimerEvent() {
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  if (digitalRead(sw1)) LED1.on();
  else LED1.off();
  if (digitalRead(sw2)) LED2.on();
  else LED2.off();
  Serial.print(" Temp('C) >> "); Serial.print(temperature, 1);
  Serial.print(", Humidity(%) >> "); Serial.println(humidity, 1);
}

void loop()
{ Blynk.run();
  timer.run(); // running timer every 250ms
}
