< Test Code >
#include <WiFi.h>
const char* ssid = "iPhoneOhm";
const char* password = "2444666668888888";

int pinTest = 2;
int pinTest2 = 19;
WiFiServer server(80);
void setup() {
  Serial.begin(115200);
  pinMode(pinTest, OUTPUT);
  pinMode(pinTest2, OUTPUT);  // set the LED pin mode
  delay(10);
  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
int value = 0;
bool LED1_Status = LOW;
bool LED2_Status = LOW;
void loop() {
  digitalWrite(pinTest, LED1_Status);
  digitalWrite(pinTest2, LED2_Status);
  WiFiClient client = server.available();  // listen for incoming clients
  if (client) {                            // if you get a client,
    Serial.println("New Client.");         // print a message out the serial port
    String currentLine = "";               // make a String to hold incoming data from the client
    while (client.connected()) {           // loop while the client's connected
      if (client.available()) {            // if there's bytes to read from the client,
        char c = client.read();            // read a byte, then
        Serial.write(c);                   // print it out the serial monitor
        if (c == '\n') {                   // if the byte is a newline character
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<html>");
            client.println("<body>");
            client.println("<h1>LED Status</h1>");
            //client.println("<h1>LED2 Status</h1>");
            client.println("<p>");
            if (LED1_Status == HIGH) {
              client.println("LED1-On");
            } else {
              client.println("LED1-Off");
            }
            if (LED2_Status == HIGH) {
              client.println("LED2-On");
            } else {
              client.println("LED2-Off");
            }
            client.println("<p>");
            //client.println("<a href=\"/ledon\"><button>LED On</button></a>");
            client.println("<a href=\"/LED1-On\"><button style = \"background-color: #f44336;\">LED1On</button></a>");
            client.println("<a href=\"/LED2-On\"><button style = \"background-color: #f44336;\">LED2On</button></a>");
            client.println("</p>");
            //client.println("<a href=\"/ledoff\"><button>LED Off</button></a>");
            client.println("<a href=\"/LED1-Off\"><button style = \"background-color: #008CBA;\">LED1Off</button></a>");
            client.println("<a href=\"/LED2-Off\"><button style = \"background-color: #008CBA;\">LED2Off</button></a>");
            client.println("<body>");
            client.println("<html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /LED1-On")) LED1_Status = HIGH;
        if (currentLine.endsWith("GET /LED2-On")) LED2_Status = HIGH;
        if (currentLine.endsWith("GET /LED1-Off")) LED1_Status = LOW;
        if (currentLine.endsWith("GET /LED2-Off")) LED2_Status = LOW;
      }
    }
    client.stop();  // close the connection:
    Serial.println("Client Disconnected.");
  }
}


