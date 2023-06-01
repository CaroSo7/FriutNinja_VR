#include <Wire.h>
#include <ArduinoJson.h> // Agregamos la librería ArduinoJson para trabajar con JSON
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;
void setup() {
  SerialBT.begin("ESepecito"); //Bluetooth device name
  Serial.begin(115200);
  Wire.setPins(21, 22);
  Wire.setClock(100000);
  Wire.begin();

  Wire.beginTransmission(0x1D);
  Wire.write(0x2D);
  Wire.write(0x08);
  Wire.endTransmission();
}

void loop() {
  float valorEnX = valorEn_x();
  float valorEnY = valorEn_y();
  float valorEnZ = valorEn_z();

  // Crear un objeto JSON
  StaticJsonDocument<100> jsonDocument;
  jsonDocument["ax"] = valorEnX;
  jsonDocument["ay"] = valorEnY;
  jsonDocument["az"] = valorEnZ;

  // Convertir el objeto JSON a una cadena
  String jsonString;
  serializeJson(jsonDocument, jsonString);

  // Imprimir la cadena JSON por Bluetooth
  Serial.println(jsonString);
  SerialBT.println(jsonString);

  delay(80);
}

int valorEn_x() {
  short x;
  Wire.beginTransmission(0x1D);
  Wire.write(0x32);
  Wire.endTransmission();

  Wire.requestFrom(0x1D, 2);
  while (!Wire.available()) {}
  short a = Wire.read();
  while (!Wire.available()) {}
  short b = Wire.read();

  x = (b << 8) | a;
  return x;
}

int valorEn_y() {
  short y;
  Wire.beginTransmission(0x1D);
  Wire.write(0x34);
  Wire.endTransmission();

  Wire.requestFrom(0x1D, 2);
  while (!Wire.available()) {}
  short a = Wire.read();
  while (!Wire.available()) {}
  short b = Wire.read();

  y = (b << 8) | a;
  return y;
}

int valorEn_z() {
  short z;
  Wire.beginTransmission(0x1D);
  Wire.write(0x36);
  Wire.endTransmission();

  Wire.requestFrom(0x1D, 2);
  while (!Wire.available()) {}
  short a = Wire.read();
  while (!Wire.available()) {}
  short b = Wire.read();

  z = (b << 8) | a;
  return z;
}
