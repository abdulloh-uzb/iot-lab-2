#include <Arduino.h>

const int lightSensorPin = 33;
const int alertOnLevel = 3000;
const int alertOffLevel = 2500;

bool alertActive = false;


/****************************************************/
void setup() {
  Serial.begin(115200);

  pinMode(lightSensorPin, INPUT);
}

/****************************************************/
void loop() {
  int value = analogRead(lightSensorPin);

  if (!alertActive && value > alertOnLevel) {
    alertActive = true;
    Serial.println("ALERT=1");
  } else if (alertActive && value < alertOffLevel) {
    alertActive = false;
    Serial.println("ALERT=0");
  }

  delay(300);
}
