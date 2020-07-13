/*
Library to interface with TMP007, OPT3001, and BME280 using Software I2C.
Uses integer-only calculations and variables.
   https://github.com/Andy4495/Weather_Sensors_SWI2C

Version History
   1.0.0    05/17/2020  A.T.   Original
*/

#include "Weather_Sensors_SWI2C.h"

TMP007_SWI2C::TMP007_SWI2C(byte sda_pin, byte scl_pin, byte address) {
  _TMP007_device = new SWI2C(sda_pin, scl_pin, address);
}

TMP007_SWI2C::~TMP007_SWI2C() {
  delete _TMP007_device;
}

void TMP007_SWI2C::begin() {
  _TMP007_device->begin();
  _TMP007_device->write2bToRegisterMSBFirst(TMP007_CONFIGURATION, TMP007_VALUE_CONFIG);
}

void TMP007_SWI2C::readSensor() {
  uint16_t data16;

  _TMP007_device->read2bFromRegisterMSBFirst(TMP007_INTERNAL_TEMPERATURE, &data16);
  // Temperature is returned in 14 msb, so shift over to lsb
  _intTempC = (int)data16 >> 2;

  // Temperature is returned in 1/32 degree Celsius
  // Next, multiply by 10 and divide by 32 (right shift 5) to get units in 1/10 degree Celsius (using integer math)
  // Add in 16 to round up the hundredths to tenths (sixteen 1/32 degree corresponds to 1/2 for rounding)
  _intTempC = (_intTempC * 10 + 16) >> 5;

  _TMP007_device->read2bFromRegisterMSBFirst(TMP007_EXTERNAL_TEMPERATURE, &data16);
  // Temperature is returned in 14 msb, so shift over to lsb
  _extTempC = (int)data16 >> 2;

  // Temperature is returned in units of 1/32 degree Celsius
  // Next, multiply by 10 and divide by 32 (right shift 5) to get units in 1/10 degree Celsius (using integer math)
  // Add in 16 to round up the hundredths to tenths (sixteen 1/32 degree corresponds to 1/2 for rounding)
  _extTempC = (_extTempC * 10 + 16) >> 5;
}

int TMP007_SWI2C::getIntTempC() {
  return _intTempC;
}

int TMP007_SWI2C::getIntTempF() {
  // Standard formula is F = 9/5 * C + 32
  // Since temp is stored in 0.1 degrees, then formula is F = 9/5 * C + 320
  // When using integer math, need to scale up by 10 and add five to round correctly,
  // then scale down again to get back into correct units of 0.1 degrees.
  // So the integer formula with rounding is now F = (90/5 * C + 5) / 10 + 320.
  // Which simplifies to F = (18 * C + 5) / 10 + 320
  return (_intTempC * 18 + 5) / 10 + 320;
}

int TMP007_SWI2C::getExtTempC() {
  return _extTempC;
}

int TMP007_SWI2C::getExtTempF() {
  // See TMP007_SWI2C::getIntTempF() above for explanation on this conversion formula
  return (_extTempC * 18 + 5) / 10 + 320;
}

uint16_t TMP007_SWI2C::readDeviceID() {
  uint16_t data16;
  _TMP007_device->read2bFromRegisterMSBFirst(TMP007_DEVICE_ID, &data16);
  return data16;
}

OPT3001_SWI2C::OPT3001_SWI2C(byte sda_pin, byte scl_pin, byte address) {
  _OPT3001_device = new SWI2C(sda_pin, scl_pin, address);
}

OPT3001_SWI2C::~OPT3001_SWI2C() {
  delete _OPT3001_device;
}

void OPT3001_SWI2C::begin() {
  _OPT3001_device->begin();
  _OPT3001_device->write2bToRegisterMSBFirst(OPT3001_CONFIGURATION_REGISTER, OPT3001_STARTUP_CONFIG);
}

void OPT3001_SWI2C::readSensor() {
  uint16_t data16;
  unsigned int exponent;
  unsigned int mantissa;

  _OPT3001_device->read2bFromRegisterMSBFirst(OPT3001_RESULT_REGISTER, &data16);
  exponent = ((data16 >> 12) & 0x000f);   // Exponent is 4 msb
  exponent = 1 << exponent;               // Convert to "lsb size", in units of 1/100 lux
  mantissa = data16 & 0x0fff;             // Fractional part is 12 lsb
  _lux = (mantissa * exponent + 50) / 100;      // Only need precision down to single lux, add 50 to round up fractional lux
}

unsigned long OPT3001_SWI2C::getLux() {
  return _lux;
}

uint16_t OPT3001_SWI2C::readDeviceID(){
  uint16_t data16;
  _OPT3001_device->read2bFromRegisterMSBFirst(OPT3001_DEVICE_ID_REGISTER, &data16);
  return data16;
}

