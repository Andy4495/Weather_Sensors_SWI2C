Weather Sensors SWI2C Library
=============================

This library is designed Library for [TMP007][1], [OPT3001][2], [BME280][3] sensors using software I2C and integer-only calculations. Specifically designed for [SENSORS BoosterPack][4], but can be used as a software I2C interface for individual configurations of these sensors.

Usage
-----
*See the sketch included in the `examples` folder.*

External Libraries
------------------
[SWI2C][5] - Software I2C library

Inspired by Rei Vilo's library (add reference).

BME280 pressure compensation adapted from Bosh code compensate_pressure 64-bit integer from (add reference and license info).

Some conversions provided by library, others are left as an exercise for the reader. 

References
----------
+ [datasheet][1]

[1]: https://cdn-shop.adafruit.com/datasheets/tmp007.pdf
[2]: https://www.ti.com/lit/ds/symlink/opt3001.pdf
[3]: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf
[4]: https://www.ti.com/tool/BOOSTXL-SENSORS
[5]: https://github.com/Andy4495/SWI2C
[6]: https://github.com/BoschSensortec/BME280_driver
