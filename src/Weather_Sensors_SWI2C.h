/*
Library to interface with TMP007, OPT3001, and BME280 using Software I2C.
   https://github.com/Andy4495/Weather_Sensors_SWI2C

Version History
   1.0.0    05/17/2020  A.T.   Original
*/

#ifndef WEATHER_SENSORS_SWI2C
#define WEATHER_SENSORS_SWI2C

#include "SWI2C.h"

#define TMP007_DEFAULT_ADDRESS               0x40     // 0x40 when ADDR0 and ADDR0 pulled to GND
#define OPT3001_DEFAULT_ADDRESS              0x44     // 0x44 when ADDR pulled to GND
#define BME280_DEFAULT_ADDRESS               0x76     // 0x76 when SDO pulled to GND

#define TMP007_MASK_FOUR_SAMPLES             0x0400
#define TMP007_MASK_FOUR_SAMPLES_LOW_POWER   0x0E00
#define TMP007_MASK_TWO_SAMPLES_LOW_POWER    0x0C00
#define TMP007_MASK_POWER_UP                 0x1000   // Conversions enabled

#define TMP007_INTERNAL_TEMPERATURE     0x01
#define TMP007_CONFIGURATION            0x02
#define TMP007_EXTERNAL_TEMPERATURE     0x03
#define TMP007_DEVICE_ID                0x1F

#define TMP007_VALUE_CONFIG             (TMP007_MASK_POWER_UP | TMP007_MASK_TWO_SAMPLES_LOW_POWER)

#define OPT3001_RESULT_REGISTER         0x00
#define OPT3001_CONFIGURATION_REGISTER  0x01
#define OPT3001_LOW_LIMIT_REGISTER      0x02
#define OPT3001_HIGH_LIMIT_REGISTER     0x03
#define OPT3001_MANUFACTURE_ID_REGISTER 0x7e     // Should return 0x5449 (ASCI for "TI")
#define OPT3001_DEVICE_ID_REGISTER      0x7f     // Should return 0x3001

// Configuration:
//   - Automatic Full Scale Mode
//   - 100 ms Conversion Time
//   - Continuous Conversion Mode
//   - Latched Interrupt Reporting
//   - INT Active low
//   - Mask Exponent Disabled
//   - Fault Count - One Fault
#define OPT3001_STARTUP_CONFIG          0xC410

// BME280 Registers
#define BME280_DATA_F7_FE                    0xF7
#define BME280_CONTROL_TEMPERATURE_PRESSURE  0xF4
#define BME280_CONTROL_HUMIDITY              0xF2
#define BME280_STATUS                        0xF3
#define BME280_CONFIGURATION                 0xF5
#define BME280_RESET                         0xE0
#define BME280_ID                            0xD0
#define BME280_CALIBRATION_T1                0x88
#define BME280_CALIBRATION_T2                0x8A
#define BME280_CALIBRATION_T3                0x8C
#define BME280_CALIBRATION_P1                0x8E
#define BME280_CALIBRATION_P2                0x90
#define BME280_CALIBRATION_P3                0x92
#define BME280_CALIBRATION_P4                0x94
#define BME280_CALIBRATION_P5                0x96
#define BME280_CALIBRATION_P6                0x98
#define BME280_CALIBRATION_P7                0x9A
#define BME280_CALIBRATION_P8                0x9C
#define BME280_CALIBRATION_P9                0x9E
#define BME280_CALIBRATION_H1                0xA1
#define BME280_CALIBRATION_H2                0xE1
#define BME280_CALIBRATION_H3                0xE3
#define BME280_CALIBRATION_H4                0xE4
#define BME280_CALIBRATION_H5                0xE5
#define BME280_CALIBRATION_H5B2              0xE6
#define BME280_CALIBRATION_H6                0xE7

// BME280 Configuration values
#define BME280_VALUE_RESET_COMMAND           0xB6 // Forces power-on reset procedure
#define BME280_VALUE_CONFIG                  0xA0 // 1000ms standby[7..5], IIR filter off [4..2], SPI disabled [0]
#define BME280_VALUE_TP_CONFIG               0x27 // 1x sampling for P, 1x sampling for T, normal mode
#define BME280_VALUE_HUM_CONFIG              0x01 // 1x sampling for Humidity

class TMP007_SWI2C {
public:
  TMP007_SWI2C(byte sda_pin, byte scl_pin, byte address = TMP007_DEFAULT_ADDRESS);
  ~TMP007_SWI2C();
  void     begin();
  void     readSensor();
  int      getIntTempC();
  int      getIntTempF();
  int      getExtTempC();
  int      getExtTempF();
  SWI2C*   getSWI2CObject();
  uint16_t readDeviceID();

private:
  SWI2C* _TMP007_device;
  int    _intTempC;
  int    _extTempC;
};

class OPT3001_SWI2C {
public:
  OPT3001_SWI2C(byte sda_pin, byte scl_pin, byte address = OPT3001_DEFAULT_ADDRESS);
  ~OPT3001_SWI2C();
  void             begin();
  void             readSensor();
  unsigned long    getLux();
  SWI2C*           getSWI2CObject();
  uint16_t         readDeviceID();

private:
  SWI2C* _OPT3001_device;
  unsigned long    _lux;
};

class BME280_SWI2C {
public:
  BME280_SWI2C(byte sda_pin, byte scl_pin, byte address = BME280_DEFAULT_ADDRESS);
  ~BME280_SWI2C();
  void      begin();
  void      readSensor();
  int       getTempC();
  int       getTempF();
  int       getRH();
  uint32_t  getPressurePa();
  uint16_t  getPressureInHg();
  SWI2C*    getSWI2CObject();
  uint8_t   readDeviceID();

private:
  SWI2C*    _BME280_device;
  void      calibrate();
  int       _tempC;
  int       _TempF;
  int       _RH;
  uint32_t  _pressurePa;

  // BME280 Calibration values
  uint16_t dig_T1;
  int16_t  dig_T2;
  int16_t  dig_T3;

  uint16_t dig_P1;
  int16_t  dig_P2;
  int16_t  dig_P3;
  int16_t  dig_P4;
  int16_t  dig_P5;
  int16_t  dig_P6;
  int16_t  dig_P7;
  int16_t  dig_P8;
  int16_t  dig_P9;

  uint8_t  dig_H1;
  int16_t  dig_H2;
  uint8_t  dig_H3;
  int16_t  dig_H4;
  int16_t  dig_H5;
  int8_t   dig_H6;
};

#endif
