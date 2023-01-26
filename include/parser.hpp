#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WS2812FX.h>

#include "config.hpp"
#include "mqtt.hpp"

class Parser {
 private:
  Mqtt *mqtt;
  WS2812FX *fx;

  int singleMode = 0;

  DynamicJsonDocument *docAutoValues;
  String autoValuesStr = "";
  unsigned long autoDelay = 1024;
  unsigned long autoPreviousMillis = 0;
  int isAutoEnable = 0;
  int autoValueIndex = 0;

  void enableAuto() {
#ifdef DEBUG
    Serial.println("enabled auto called");
#endif

    this->autoPreviousMillis = 0;
    this->autoValueIndex = 0;
    this->isAutoEnable = 1;
  }

  void disableAuto() {
#ifdef DEBUG
    Serial.println("disable auto called");
#endif

    this->fx->setMode(this->singleMode);
    this->isAutoEnable = 0;
  }

 public:
  void begin(Mqtt *mqtt, WS2812FX *fx, DynamicJsonDocument *docAutoValues) {
    this->mqtt = mqtt;
    this->fx = fx;
    this->docAutoValues = docAutoValues;
  }

  void stream() {
    if (this->isAutoEnable == 1) {
      unsigned long currentMillis = millis();
      if (currentMillis - this->autoPreviousMillis >= this->autoDelay) {
        this->autoPreviousMillis = currentMillis;

        if ((*this->docAutoValues).containsKey("n")) {
          int count = (*this->docAutoValues)["n"].as<int>();

          if (count > 0) {
            // todo: auto mode -> custom from user
            int currentAutoMode =
                (*this->docAutoValues)["v"][autoValueIndex].as<int>();

#ifdef DEBUG
            Serial.print("current auto mode -> ");
            Serial.println(currentAutoMode);
#endif

            this->fx->setMode(currentAutoMode);

            if (this->autoValueIndex < (count - 1))
              this->autoValueIndex++;
            else
              this->autoValueIndex = 0;
          } else {
// todo: auto mode from 0 to 55
#ifdef DEBUG
            Serial.print("current auto mode -> ");
            Serial.println(this->autoValueIndex);
#endif

            this->fx->setMode(this->autoValueIndex);

            if (this->autoValueIndex < 55)
              this->autoValueIndex++;
            else
              this->autoValueIndex = 0;
          }
        } else {
#ifdef DEBUG
          Serial.println("json doc -> null");
#endif
        }
      }
    }
  }

  void parse(String topic, String data) {
    // todo: auto mode
    if (topic == this->mqtt->topicAuto) {
      int enable = data.toInt();
      if (this->isAutoEnable != enable) {
        this->isAutoEnable = enable;
        if (this->isAutoEnable)
          this->enableAuto();
        else
          this->disableAuto();
      }
    }

    // todo: auto values
    if (topic == this->mqtt->topicAutoValues) {
      if (this->autoValuesStr != data) {
        this->autoValuesStr = data;
        deserializeJson(*this->docAutoValues, this->autoValuesStr);

        if (this->isAutoEnable == 1) this->enableAuto();
      }
    }

    // todo: auto delay
    if (topic == this->mqtt->topicAutoDelay) {
      unsigned long delay = atol(data.c_str());
      if (this->autoDelay != delay) this->autoDelay = delay;
    }

    // todo: mode
    if (topic == this->mqtt->topicMode) {
      int mode = data.toInt();
      if (this->fx->getMode() != mode) {
        this->fx->setMode(mode);
        this->singleMode = mode;
      }
    }

    // todo: brightness
    if (topic == this->mqtt->topicBrightness) {
      int brightness = data.toInt();
      if (this->fx->getBrightness() != brightness)
        this->fx->setBrightness(brightness);
    }

    // todo: speed
    if (topic == this->mqtt->topicSpeed) {
      int speed = data.toInt();
      if (this->fx->getSpeed() != speed) this->fx->setSpeed(speed);
    }

    // todo: color
    if (topic == this->mqtt->topicColor) {
      // todo: sample data -> 0.0.0
      int a0 = data.indexOf(".");
      int a1 = data.lastIndexOf(".");
      int a2 = data.length();

      String rStr = data.substring(0, a0);
      String gStr = data.substring(a0 + 1, a1);
      String bStr = data.substring(a1 + 1, a2);

      this->fx->setColor(rStr.toInt(), gStr.toInt(), bStr.toInt());
    }
  }
};