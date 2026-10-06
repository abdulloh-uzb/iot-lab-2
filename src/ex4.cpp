#include "Arduino.h"

#define RED_LED_PIN 26
#define GREEN_LED_PIN 27
#define YELLOW_LED_PIN 12
#define BLUE_LED_PIN 14

#define BUTTON_PIN 25

int leds_order[4] = {RED_LED_PIN, GREEN_LED_PIN, YELLOW_LED_PIN, BLUE_LED_PIN};
int count = 0;
int last_button_state = LOW;


/****************************************************/
void setup(void)
{
    Serial.begin(115200);

    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);

    pinMode(BUTTON_PIN, INPUT);
}

/****************************************************/
void loop(void)
{
    int button_state = digitalRead(BUTTON_PIN);

    if (button_state == HIGH && last_button_state == LOW)
    {
        count = (count + 1) % 5;

        Serial.print("count=");
        Serial.println(count);

        for (int i = 0; i < 4; i++)
        {
            if (i < count)
            {
                digitalWrite(leds_order[i], HIGH);
            }
            else
            {
                digitalWrite(leds_order[i], LOW);
            }
        }
    }

    last_button_state = button_state;

    delay(20);
}
