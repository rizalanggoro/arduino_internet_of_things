#include <Arduino.h>
#include <ArduinoJson.h>

#include "config.hpp"
// #include "iot_file.hpp"
#include "iot_wifi.hpp"
#include "mqtt.hpp"
#include "parser.hpp"

#ifdef FORMAT
#include <LittleFS.h>
#endif

// IotFile iotFile;
IotWifi iotWifi;
Mqtt mqtt;
Parser parser;

WS2812FX fx(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// todo: prototype functions
void callback(char *topic, byte *payload, unsigned int length);

// todo: device config
int deviceMode = FX_MODE_STATIC;
int deviceBrightness = 100;
int deviceSpeed = 2048;
int deviceColor = BLUE;
int deviceAutoEnable = 0;
int deviceAutoDelay = 10240;
DynamicJsonDocument docAutoValues(256);
String deviceAutoValuesStr = "";
// todo: end device config

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);

// todo: format fs
#ifdef FORMAT
  if (LittleFS.begin()) {
    Serial.println("Little FS begin successfully!");
    if (LittleFS.format()) {
      Serial.println("Little FS format successfully!");
    }
  }
  return;
#endif

  // todo: load saved config if exists
  // iotFile.begin();
  // iotFile.setVars(&deviceMode, &deviceBrightness, &deviceSpeed, &deviceColor,
  //                 &deviceAutoEnable, &deviceAutoDelay, &docAutoValues,
  //                 &deviceAutoValuesStr);
  // iotFile.loadConfig();

  // todo: initialize led
  fx.init();
  fx.setMode(deviceMode);
  fx.setBrightness(deviceBrightness);
  fx.setSpeed(deviceSpeed);
  fx.setColor(deviceColor);
  fx.start();

  // todo: open wifi connection
  iotWifi.connect();
  mqtt.open(callback);
  parser.begin(&mqtt, &fx);
  parser.setConfig(&deviceMode, &deviceBrightness, &deviceSpeed, &deviceColor,
                   &deviceAutoEnable, &deviceAutoDelay, &docAutoValues,
                   &deviceAutoValuesStr);
}

void loop() {
#ifdef FORMAT
  return;
#endif

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