BME280_SWI2C::BME280_SWI2C(byte sda_pin, byte scl_pin, byte address) {
    _BME280_device = new SWI2C(sda_pin, scl_pin, address);
}

BME280_SWI2C::~BME280_SWI2C() {
    delete _BME280_device;
}

void BME280_SWI2C::begin() {
  // Initialize BME280
  _BME280_device->begin();
  _BME280_device->writeToRegister(BME280_RESET, BME280_VALUE_RESET_COMMAND);   // Force a soft reset
  _BME280_device->writeToRegister(BME280_CONFIGURATION, BME280_VALUE_CONFIG);
  _BME280_device->writeToRegister(BME280_CONTROL_HUMIDITY, BME280_VALUE_HUM_CONFIG); // Becomes effective after writing TP Config
  _BME280_device->writeToRegister(BME280_CONTROL_TEMPERATURE_PRESSURE, BME280_VALUE_TP_CONFIG);  // Temp and Pressure configuration, needs to be set after Humidity
  calibrate();
}

int BME280_SWI2C::getTempC() {
  return _tempC;  // returns value with units of hundredths degree Celsius
}

int BME280_SWI2C::getTempF() {
  // See TMP007_SWI2C::getIntTempF() above for explanation of this conversion formula
  // Note that for the BME280, temperature units are 0.01 degrees, so the Celsius value is alread scaled up
  return ((_tempC * 9) / 5 + 5) / 10 + 320;  // returns value with units of tenths degrees Fahrenheit
}

int BME280_SWI2C::getRH() {
  return _RH;
}

uint32_t BME280_SWI2C::getPressurePa() {
  return _pressurePa;
}

uint16_t BME280_SWI2C::getPressureInHg() {
  // Scale up by 100 in order to do interger math
  // Since device is only accurate to +/- 1.5 hPa, and inHG conversion factor depends on temp, don't bother adding a rounding factor
  return (_pressurePa * 100) / 3386;
}

uint8_t BME280_SWI2C::readDeviceID(){
  uint8_t data8;
  _BME280_device->read1bFromRegister(BME280_ID, &data8);
  return data8;
}

void BME280_SWI2C::calibrate() {
  uint8_t  data8;
  uint16_t data16;

  _BME280_device->read2bFromRegister(BME280_CALIBRATION_T1, &dig_T1); // uint16_t
  _BME280_device->read2bFromRegister(BME280_CALIBRATION_T2, &data16); // int16_t
  dig_T2 = (int16_t) data16;
  _BME280_device->read2bFromRegister(BME280_CALIBRATION_T3, &data16); // int16_t
  dig_T3 = (int16_t) data16;

  _BME280_device->read2bFromRegister(BME280_CALIBRATION_P1, &dig_P1); // uint16_t
  _BME280_device->read2bFromRegister(BME280_CALIBRATION_P2, &data16); // int16_t
  dig_P2 = (int16_t) data16;
  _BME280_device->read2bFromRegister(BME280_CALIBRATION_P3, &data16); // int16_t
  dig_P3 = (int16_t) data16;
  _BME280_device->read2bFromRegister(BME280_CALIBRATION_P4, &data16); // int16_t
  dig_P4 = (int16_t) data16;
  _BME280_device->read2bFromRegister(BME280_CALIBRATION_P5, &data16); // int16_t
  dig_P5 = (int16_t) data16;
  _BME280_device->read2bFromRegister(BME280_CALIBRATION_P6, &data16); // int16_t
  dig_P6 = (int16_t) data16;
  _BME280_device->read2bFromRegister(BME280_CALIBRATION_P7, &data16); // int16_t
  dig_P7 = (int16_t) data16;
  _BME280_device->read2bFromRegister(BME280_CALIBRATION_P8, &data16); // int16_t
  dig_P8 = (int16_t) data16;
  _BME280_device->read2bFromRegister(BME280_CALIBRATION_P9, &data16); // int16_t
  dig_P9 = (int16_t) data16;

  _BME280_device->read1bFromRegister(BME280_CALIBRATION_H1, &dig_H1); // uint8_t
  _BME280_device->read2bFromRegister(BME280_CALIBRATION_H2, &data16); // int16_t
  dig_H2 = (int16_t) data16;
  _BME280_device->read1bFromRegister(BME280_CALIBRATION_H3, &dig_H3); // uint8_t

  //    0xE4 / 0xE5[3:0]  dig_H4 [11:4] / [3:0] signed short = int16_t
  _BME280_device->read1bFromRegister(BME280_CALIBRATION_H4, &data8);
  dig_H4 = data8 << 4;
  _BME280_device->read1bFromRegister(BME280_CALIBRATION_H5, &data8);
  dig_H4 = dig_H4 + (data8 & 0x0F);
  //    0xE5[7:4] / 0xE6 dig_H5 [3:0] / [11:4] signed short = int16_t
  // dig_H5  = (_BME280_device->read1bFromRegister(BME280_ADDRESS, BME280_CALIBRATION_H5) >> 4) + (_BME280_device->read1bFromRegister(BME280_ADDRESS, BME280_CALIBRATION_H5B2) << 4);
  _BME280_device->read1bFromRegister(BME280_CALIBRATION_H5, &data8);
  dig_H5 = (data8 >> 4) & 0x0F;
  _BME280_device->read1bFromRegister(BME280_CALIBRATION_H5B2, &data8);
  data16 = data8;
  dig_H5 = dig_H5 + (data16 << 4);

  _BME280_device->read1bFromRegister(BME280_CALIBRATION_H5, &data8); // int8_t
  dig_H6 = (int8_t) data8;
}

