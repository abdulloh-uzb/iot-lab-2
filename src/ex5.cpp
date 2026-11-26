#include <Arduino.h>

const int buzzerPin = 32;
const int lightSensorPin = 33;
const int lightLevel = 500;

bool buzzerOn = false;


void setup() {
  Serial.begin(115200);

  pinMode(lightSensorPin, INPUT);
}

void loop() {
  int value = analogRead(lightSensorPin);
  Serial.println(value);

  if (!buzzerOn && value > lightLevel) {
    buzzerOn = true;
    tone(buzzerPin, 1000);
    Serial.println("buzzer=ON");
  } else if (buzzerOn && value < lightLevel) {
    buzzerOn = false;
    noTone(buzzerPin);
    Serial.println("buzzer=OFF");
  }

  delay(1000);
}
