# External temperature and humidity sensor 

This projects creates a Temperature and Relative humidity sensor device using the ESP
Matter data model as well as using an external sensor for recieving values.

## 1. Additional Environment Setup

Been using a DHT22 sensor for measureing temperature and humidity.
GPIO pin for my ESP32-C3 DevKitC-02 board is pin 3. 

## 2. Post Commissioning Setup

No additional setup is required.

## 3. DHT22 driver
Found two different open-source projects on GitHub that none of them worked properly so I mashed them to one that works in this case. Hence the X in the file titles.

## 4. TODO:s or current issues
Currently I get the measured value when using CHIP-TOOL

**$ ./chip-tool temperaturemeasurement read measured-value [NODE-ID] 1**

**$ ./chip-tool relativehumiditymeasurement read measured-value [NODE-ID] 2**

Although the code doesn't update the values in HomeKit or GoogleHome yet. That is an current issue.

