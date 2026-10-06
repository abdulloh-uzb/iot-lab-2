#include "Arduino.h"

#define LIGHT_SENSOR_PIN 33
#define SAMPLES_COUNT 10


/****************************************************/
void setup(void)
{
    Serial.begin(115200);

    pinMode(LIGHT_SENSOR_PIN, INPUT);
}

/****************************************************/
void loop(void)
{
    int min_value = 4095;
    int max_value = 0;
    long sum = 0;

    for (int i = 0; i < SAMPLES_COUNT; i++)
    {
        int value = analogRead(LIGHT_SENSOR_PIN);

        if (value < min_value)
        {
            min_value = value;
        }
        if (value > max_value)
        {
            max_value = value;
        }
        sum += value;
    }

    int avg = sum / SAMPLES_COUNT;

    Serial.print("min=");
    Serial.print(min_value);
    Serial.print(" max=");
    Serial.print(max_value);
    Serial.print(" avg=");
    Serial.println(avg);

    delay(1000);
}
