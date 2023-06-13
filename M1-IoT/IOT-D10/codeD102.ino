#define DHT22_Pin 15 
#include "DHTesp.h"
DHTesp dht;
void setup() {
  Serial.begin(115200);
  Serial.println();
  dht.setup(DHT22_Pin, DHTesp::DHT22); 
}
void loop() {
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  Serial.println("B6332235 ธนพล กาศักดิ์");
  Serial.print("Temperature:");
  Serial.print(temperature, 1);
  Serial.print("C /");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("F. ");
  Serial.print("Humidity:");
  Serial.print(humidity, 1);
  Serial.println("%");
  delay(2000);
}