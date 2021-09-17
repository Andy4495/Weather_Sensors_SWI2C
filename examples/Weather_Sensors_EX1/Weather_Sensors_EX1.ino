/*
Library to interface with TMP007, OPT3001, and BME280 using Software I2C.
Uses integer-only calculations and variables.
   https://github.com/Andy4495/Weather_Sensors_SWI2C.h

Version History
   1.0.0    05/17/2020  A.T.   Original
   1.0.1    09/17/2021  A.T.   Add examples for updated API access to 
                               the SWI2C object. 
*/

#include "Weather_Sensors_SWI2C.h"

#define SDA_PIN           10
#define SCL_PIN            9
#define TMP007_ADDRESS  0x40    // Default is 0x40
#define OPT3001_ADDRESS 0x47    // Default is 0x44
#define BME280_ADDRESS  0x77    // Default is 0x76

#define DELAY_TIME 5000

TMP007_SWI2C  myTMP007(SDA_PIN, SCL_PIN, TMP007_ADDRESS);
OPT3001_SWI2C myOPT3001(SDA_PIN, SCL_PIN, OPT3001_ADDRESS);
BME280_SWI2C  myBME280(SDA_PIN, SCL_PIN, BME280_ADDRESS);

void setup() {

  Serial.begin(9600);

  myTMP007.begin();
  myOPT3001.begin();
  myBME280.begin();

  Serial.println("");
  Serial.println("Weather Sensors SWI2C Library Example 1");
  Serial.println("=======================================");

  // Verify device IDs
  Serial.print("TMP007 DeviceID: 0x");
  Serial.println(myTMP007.readDeviceID(), HEX);
  Serial.println("TMP007 Device ID should be: 0x78");
  Serial.println("Default clock stretch timout setting is 500 ms.");
  Serial.print("Clock stretch timeout setting: ");
  Serial.println(myTMP007.getSWI2CObject()->getStretchTimeout());
  Serial.println("");

  Serial.print("OPT3001 DeviceID: 0x");
  Serial.println(myOPT3001.readDeviceID(), HEX);
  Serial.println("OPT3001 Device ID should be: 0x3001");
  Serial.println("Setting clock stretch timeout to 300 ms.");
  myOPT3001.getSWI2CObject()->setStretchTimeout(300);
  Serial.print("Clock stretch timeout setting: ");
  Serial.println(myOPT3001.getSWI2CObject()->getStretchTimeout());
  Serial.println("");
  Serial.println("");

  Serial.print("BME280 Device ID: 0x");
  Serial.println(myBME280.readDeviceID(), HEX);
  Serial.println("BME280 Device ID should be: 0x60");
  Serial.println("Setting clock stretch timeout to 100 ms.");
  myBME280.getSWI2CObject()->setStretchTimeout(100);  
  Serial.print("Clock stretch timeout setting: ");
  Serial.println(myBME280.getSWI2CObject()->getStretchTimeout());
  Serial.println("");
  Serial.println("--");
}

void loop() {
  uint16_t data16A, data16B, data16C;
  uint8_t data8;

  myTMP007.readSensor();

  Serial.println("TMP007 Sensor:");
  Serial.print("  Internal Temperature (0.1 C): ");
  Serial.println(myTMP007.getIntTempC());
  Serial.print("  Internal Temperature (0.1 F): ");
  Serial.println(myTMP007.getIntTempF());

  Serial.print("  External Temperature (0.1 C): ");
  Serial.println(myTMP007.getExtTempC());
  Serial.print("  External Temperature (0.1 F): ");
  Serial.println(myTMP007.getExtTempF());

  myOPT3001.readSensor();

  Serial.println("Opt3001 Sensor: ");
  Serial.print("  Light Intensity (lux):        ");
  Serial.println(myOPT3001.getLux());

  myBME280.readSensor();
  Serial.println("BME280 Sensor: ");
  Serial.print("  Temperature (0.01 C):         ");
  Serial.println(myBME280.getTempC());
  Serial.print("  Temperature (0.1 F):          ");
  Serial.println(myBME280.getTempF());
  Serial.print("  Pressure (Pa):               ");
  Serial.println(myBME280.getPressurePa());
  Serial.print("  Pressure (inHG):              ");
  Serial.println(myBME280.getPressureInHg());
  Serial.print("  Relative Humidity (0.1%RH):   ");
  Serial.println(myBME280.getRH());
  Serial.println("---");

  
  Serial.println("Test Repeated Start");
  myTMP007.getSWI2CObject()->read2bFromRegisterMSBFirst(TMP007_INTERNAL_TEMPERATURE, &data16A, false);
  myTMP007.getSWI2CObject()->read2bFromRegisterMSBFirst(TMP007_EXTERNAL_TEMPERATURE, &data16B, false);
  myOPT3001.getSWI2CObject()->read2bFromRegisterMSBFirst(OPT3001_RESULT_REGISTER, &data16C, false);
  myBME280.getSWI2CObject()->read1bFromRegister(BME280_ID, &data8);
  Serial.print("TMP007 Internal C: ");
  Serial.println(data16A);
  Serial.print("TMP007 External C: "); 
  Serial.println(data16B);
  Serial.print("OPT3001 Lux: ");
  Serial.println(data16C);
  Serial.print("BME280 Device ID: 0x");
  Serial.println(data8, HEX);
  Serial.println("---");

  delay(DELAY_TIME);
}