void BME280_SWI2C::readSensor() {
  uint8_t  BME280RawData[8];
  int32_t  rawBME280T, rawBME280P, rawBME280H;
  int32_t  var1_32, var2_32;
  int64_t  var1_64, var2_64, var3_64;
  int32_t  v_x1_u32r;
  int32_t  t_fine;

  // Write register BME280_DATA_F7_FE with no data, plus a stop
  // 8 consecutive byte reads
  _BME280_device->startBit();
  _BME280_device->writeAddress(0); // 0 == Write bit
  _BME280_device->checkAckBit();
  _BME280_device->writeRegister(BME280_DATA_F7_FE);
  _BME280_device->checkAckBit();
  //  stopBit();
  _BME280_device->startBit();
  _BME280_device->writeAddress(1); // 1 == Read bit
  _BME280_device->checkAckBit();
  // Loop 8 bytes
  for (int i = 0; i < 8; i++) {
    BME280RawData[i] = _BME280_device->read1Byte();
    if (i < 7) {
      _BME280_device->writeAck();
    }
    else { // Last byte needs a NACK
      _BME280_device->checkAckBit(); // Controller needs to send NACK when done reading data
    }
  }
  _BME280_device->stopBit();

  rawBME280P = ((uint32_t)BME280RawData[0] << 12) + ((uint32_t)BME280RawData[1] << 4) + ((uint32_t)BME280RawData[2] >> 4);
  rawBME280T = ((uint32_t)BME280RawData[3] << 12) + ((uint32_t)BME280RawData[4] << 4) + ((uint32_t)BME280RawData[5] >> 4);
  rawBME280H = ((uint32_t)BME280RawData[6] <<  8) + (uint32_t)BME280RawData[7];

  // The following T, H, and P compensation code is adapted from BoschSensortec's BME280 driver code on GitHub. (https://github.com/BoschSensortec/BME280_driver)

  var1_32  = ((((rawBME280T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
  var2_32  = (((((rawBME280T >> 4) - ((int32_t)dig_T1)) * ((rawBME280T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
  t_fine   = var1_32 + var2_32;
  _tempC   = ((t_fine * 5 + 128) >> 8);  // 0.01 C

  v_x1_u32r = (t_fine - ((int32_t)76800));
  v_x1_u32r = (((((rawBME280H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
  if (v_x1_u32r < 0)
  {
    v_x1_u32r = 0;
  }
  if (v_x1_u32r > 419430400)
  {
    v_x1_u32r = 419430400;
  }
  // Technically should divide by 1024 and return in units of 0.01% RH, but device is only accurate to +/- 3 %RH,
  // so divide by 102 and return units of 0.1 %RH, which is still give more significant digits than accuracy of device.
  _RH = ((v_x1_u32r >> 12) / 102); // units of 0.1 %RH

  var1_64 = ((int64_t)t_fine) - 128000;
  var2_64 = var1_64 * var1_64 * (int64_t)dig_P6;
  var2_64 = var2_64 + ((var1_64 * (int64_t)dig_P5) << 17);
  var2_64 = var2_64 + (((int64_t)dig_P4) << 35);
  var1_64 = ((var1_64 * var1_64 * (int64_t)dig_P3) >> 8) + ((var1_64 * (int64_t)dig_P2) << 12);
  var1_64 = (((((int64_t)1) << 47) + var1_64)) * ((int64_t)dig_P1) >> 33;
  if (var1_64 != 0)
  {
    var3_64 = 1048576 - rawBME280P;
    var3_64 = (((var3_64 << 31) - var2_64) * 3125) / var1_64;
    var1_64 = (((int64_t)dig_P9) * (var3_64 >> 13) * (var3_64 >> 13)) >> 25;
    var2_64 = (((int64_t)dig_P8) * var3_64) >> 19;

    var3_64 = ((var3_64 + var1_64 + var2_64) >> 8) + (((int64_t)dig_P7) << 4);
    _pressurePa =  (var3_64 / 256); // in units of Pa (or 0.01 hPa)
  }
  else {   // Avoid divide by 0 and return 0
    _pressurePa =  0;
  }

}
