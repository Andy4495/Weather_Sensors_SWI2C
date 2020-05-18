/*
Library to interface with TMP007, OPT3001, and BME280 using Software I2C.
Uses integer-only calculations and variables.
   https://github.com/Andy4495/Weather_Sensors_SWI2C.h

Version History
   1.0.0    05/17/2020  A.T.   Original
*/

#include "Weather_Sensors_SWI2C.h"

#define SDA_PIN      10
#define SCL_PIN       9

#define DELAY_TIME 5000

TMP007_SWI2C  myTMP007(SDA_PIN, SCL_PIN);
OPT3001_SWI2C myOPT3001(SDA_PIN, SCL_PIN);
BME280_SWI2C  myBME280(SDA_PIN, SCL_PIN);

void setup() {

  Serial.begin(9600);

  myTMP007.begin();
  myOPT3001.begin();
  myBME280.begin();

  // Verify device IDs
  Serial.print("TMP007 DevID: 0x");
  Serial.println(myTMP007.readDeviceID(), HEX);

  Serial.print("OPT3001 ManuID: 0x");
  Serial.println(myOPT3001.readDeviceID(), HEX);

  Serial.print("BME280 Chip ID: 0x");
  Serial.println(myBME280.readDeviceID(), HEX);

  Serial.println("--");
}


void loop() {

  myTMP007.readSensor();

  Serial.print("TMP007 Int (0.1 C): ");
  Serial.println(myTMP007.getIntTempC());
  Serial.print("TMP007 Int (0.1 F): ");
  Serial.println(myTMP007.getIntTempF());

  Serial.print("TMP007 Ext (0.1 C): ");
  Serial.println(myTMP007.getExtTempC());
  Serial.print("TMP007 Ext (0.1 F): ");
  Serial.println(myTMP007.getExtTempF());

  myOPT3001.readSensor();

  Serial.print("OTP3001 Lux: ");
  Serial.println(myOPT3001.getLux());


  myBME280.readSensor();
  Serial.print("BME280 T (0.01 C): ");
  Serial.println(myBME280.getTempC());
  Serial.print("BME280 T (0.1 F): ");
  Serial.print(myBME280.getTempF());
  Serial.print("BME280 P (Pa): ");
  Serial.println(myBME280.getPressurePa());
  Serial.print("BME280 P (inHG): ");
  Serial.print(myBME280.getPressureInHg());
  Serial.print("BME280 H (0.1%RH): ");
  Serial.println(myBME280.getRH());

  delay(DELAY_TIME);
}
