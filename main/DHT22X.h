
#include <driver/gpio.h>
#include <esp_err.h>

#ifndef __DHT22X_H__
#define __DHT22X_H__

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
 * @return DHT_OK if successful, otherwise error code
 */
void DHT22_task_start(void);

/**
 * Stops DHT22 sensor task
 * @return DHT_OK if successful, otherwise error code
 */
void errorHandler(int response);

/**
 * Get the humidity
 * @return humidity
 */
float getHumidity();

/**
 * Get the temperature
 * @return temperature
 */
float getTemperature();

/**
 * Get the signal level
 * @param usTimeOut Timeout
 * @param state State of the signal
 * @return uSec is number of microseconds passed
 */
int getSignalLevel(int usTimeOut, bool state);

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
