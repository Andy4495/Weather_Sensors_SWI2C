# Weather Sensors SWI2C Library

[![Arduino Compile Sketches](https://github.com/Andy4495/Weather_Sensors_SWI2C/actions/workflows/arduino-compile-sketches.yml/badge.svg)](https://github.com/Andy4495/Weather_Sensors_SWI2C/actions/workflows/arduino-compile-sketches.yml)
[![Check Markdown Links](https://github.com/Andy4495/Weather_Sensors_SWI2C/actions/workflows/check-links.yml/badge.svg)](https://github.com/Andy4495/Weather_Sensors_SWI2C/actions/workflows/check-links.yml)

This library is designed for [TMP007][1], [OPT3001][2], and [BME280][3] sensors using software I2C and integer-only calculations. The library only supports polling mode (interrupts are not supported).

It was specifically designed for the [SENSORS BoosterPack][4], but it can be used as a software I2C interface for individual configurations of any of these sensors.

## Usage

*See the exmple sketch included with the library.*

1. Install the [SWI2C][5] library in addition to this library.

2. Include this library in your sketch:

    ```cpp
    #include "Weather_Sensors_SWI2C.h"
    ```

3. Instantiate objects for the sensors that you are using:

    ```cpp
    TMP007_SWI2C  myTMP007(sda_pin, scl_pin, TMP007_7bit_I2C_address);
    OPT3001_SWI2C myOPT3001(sda_pin, scl_pin, OPT3001_7bit_I2C_address);
    BME280_SWI2C  myBME280(sda_pin, scl_pin, BME280_7bit_I2C_address);
    ```

4. Initialize the sensor objects that you created:

    ```cpp
    myTMP007.begin();
    myOPT3001.begin();
    myBME280.begin();
   ```

5. Read the measurements from the sensors. Note that every time that want to get a new sensor reading, you need to call the readSensor() method for that sensor.

    ```cpp
    myTMP007.readSensor();
    myOPT3001.readSensor();
    myBME280.readSensor();

    ```

6. Access the sensor values read from the last readSensor() call. Each sensor has different accessor methods as described below.

## Accessing the Sensor Data

### TMP007

```cpp
myTMP007.getIntTempC();  // Returns int representing internal (die) temperature in units of 0.1 degrees Celsius
myTMP007.getIntTempF();  // Returns int representing internal (die) temperature in units of 0.1 degrees Fahrenheit
myTMP007.getExtTempC();  // Returns int representing external (object) temperature in units of 0.1 degrees Celsius
myTMP007.getExtTempF();  // Returns int representing external (object) temperature in units of 0.1 degrees Fahrenheit
```

### OPT3001

```cpp
myOPT3001.getLux();  // Returns unsigned long representing light intensity in lux.
```

### BME280

```cpp
myBME280.getTempC();  // Returns int representing temperature in units of 0.01 degrees Celsius
myBME280.getTempF();  // Returns int representing temperature in units of 0.1 degrees Fahrenheit
myBME280.getRH();  // Returns int representing relative humidity in units of 0.1 %RH
myBME280.getPressurePa();  // Returns uint32_t representing barometric pressure in units of pascals (Pa)
myBME280.getPressureInHg();  // Returns uint16_t representing barometric pressure in units of 0.01 inches of mercury (inHg)
```

## Implementation Details

This library was inspired by Rei Vilo's [Weather Sensors Library][7]. My library is implemented using software I2C and uses integer-only math. In addition, my library configures the sensors somewhat differently.

The BME280 object uses code based on Section 4.2.3 *Compensation Forumulas* from the BME280 [datasheet][3].

All sensors support the `readDeviceID()` method, which is a factory programmed value that can be used to confirm you are communicating with the correct device:

```cpp
myTMP007.readDeviceID();  // Should return the 16-bit value 0x0078
myOPT3001.readDeviceID(); // Should return the 16-bit value 0x3001
myBME280.readDeviceID();  // Should return the 8-bit value 0x60
```

All sensors also support the `getSWI2CObject()` method. This returns a pointer to the underlying `SWI2C` object and its available [public methods][5]. This could be useful for debugging and other specialized situations.

The constructors have default values for the I2C_Address. If your device is configured with its address select pins tied to ground, you can call the constructor without the I2C_Address and just include the sda_pin and scl_pin parameters. The default 7-bit I2C addresses are:

- TMP007: `0x40`
- OPT3001: `0x44`
- BME280: `0x76`

The library provides unit conversions of Celsius to Fahrenheit and pascals to inches of mercury. Other unit conversions are left as an exercise for the user.

The library only supports a single hardcoded configuration for each device. This configuration works well for a battery-powered weather sensor which takes infrequent sensor measurements (e.g., once a minute). Note that in some cases, the device's low-power mode is not configured. This is because the devices draw so little current (microamps), that the added complexity of supporting low power modes is not worth the power savings. The current device configurations are:

- TMP007: Two samples per measurement, low power mode
- OPT3001: Automatic full scale mode, 100 ms conversion time, continuous conversion mode
- BME280: 1000 ms standby, IIR filter off, pressure sampling x1, temperature sampling x1, humidity sampling x1, normal mode

For applications which require more frequent (i.e., on the order of once per second) or more accurate measurements, different configuration values may be needed and will require changes to the library. See the device datasheets for configuration options.

## External Libraries That Need To Be Installed

[SWI2C][5] - Software I2C library

## References

- TMP007 [datasheet][1]
- OPT3001 [datasheet][2]
- BME280 [datasheet][3]
  - This library uses code adapted from the datasheet section 4.2.3 for the sensor compensation calculations.
  - Bosch Sensortec also publishes driver code with similar calculations on [GitHub][6]

## License

The software and other files in this repository (with the exception listed below) are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

The implementation of `BME280_SWI2C::readSensor()` in `Weather_Sensors_SWI2C.cpp` is adapted from sample code provided by Bosch Sensortec. The library specifically adapted code from the BME280 [datasheet][3] section 4.2.3, which does not list licensing information beyond saying "the code below can be applied at the user's risk." However, similar code is published by Bosch Sensortec on [GitHub][6] under the BSD-3-Clause license.

[1]: https://cdn-shop.adafruit.com/datasheets/tmp007.pdf
[2]: https://www.ti.com/lit/ds/symlink/opt3001.pdf
[3]: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf
[4]: https://www.ti.com/lit/ug/slau666b/slau666b.pdf
[5]: https://github.com/Andy4495/SWI2C
[6]: https://github.com/BoschSensortec/BME280_driver
[7]: https://github.com/rei-vilo/SensorsWeather_Library
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[//]: # ([200]: https://github.com/Andy4495/Weather_Sensors_SWI2C)
