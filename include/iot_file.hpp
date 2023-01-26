// #pragma once

// #include <Arduino.h>
// #include <ArduinoJson.h>
// #include <LittleFS.h>

// #include "config.hpp"

// #define DEVICE_MODE_PATH "/mode.txt"
// #define DEVICE_BRIGHTNESS_PATH "/brightness.txt"
// #define DEVICE_SPEED_PATH "/speed.txt"
// #define DEVICE_COLOR_PATH "/color.txt"
// #define DEVICE_AUTO_ENABLE_PATH "/a_enable.txt"
// #define DEVICE_AUTO_DELAY_PATH "/a_delay.txt"
// #define DEVICE_AUTO_VALUES_PATH "/a_values.txt"

// class IotFile {
//  private:
//   int *deviceMode;
//   int *deviceBrightness;
//   int *deviceSpeed;
//   int *deviceColor;
//   int *deviceAutoEnable;
//   int *deviceAutoDelay;
//   DynamicJsonDocument *docAutoValues;
//   String *deviceAutoValuesStr;

//   void writeInt(char *path, int data) {
//     File file = LittleFS.open(path, "w");
//     if (file) {
//       file.write(data);
//       file.close();

// #ifdef DEBUG
//       Serial.print("Write: ");
//       Serial.print(path);
//       Serial.println(" -> successfully!");
// #endif
//     }
//   }

//   void writeStr(char *path, String data) {
//     File file = LittleFS.open(path, "w");
//     if (file) {
//       file.write(data.c_str());
//       file.close();

// #ifdef DEBUG
//       Serial.print("Write: ");
//       Serial.print(path);
//       Serial.println(" -> successfully!");
// #endif
//     }
//   }

//   int readInt(char *path) {
//     File file = LittleFS.open(path, "r");
//     if (file) {
//       int data = file.read();
//       file.close();
//       return data;
//     }
//     return -2;
//   }

//   String readStr(char *path) {
//     File file = LittleFS.open(path, "r");
//     if (file) {
//       String data = file.readString();
//       file.close();
//       return data;
//     }
//     return "";
//   }

//  public:
//   void setVars(int *deviceMode, int *deviceBrightness, int *deviceSpeed,
//                int *deviceColor, int *deviceAutoEnable, int *deviceAutoDelay,
//                DynamicJsonDocument *docAutoValues,
//                String *deviceAutoValuesStr) {
//     this->deviceMode = deviceMode;
//     this->deviceBrightness = deviceBrightness;
//     this->deviceSpeed = deviceSpeed;
//     this->deviceColor = deviceColor;
//     this->deviceAutoEnable = deviceAutoEnable;
//     this->deviceAutoDelay = deviceAutoDelay;
//     this->docAutoValues = docAutoValues;
//     this->deviceAutoValuesStr = deviceAutoValuesStr;
//   }

//   void begin() {
//     if (LittleFS.begin()) {
// #ifdef DEBUG
//       Serial.println("Little FS begin successfully!");
// #endif
//     }
//   }

//   void saveConfig() {
// #ifdef DEBUG
//     Serial.println("Saving device config...");
// #endif

//     this->writeInt(DEVICE_MODE_PATH, *this->deviceMode);
//     this->writeInt(DEVICE_BRIGHTNESS_PATH, *this->deviceBrightness);
//     this->writeInt(DEVICE_SPEED_PATH, *this->deviceSpeed);
//     this->writeInt(DEVICE_COLOR_PATH, *this->deviceColor);
//     this->writeInt(DEVICE_AUTO_ENABLE_PATH, *this->deviceAutoEnable);
//     this->writeInt(DEVICE_AUTO_DELAY_PATH, *this->deviceAutoDelay);
//     this->writeStr(DEVICE_AUTO_VALUES_PATH, *this->deviceAutoValuesStr);
//   }

//   void loadConfig() {
// #ifdef DEBUG
//     Serial.println("Loading device config...");
// #endif

//     int rMode = this->readInt(DEVICE_MODE_PATH);
//     if (rMode != -2) *this->deviceMode = rMode;

//     int rBrightness = this->readInt(DEVICE_BRIGHTNESS_PATH);
//     if (rBrightness != -1) *this->deviceBrightness = rBrightness;

//     int rSpeed = this->readInt(DEVICE_SPEED_PATH);
//     if (rSpeed != -2) *this->deviceSpeed = rSpeed;

//     int rColor = this->readInt(DEVICE_COLOR_PATH);
//     if (rColor != -2) *this->deviceColor = rColor;

//     int rAutoEnable = this->readInt(DEVICE_AUTO_ENABLE_PATH);
//     if (rAutoEnable != -2) *this->deviceAutoEnable = rAutoEnable;

//     int rAutoDelay = this->readInt(DEVICE_AUTO_DELAY_PATH);
//     if (rAutoDelay != -2) *this->deviceAutoDelay = rAutoDelay;

//     String rAutoValues = this->readStr(DEVICE_AUTO_VALUES_PATH);
//     if (rAutoValues.length() > 0) {
//       *this->deviceAutoValuesStr = rAutoValues;
//       deserializeJson(*this->docAutoValues, *this->deviceAutoValuesStr);
//     }
//   }
// };