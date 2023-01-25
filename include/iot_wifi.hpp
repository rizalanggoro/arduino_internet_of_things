#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "config.hpp"

class IotWifi {
 public:
  void connect() {
    WiFi.begin(WIFI_NAME, WIFI_PASS);
    Serial.print("Connecting to ");
    Serial.print(WIFI_NAME);
  }

  void stream() {
    if (!this->connected) {
      unsigned long currentMillis = millis();
      if (currentMillis - this->previousMillis >= this->delay) {
        this->previousMillis = currentMillis;
        if (WiFi.status() == WL_CONNECTED) {
          this->connected = true;
          Serial.println("Connected!");
        } else
          Serial.print(".");
      }
    }
  }

  bool isConnected() { return this->connected; }

 private:
  unsigned long previousMillis = 0;
  unsigned long delay = 500;
  bool connected = false;
};