#include "Arduino.h"
#include "WiFi.h"
#include "secrets.h"
#include "PubSubClient.h"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char* wifi_ssid = WIFI_SSID;
const char* wifi_password = WIFI_PASSWORD;

const char* mqttServer = "turin.iotserver.uz";
const int mqttPort = 2890;
const char* mqttUser = MQTT_USER;
const char* mqttPassword = MQTT_PASSWORD;

const char* pairTopic = "ttpu/poly/led";    

const int buttonPin = 25;

int ledState = 0; 
bool lastBtnState = LOW;

void wifi_reconnect();
void mqtt_reconnect();
void sendNextState();

void setup()
{
    Serial.begin(115200);

    pinMode(buttonPin, INPUT_PULLDOWN);

    wifi_reconnect();

    mqttClient.setServer(mqttServer, mqttPort);
    mqtt_reconnect();
}

void loop()
{
    wifi_reconnect();
    mqtt_reconnect();
    mqttClient.loop();

    bool btnState = digitalRead(buttonPin);

    if (btnState != lastBtnState)
    {
        if (btnState == HIGH)
        {
            sendNextState();
        }
        lastBtnState = btnState;
        delay(50);   // debounce
    }
}

void sendNextState()
{
    if (ledState == 0) {
        mqttClient.publish(pairTopic, "redon");
    } else if (ledState == 1) {
        mqttClient.publish(pairTopic, "redoff");
        mqttClient.publish(pairTopic, "greenon");
    } else if (ledState == 2) {
        mqttClient.publish(pairTopic, "greenoff");
        mqttClient.publish(pairTopic, "blueon");
    } else if (ledState == 3) {
        mqttClient.publish(pairTopic, "blueoff");
        mqttClient.publish(pairTopic, "yellowon");
    } else if (ledState == 4) {
        mqttClient.publish(pairTopic, "yellowoff");
    }

    ledState = (ledState + 1) % 5;
    Serial.print("ledState=");
    Serial.println(ledState);
}

void wifi_reconnect()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }

    Serial.println("Wifi disconnected. Attempting to connect");

    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(".");
        delay(1000);
    }

    Serial.println(WiFi.localIP());
}

void mqtt_reconnect()
{
    while (!mqttClient.connected())
    {
        String clientId = "esp_client_" + WiFi.macAddress();

        Serial.println("Connecting to MQTT broker...");

        if (mqttClient.connect(clientId.c_str(), mqttUser, mqttPassword))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.println(mqttClient.state());
            delay(500);
        }
    }
}
