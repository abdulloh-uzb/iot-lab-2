#include "Arduino.h"

#define RED_LED_PIN 26
#define GREEN_LED_PIN 27
#define YELLOW_LED_PIN 12
#define BLUE_LED_PIN 14

char* names[6] = {"RED", "GREEN", "YELLOW", "BLUE", "YELLOW", "GREEN"};
int leds_order[6] = { RED_LED_PIN, GREEN_LED_PIN, YELLOW_LED_PIN, BLUE_LED_PIN, YELLOW_LED_PIN, GREEN_LED_PIN};
int step = 0;


/****************************************************/
void setup(void)
{
    Serial.begin(115200);

    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);
}

/****************************************************/
void loop(void)
{

    for (int i = 0; i < 4; i++)
    {
        digitalWrite(leds_order[i], LOW);
    }

    digitalWrite(leds_order[step], HIGH);
    
    Serial.print("chase=");
    Serial.println(names[step]);

    Serial.print("step=");
    Serial.println(step);

    step = (step + 1) % 6;

    delay(150);

}
