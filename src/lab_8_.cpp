#include "Arduino.h"
#include "WiFi.h"
#include "secrets.h"
#include "PubSubClient.h"

WiFiClient apachy;
PubSubClient mqttClient(apachy);

const char* wifi_name = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* mqttServer = "turin.iotserver.uz";
const int mqttPort = 2890;
const char* mqttUser = MQTT_USER;
const char* mqttPassword = MQTT_PASSWORD;

const char* mqttTopic   = "ttpu/led";
const char* myName      = "mahmudjon";

const char* timeTopic   = "time/now";              // Task-3
const char* buttonTopic = "ttpu/mahmudjon/button";   // Task-4
const char* ledTopic    = "ttpu/mahmudjon/led";      // Task-5

// Pinlar
const int buttonPin    = 25;
const int redLedPin    = 26;
const int greenLedPin  = 27;
const int blueLedPin   = 14;
const int yellowLedPin = 12;

bool lastBtnState = LOW;

void wifi_reconnect();
void mqtt_reconnect();
void mqtt_callback(char* topic, byte* payload, unsigned int length);

void setup()
{
    Serial.begin(115200);

    pinMode(buttonPin, INPUT_PULLDOWN);
    pinMode(redLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
    pinMode(blueLedPin, OUTPUT);
    pinMode(yellowLedPin, OUTPUT);

    wifi_reconnect();

    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.setCallback(mqtt_callback);   
    mqtt_reconnect();

    String message = "Hello from ";
    message += myName;
    mqttClient.publish(mqttTopic, message.c_str());
}

void loop()
{
    wifi_reconnect();
    mqtt_reconnect();
    mqttClient.loop();

    bool btnState = digitalRead(buttonPin);

    if (btnState != lastBtnState) {
        if (btnState == HIGH) {
            mqttClient.publish(buttonTopic, "pressed");
            Serial.println("Tugma: pressed");
        } else {
            mqttClient.publish(buttonTopic, "released");
            Serial.println("Tugma: released");
        }
        lastBtnState = btnState;
        delay(50); 
    }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length)
{
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    if (strcmp(topic, timeTopic) == 0) {
        Serial.print("Hozirgi vaqt: ");
        Serial.println(message);
    }

    // Task-5: LED buyruqlari
    else if (strcmp(topic, ledTopic) == 0) {
        Serial.print("LED buyrug'i: ");
        Serial.println(message);

        if      (message == "redon")     digitalWrite(redLedPin, HIGH);
        else if (message == "redoff")    digitalWrite(redLedPin, LOW);
        else if (message == "greenon")   digitalWrite(greenLedPin, HIGH);
        else if (message == "greenoff")  digitalWrite(greenLedPin, LOW);
        else if (message == "blueon")    digitalWrite(blueLedPin, HIGH);
        else if (message == "blueoff")   digitalWrite(blueLedPin, LOW);
        else if (message == "yellowon")  digitalWrite(yellowLedPin, HIGH);
        else if (message == "yellowoff") digitalWrite(yellowLedPin, LOW);
        else Serial.println("Noma'lum buyruq");
    }
}

void wifi_reconnect()
{
    if (WiFi.status() == WL_CONNECTED) {
        return;
    }

    Serial.println("Apachy bilan ulan");
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_name, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("hello");
    }

    Serial.println(WiFi.localIP());
}

void mqtt_reconnect()
{
    while (!mqttClient.connected()) {
        String clientId = "esp32_client_";
        clientId += WiFi.macAddress();

        Serial.print("MQTT'ga ulanmoqda... ");
        if (mqttClient.connect(clientId.c_str(), mqttUser, mqttPassword)) {
            Serial.println("ulandi");

            // Ulangandan keyin topiclarni tinglash
            mqttClient.subscribe(timeTopic); 
            mqttClient.subscribe(ledTopic);
        } else {
            Serial.print("xato, rc=");
            Serial.println(mqttClient.state());
            delay(500);
        }
    }
}