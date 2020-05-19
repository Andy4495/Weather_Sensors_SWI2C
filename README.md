Weather Sensors SWI2C Library
=============================

This library is designed for [TMP007][1], [OPT3001][2], and [BME280][3] sensors using software I2C and integer-only calculations. The library only supports polling mode (interrupts are not supported).

It was specifically designed for the [SENSORS BoosterPack][4], but it can be used as a software I2C interface for individual configurations of any of these sensors.

Usage
-----
*See the sketch included in the `examples` folder.*

1. Install the [SWI2C][5] library in addition to this library.
2. Include the library in your sketch:
```
#include "Weather_Sensors_SWI2C.h"
```
3. Instantiate objects for the sensors that you are using:
```
TMP007_SWI2C  myTMP007(sda_pin, scl_pin, I2C_Address_7bit);
OPT3001_SWI2C myOPT3001(sda_pin, scl_pin, I2C_Address_7bit);
BME280_SWI2C  myBME280(sda_pin, scl_pin, I2C_Address_7bit);
```
4. Initialize the sensor objects that you created:
```
myTMP007.begin();
myOPT3001.begin();
myBME280.begin();
```
5. Read the measurements from the sensors. Note that every time that want to get a new sensor reading, you need to call the readSensor() method for that sensor.
```
myTMP007.readSensor();
myOPT3001.readSensor();
myBME280.readSensor();
```
6. Access the sensor values read from the last readSensor() call. Each sensor has different accessor methods as described below.

Accessing the Sensor Data
-------------------------
#### TMP007
```
myTMP007.getIntTempC();  // Returns int representing internal (die) temperature in units of 0.1 degrees Celsius
myTMP007.getIntTempF();  // Returns int representing internal (die) temperature in units of 0.1 degrees Fahrenheit
myTMP007.getExtTempC();  // Returns int representing external (object) temperature in units of 0.1 degrees Celsius
myTMP007.getExtTempF();  // Returns int representing external (object) temperature in units of 0.1 degrees Fahrenheit
```

#### OPT3001
```
myOPT3001.getLux();  // Returns unsigned long representing light intensity in lux.
```

#### BME280
```
myBME280.getTempC();  // Returns int representing temperature in units of 0.01 degrees Celsius
myBME280.getTempF();  // Returns int representing temperature in units of 0.1 degrees Fahrenheit
myBME280.getRH();  // Returns int representing relative humidity in units of 0.1 %RH
myBME280.getPressurehPa();  // Returns uint16_t representing barometric pressure in units of hectopascals (hPa)
myBME280.getPressureInHg();  // Returns uint16_t representing barometric pressure in units of 0.01 inches of mercury (inHg)
```

Implementation Details
----------------------

This library was inspired by Rei Vilo's [Weather Sensors Library][7]. My library is implemented using software I2c and uses integer-only math. In addition, my library configures the sensors slightly differently.

The BME280 object uses code based on portions of BoschSensortec's [BME280 driver code][6].

All sensors support the `readDeviceID()` method, which is a factory programmed value that can be used to confirm you are communicating with the correct device:
```
myTMP007.readDeviceID();  // Should return the 16-bit value 0x0078
myOPT3001.readDeviceID(); // Should return the 16-bit value 0x3001
myBME280.readDeviceID();  // Should return the 8-bit value 0x60
```

The constructors have default values for the I2C_Address. If your device is configured with its address select pins tied to ground, you can call the constructor without the I2C_Address and just include the sda_pin and scl_pin parameters. The default 7-bit I2C addresses are:
- TMP007  - 0x40
- OPT3001 - 0x44
- BME280  - 0x76

The library provides unit conversions of Celsius to Fahrenheit and pascals to inches of mercury. Other unit conversions are left as an exercise to the user.

The library only supports a single hardcoded configuration for each device. This configuration works well for a battery-powered weather sensor which takes infrequent sensor measurements (e.g., once a minute). For applications which require more frequent (i.e., on the order of once per second) or more accurate measurements, different configuration values may be needed and will require changes to the library. See the device datasheets for configuration options. The current device configurations are:

- TMP007: Two samples per measurement, low power mode
- OPT3001: Automatic full scale mode, 100 ms conversion time, continuous conversion mode
- BME280: 1000 ms standby, IIR filter off, pressure sampling x1, temperature sampling x4, humidity sampling x1, forced mode

External Libraries That Need To Be Installed
--------------------------------------------
[SWI2C][5] - Software I2C library

References
----------
+ [TMP007][1]
+ [OPT3001][2]
+ [BME280][3]

[1]: https://cdn-shop.adafruit.com/datasheets/tmp007.pdf
[2]: https://www.ti.com/lit/ds/symlink/opt3001.pdf
[3]: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf
[4]: https://www.ti.com/tool/BOOSTXL-SENSORS
[5]: https://github.com/Andy4495/SWI2C
[6]: https://github.com/BoschSensortec/BME280_driver
[7]: https://github.com/rei-vilo/SensorsWeather_Library
