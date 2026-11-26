#include <Arduino.h>

const int redLedPin = 26;
const int greenLedPin = 27;
const int yellowLedPin = 12;
const int blueLedPin = 14;

const int buttonPin = 25;

int ledPins[4] = {redLedPin, greenLedPin, yellowLedPin, blueLedPin};
int pressCount = 0;
int lastButtonState = LOW;


/****************************************************/
void setup() {
  Serial.begin(115200);

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  pinMode(buttonPin, INPUT);
}

/****************************************************/
void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH && lastButtonState == LOW) {
    pressCount = (pressCount + 1) % 5;

    Serial.print("count=");
    Serial.println(pressCount);

    for (int i = 0; i < 4; i++) {
      if (i < pressCount) {
        digitalWrite(ledPins[i], HIGH);
      } else {
        digitalWrite(ledPins[i], LOW);
      }
    }
  }

  lastButtonState = buttonState;

  delay(20);
}
