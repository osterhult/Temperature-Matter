
#include <driver/gpio.h>
#include <esp_err.h>

#ifndef __DHT22X_H__
#define __DHT22X_H__

// /**
//  * Sensor type
//  */
// typedef enum
// {
//     DHT_TYPE_DHT11 = 0, //!< DHT11
//     DHT_TYPE_AM2301,    //!< AM2301 (DHT21, DHT22, AM2302, AM2321)
//     DHT_TYPE_SI7021     //!< Itead Si7021
// } dht_sensor_type_t;

// DHT22 Sensor Task
#define DHT22_TASK_STACK_SIZE 4096
#define DHT22_TASK_PRIORITY 4
#define DHT22_TASK_CORE_ID 1

#define DHT_GPIO GPIO_NUM_3 // GPIO pin connected to the DHT22

#define DHT_OK 0
#define DHT_CHECKSUM_ERROR -1
#define DHT_TIMEOUT_ERROR -2

/**
 * Starts DHT22 sensor task
 */
// void DHT22_task_start(void);

void DHT22_task_start(void);

// == function prototypes =======================================

void errorHandler(int response);
// void setDHTgpio(int gpio);
// int readDHT();
float getHumidity();
float getTemperature();
int getSignalLevel(int usTimeOut, bool state);

//////////////////////

/**
 * @brief Read integer data from sensor on specified pin
 *
 * Humidity and temperature are returned as integers.
 * For example: humidity=625 is 62.5 %, temperature=244 is 24.4 degrees Celsius
 *
 * @param sensor_type DHT11 or DHT22
 * @param pin GPIO pin connected to sensor OUT
 * @param[out] humidity Humidity, percents * 10, nullable
 * @param[out] temperature Temperature, degrees Celsius * 10, nullable
 * @return `ESP_OK` on success
 */
esp_err_t dht_read_data();

/**
 * @brief Read float data from sensor on specified pin
 *
 * Humidity and temperature are returned as floats.
 *
 * @param sensor_type DHT11 or DHT22
 * @param pin GPIO pin connected to sensor OUT
 * @param[out] humidity Humidity, percents, nullable
 * @param[out] temperature Temperature, degrees Celsius, nullable
 * @return `ESP_OK` on success
 */
esp_err_t dht_read_float_data();

#endif // __DHT22X_H__
