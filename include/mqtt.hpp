#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "config.hpp"

class Mqtt {
 private:
  WiFiClient espClient;
  PubSubClient client{espClient};

  void initializeTopics() {
    String prefix = String(COMPANY) + "-" + String(DEVICE_ID);

    this->topicWill = prefix + "/status";
    this->topicMode = prefix + "/mode";
    this->topicBrightness = prefix + "/brightness";
    this->topicSpeed = prefix + "/speed";
    this->topicColor = prefix + "/color";
  }

  void reconnect() {
    while (!this->client.connected()) {
      Serial.println("Attempting MQTT connection...");

      String clientId = String(COMPANY) + "-" + String(DEVICE_ID) + "-" +
                        String(random(0xffff), HEX);

      Serial.print("Client id -> ");
      Serial.println(clientId);

      if (this->client.connect(clientId.c_str(), this->topicWill.c_str(), 2,
                               true, "off")) {
        Serial.println("connected");

        this->client.publish(this->topicWill.c_str(), "on", true);

        client.subscribe(topicMode.c_str());
        client.subscribe(topicBrightness.c_str());
        client.subscribe(topicSpeed.c_str());
        client.subscribe(topicColor.c_str());
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
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
    } else
      digitalWrite(LED_BUILTIN, LOW);

    this->client.loop();
  }
};