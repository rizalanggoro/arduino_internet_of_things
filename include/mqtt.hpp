#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "config.hpp"

class Mqtt {
 private:
  WiFiClient espClient;
  PubSubClient client{espClient};

  unsigned long notifyOnlineDelay = 3000;
  unsigned long notifyOnlinePreviousMillis = 0;

  void initializeTopics() {
    String prefix = String(COMPANY) + "-" + String(DEVICE_ID);

    this->topicWill = prefix + "/status";
    this->topicMode = prefix + "/mode";
    this->topicBrightness = prefix + "/brightness";
    this->topicSpeed = prefix + "/speed";
    this->topicColor = prefix + "/color";
    this->topicAuto = prefix + "/auto";
    this->topicAutoDelay = prefix + "/auto_delay";
    this->topicAutoValues = prefix + "/auto_values";
    this->topicSave = prefix + "/save";
  }

  void notifyOnline() {
    unsigned long currentMillis = millis();
    if (currentMillis - this->notifyOnlinePreviousMillis >=
        this->notifyOnlineDelay) {
      this->notifyOnlinePreviousMillis = currentMillis;
      this->client.publish(this->topicWill.c_str(), "on", true);
    }
  }

  void reconnect() {
    while (!this->client.connected()) {
#ifdef DEBUG
      Serial.println("Attempting MQTT connection...");
#endif

      String clientId = String(COMPANY) + "-" + String(DEVICE_ID) + "-" +
                        String(random(0xffff), HEX);

#ifdef DEBUG
      Serial.print("Client id -> ");
      Serial.println(clientId);
#endif

      if (this->client.connect(clientId.c_str(), this->topicWill.c_str(), 2,
                               true, "off")) {
#ifdef DEBUG
        Serial.println("connected");
#endif

        client.subscribe(topicMode.c_str());
        client.subscribe(topicBrightness.c_str());
        client.subscribe(topicSpeed.c_str());
        client.subscribe(topicColor.c_str());
        client.subscribe(topicAuto.c_str());
        client.subscribe(topicAutoDelay.c_str());
        client.subscribe(topicAutoValues.c_str());
        client.subscribe(topicSave.c_str());
      } else {
#ifdef DEBUG
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
#endif

        delay(5000);
      }
    }
  }

 public:
  String topicWill;
  String topicMode;
  String topicBrightness;
  String topicSpeed;
  String topicColor;
  String topicAuto;
  String topicAutoDelay;
  String topicAutoValues;
  String topicSave;

  void open(std::function<void(char *topic, byte *payload, unsigned int length)>
                callback) {
    this->client.setServer(BROKER_HOST, BROKER_PORT);
    this->client.setCallback(callback);

    this->initializeTopics();
  }

  void stream() {
    if (!this->client.connected()) {
      digitalWrite(LED_BUILTIN, HIGH);
      this->reconnect();
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      this->notifyOnline();
    }

    this->client.loop();
  }
};