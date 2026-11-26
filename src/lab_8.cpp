#include "Arduino.h"
#include "WiFi.h"
#include "secrets.h"
#include "PubSubClient.h"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char* mqttServer = "turin.iotserver.uz";
const int mqttPort = 2890;
const char* mqttUser = MQTT_USER;
const char* mqttPassword = MQTT_PASSWORD;

const char* mqttTopic = "ttpu/led";
const char* myName = "ABDULLOH";  

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;    

const int buttonPin = 25;

// LED pins (from diagram.json)
const int redLedPin = 26;
const int greenLedPin = 27;
const int blueLedPin = 14;
const int yellowLedPin = 12;

const char* ledTopic = "ttpu/usmonov/led";
const char* pairTopic = "ttpu/poly/led";    // pair board shu topicni tinglaydi

int pairLedState = 0;  
void sendNextState();
const char* ledTopic2 = "ttpu/usmonov/led2";

bool btnState = LOW;
bool lastBtnState = LOW;

void mqtt_subscribe(char* topic, byte* payload, unsigned int length);

void mqtt_reconnect();
void wifi_reconnect();

void setup()
{

    pinMode(buttonPin, INPUT_PULLDOWN);
    pinMode(redLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
    pinMode(blueLedPin, OUTPUT);
    pinMode(yellowLedPin, OUTPUT);
    Serial.begin(115200);

    wifi_reconnect();

    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.setCallback(mqtt_subscribe);
    mqtt_reconnect();
}

void loop()
{
    wifi_reconnect(); 

    mqtt_reconnect();

    mqttClient.loop();

    // String message = "Hello from ";
    // message += myName;

    // Serial.print("Publishing message: ");
    // Serial.println(message);

    // mqttClient.publish(mqttTopic, message.c_str());

    btnState = digitalRead(buttonPin);

    if (btnState != lastBtnState){

        if (btnState == HIGH){
            sendNextState();
        }
        lastBtnState = btnState;

        delay(50);  

    }

}

void sendNextState()
{
    if (pairLedState == 0) {
        mqttClient.publish(pairTopic, "redon");
    } else if (pairLedState == 1) {
        mqttClient.publish(pairTopic, "redoff");
        mqttClient.publish(pairTopic, "greenon");
    } else if (pairLedState == 2) {
        mqttClient.publish(pairTopic, "greenoff");
        mqttClient.publish(pairTopic, "blueon");
    } else if (pairLedState == 3) {
        mqttClient.publish(pairTopic, "blueoff");
        mqttClient.publish(pairTopic, "yellowon");
    } else if (pairLedState == 4) {
        mqttClient.publish(pairTopic, "yellowoff");
    }

    pairLedState = (pairLedState + 1) % 5;
    Serial.print("pairLedState=");
    Serial.println(pairLedState);
}

void mqtt_subscribe(char* topic, byte* payload, unsigned int length )
{

    Serial.println("Message received from topic: ");
    Serial.println(topic);

    String message;

    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.print("Message arrived:");
    Serial.println(message);

    if (strcmp(topic, ledTopic) == 0) {
        if      (message == "redon")     digitalWrite(redLedPin, HIGH);
        else if (message == "redoff")    digitalWrite(redLedPin, LOW);
        else if (message == "greenon")   digitalWrite(greenLedPin, HIGH);
        else if (message == "greenoff")  digitalWrite(greenLedPin, LOW);
        else if (message == "blueon")    digitalWrite(blueLedPin, HIGH);
        else if (message == "blueoff")   digitalWrite(blueLedPin, LOW);
        else if (message == "yellowon")  digitalWrite(yellowLedPin, HIGH);
        else if (message == "yellowoff") digitalWrite(yellowLedPin, LOW);
        else Serial.println("Unknown LED command");
    }

}

void mqtt_reconnect()
{
    while (!mqttClient.connected()) {
        String clientId = "esp32_client_";
        clientId += WiFi.macAddress();

        Serial.print("Connecting to MQTT broker... ");
        if (mqttClient.connect(clientId.c_str(), mqttUser, mqttPassword)) {
            Serial.println("connected");

            mqttClient.subscribe("time/now");
            mqttClient.subscribe(ledTopic);
        } else {
            Serial.print("failed, rc=");
            Serial.println(mqttClient.state());
            delay(500);
        }
    }
}

void wifi_reconnect()
{
    if (WiFi.status() == WL_CONNECTED) {
        return;
    }

    Serial.println("Wifi disconnected. attempting to reconnect");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);   // put your SSID and password here

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(5000);
        Serial.println(".");
    }

    Serial.println(WiFi.localIP());
}
