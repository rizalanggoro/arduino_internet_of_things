#include <Arduino.h>
#include <ArduinoJson.h>

#include "config.hpp"
#include "iot_wifi.hpp"
#include "mqtt.hpp"
#include "parser.hpp"

IotWifi iotWifi;
Mqtt mqtt;
Parser parser;

WS2812FX fx(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
DynamicJsonDocument docAutoValues(256);

// todo: prototype functions
void callback(char *topic, byte *payload, unsigned int length);

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);

  // todo: open wifi connection
  iotWifi.connect();
  mqtt.open(callback);
  parser.begin(&mqtt, &fx, &docAutoValues);

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
  parser.stream();
}

void callback(char *topic, byte *payload, unsigned int len) {
#ifdef DEBUG
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
#endif

  String data = "";
  for (unsigned int a = 0; a < len; a++) data += (char)payload[a];

#ifdef DEBUG
  Serial.print("Receive data -> ");
  Serial.println(data);
#endif

  parser.parse(String(topic), data);
}