#include "Arduino.h"

#include "WiFi.h"
#include "secrets.h"

#include <HTTPClient.h>

const int redLedPin = 26;
const int greenLedPin = 27;
const int yellowLedPin = 12;
const int blueLedPin = 14;

HTTPClient http;

void setup() {


    Serial.begin(115200);

    pinMode(redLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
    pinMode(yellowLedPin, OUTPUT);
    pinMode(blueLedPin, OUTPUT);

    //   wifi ga ulanish va ip address olish
    WiFi.mode(WIFI_STA);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // send request to spm.iotserver.uz/esp32
    http.begin("http://spm.iotserver.uz/esp32");
    
    Serial.println("Sending request to spm.iotserver.uz/esp32");
    int httpCode = http.GET();
    Serial.print("HTTP code: ");
    Serial.println(httpCode);

}

void loop() {
    http.begin("http://spm.iotserver.uz/led");

    http.GET();

    String payload = http.getString(); 
    
    Serial.println("Sending request to spm.iotserver.uz/led");
    
    if (payload == "redon") {
        digitalWrite(redLedPin, HIGH);
        Serial.println("Red LED ON");
    } else if (payload == "redoff") {
        digitalWrite(redLedPin, LOW);
        Serial.println("Red LED OFF");
    } else if (payload == "yellowon") {
        digitalWrite(yellowLedPin, HIGH);
        Serial.println("Yellow LED ON");
    } else if (payload == "yellowoff") {
        digitalWrite(yellowLedPin, LOW);
        Serial.println("Yellow LED OFF");
    } else if (payload == "greenon") {
        digitalWrite(greenLedPin, HIGH);
        Serial.println("Green LED ON");
    } else if (payload == "greenoff") {
        digitalWrite(greenLedPin, LOW);
        Serial.println("Green LED OFF");
    } else if (payload == "blueon") {
        digitalWrite(blueLedPin, HIGH);
        Serial.println("Blue LED ON");
    } else if (payload == "blueoff") {
        digitalWrite(blueLedPin, LOW);
        Serial.println("Blue LED OFF");
    }

  
}