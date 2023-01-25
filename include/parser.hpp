#pragma once

#include <Arduino.h>
#include <WS2812FX.h>

#include "config.hpp"
#include "mqtt.hpp"

class Parser {
 private:
  Mqtt *mqtt;
  WS2812FX *fx;

 public:
  void begin(Mqtt *mqtt, WS2812FX *fx) {
    this->mqtt = mqtt;
    this->fx = fx;
  }

  void parse(String topic, String data) {
    // todo: mode
    if (topic == this->mqtt->topicMode) {
      int mode = data.toInt();
      if (this->fx->getMode() != mode) this->fx->setMode(mode);
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