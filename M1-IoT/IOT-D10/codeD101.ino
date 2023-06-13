#define pushButton1 23
#define pushButton2 19
#define LEDPin1 22
#define LEDPin2 18
int buttonState = 0;
void setup() {
  Serial.begin(115200);
  pinMode(pushButton1, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
}
void loop() {
  if (digitalRead(pushButton1) == LOW) {
    delay(20);
    buttonState = 1 - buttonState;
    digitalWrite(LEDPin1, buttonState);
    while (digitalRead(pushButton1) == LOW);
    delay(20);
  }else if (digitalRead(pushButton2) == LOW) {
    delay(20);
    buttonState = 1 - buttonState;
    digitalWrite(LEDPin2, buttonState);
    while (digitalRead(pushButton2) == LOW);
    delay(20);
  }
}
