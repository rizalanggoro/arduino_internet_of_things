#include <Arduino.h>

#include "config.hpp"
#include "iot_wifi.hpp"
#include "mqtt.hpp"
#include "parser.hpp"

IotWifi iotWifi;
Mqtt mqtt;
Parser parser;

WS2812FX fx(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// todo: prototype functions
void callback(char *topic, byte *payload, unsigned int length);

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);

  // todo: open wifi connection
  iotWifi.connect();
  mqtt.open(callback);
  parser.begin(&mqtt, &fx);

  // todo: initialize led
  fx.init();
  fx.setBrightness(100);
  fx.setSpeed(1000);
  fx.setMode(FX_MODE_STATIC);
  fx.start();
}

void loop() {
  iotWifi.stream();

  if (iotWifi.isConnected()) {
    mqtt.stream();
  }

  fx.service();
}

void callback(char *topic, byte *payload, unsigned int len) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String data = "";
  for (unsigned int a = 0; a < len; a++) data += (char)payload[a];

  Serial.print("Receive data -> ");
  Serial.println(data);

  parser.parse(String(topic), data);
}