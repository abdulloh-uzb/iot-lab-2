#include <Arduino.h>

const int redLedPin = 26;
const int greenLedPin = 27;
const int yellowLedPin = 12;
const int blueLedPin = 14;

const char* ledNames[6] = {"RED", "GREEN", "YELLOW", "BLUE", "YELLOW", "GREEN"};
int chaseOrder[6] = {redLedPin, greenLedPin, yellowLedPin, blueLedPin, yellowLedPin, greenLedPin};
int step = 0;


/****************************************************/
void setup() {
  Serial.begin(115200);

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
}

/****************************************************/
void loop() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(chaseOrder[i], LOW);
  }

  digitalWrite(chaseOrder[step], HIGH);

  Serial.print("chase=");
  Serial.println(ledNames[step]);

  Serial.print("step=");
  Serial.println(step);

  step = (step + 1) % 6;

  delay(150);
}
