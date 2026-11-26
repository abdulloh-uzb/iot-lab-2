#include <Arduino.h>

const int lightSensorPin = 33;


/****************************************************/
void setup() {
  Serial.begin(115200);

  pinMode(lightSensorPin, INPUT);
}

/****************************************************/
void loop() {
  int minValue = 4095;
  int maxValue = 0;
  long sum = 0;

  for (int i = 0; i < 10; i++) {
    int value = analogRead(lightSensorPin);

    if (value < minValue) {
      minValue = value;
    }
    if (value > maxValue) {
      maxValue = value;
    }
    sum += value;
  }

  int avg = sum / 10;

  Serial.print("min=");
  Serial.print(minValue);
  Serial.print(" max=");
  Serial.print(maxValue);
  Serial.print(" avg=");
  Serial.println(avg);

  delay(1000);
}
