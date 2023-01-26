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

    this->streamLiveConnection();
  }

  bool isConnected() { return this->connected; }

 private:
  unsigned long previousMillis = 0;
  unsigned long previousMillisLive = 0;
  unsigned long delay = 500;
  bool connected = false;

  void streamLiveConnection() {
#ifdef DEBUG
    unsigned long currentMillis = millis();
    if (currentMillis - this->previousMillisLive >= 2000) {
      this->previousMillisLive = currentMillis;

      Serial.print("Live wifi connection -> ");
      Serial.println(WiFi.status() == WL_CONNECTED ? "ok" : "err");
    }
#endif
  }
};