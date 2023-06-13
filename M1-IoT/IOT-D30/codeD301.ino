#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char auth[] = "gOVELUAYorH-U-5YCExxEemXYjjhcl1S";
const char* ssid = "iPhoneOhm";
const char* password = "2444666668888888";
void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
