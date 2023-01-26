#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WS2812FX.h>

#include "config.hpp"
// #include "iot_file.hpp"
#include "mqtt.hpp"

class Parser {
 private:
  Mqtt *mqtt;
  WS2812FX *fx;
  // IotFile *iotFile;

  int *deviceMode;
  int *deviceBrightness;
  int *deviceSpeed;
  int *deviceColor;
  int *deviceAutoEnable;
  int *deviceAutoDelay;
  DynamicJsonDocument *docAutoValues;
  String *deviceAutoValuesStr;

  unsigned long autoPreviousMillis = 0;
  int autoValueIndex = 0;

  void enableAuto() {
#ifdef DEBUG
    Serial.println("enabled auto called");
#endif

    this->autoPreviousMillis = 0;
    this->autoValueIndex = 0;
    *this->deviceAutoEnable = 1;
  }

  void disableAuto() {
#ifdef DEBUG
    Serial.println("disable auto called");
#endif

    this->fx->setMode(*this->deviceMode);
    *this->deviceAutoEnable = 0;
  }

 public:
  void setConfig(int *deviceMode, int *deviceBrightness, int *deviceSpeed,
                 int *deviceColor, int *deviceAutoEnable, int *deviceAutoDelay,
                 DynamicJsonDocument *docAutoValues,
                 String *deviceAutoValuesStr) {
    this->deviceMode = deviceMode;
    this->deviceBrightness = deviceBrightness;
    this->deviceSpeed = deviceSpeed;
    this->deviceColor = deviceColor;
    this->deviceAutoEnable = deviceAutoEnable;
    this->deviceAutoDelay = deviceAutoDelay;
    this->docAutoValues = docAutoValues;
    this->deviceAutoValuesStr = deviceAutoValuesStr;
  }

  void begin(Mqtt *mqtt, WS2812FX *fx) {
    this->mqtt = mqtt;
    this->fx = fx;
    // this->iotFile = iotFile;
  }

  void stream() {
    if (*this->deviceAutoEnable == 1) {
      unsigned long currentMillis = millis();
      if (currentMillis - this->autoPreviousMillis >= *this->deviceAutoDelay) {
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
    // todo: save config
    // if (topic == this->mqtt->topicSave) {
    //   this->iotFile->saveConfig();
    // }

    // todo: auto mode
    if (topic == this->mqtt->topicAuto) {
      int enable = data.toInt();
      if (*this->deviceAutoEnable != enable) {
        *this->deviceAutoEnable = enable;
        if (*this->deviceAutoEnable == 1)
          this->enableAuto();
        else
          this->disableAuto();
      }
    }

    // todo: auto values
    if (topic == this->mqtt->topicAutoValues) {
      if (*this->deviceAutoValuesStr != data) {
        *this->deviceAutoValuesStr = data;
        deserializeJson(*this->docAutoValues, *this->deviceAutoValuesStr);

        if (*this->deviceAutoEnable == 1) this->enableAuto();
      }
    }

    // todo: auto delay
    if (topic == this->mqtt->topicAutoDelay) {
      int delay = data.toInt();
      if (*this->deviceAutoDelay != delay) {
        *this->deviceAutoDelay = delay;
      }
    }

    // todo: mode
    if (topic == this->mqtt->topicMode) {
      int mode = data.toInt();
      if (*this->deviceMode != mode) {
        this->fx->setMode(mode);
        *this->deviceMode = mode;
      }
    }

    // todo: brightness
    if (topic == this->mqtt->topicBrightness) {
      int brightness = data.toInt();
      if (*this->deviceBrightness != brightness) {
        this->fx->setBrightness(brightness);
        *this->deviceBrightness = brightness;
      }
    }

    // todo: speed
    if (topic == this->mqtt->topicSpeed) {
      int speed = data.toInt();
      if (*this->deviceSpeed != speed) {
        this->fx->setSpeed(speed);
        *this->deviceSpeed = speed;
      }
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

      int color = WS2812FX::Color(rStr.toInt(), gStr.toInt(), bStr.toInt());
      if (*this->deviceColor != color) {
        this->fx->setColor(color);
        *this->deviceColor = color;
      }
    }
  }
};