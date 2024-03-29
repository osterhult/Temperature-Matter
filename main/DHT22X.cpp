
////////////////////////////// DHT22 CODE //////////////////////////////

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include "DHT22X.h"

// DHT timer precision in microseconds
#define DHT_TIMER_INTERVAL 2
#define DHT_DATA_BITS 40
#define DHT_DATA_BYTES (DHT_DATA_BITS / 8)

// == global defines =============================================

static const char *TAG = "DHTX";

float humidity = 0.;
float temperature = 0.;

static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
#define PORT_ENTER_CRITICAL() portENTER_CRITICAL(&mux)
#define PORT_EXIT_CRITICAL() portEXIT_CRITICAL(&mux)

// == get temp & hum =============================================

/**
 * Get the humidity
 */
float getHumidity()
{
    return humidity;
}

/**
 * Get the temperature
 */
float getTemperature()
{
    return temperature;
}

// == error handler ===============================================

/**
 * Error handler
 * @param response
 */
void errorHandler(int response)
{
    switch (response)
    {

    case DHT_TIMEOUT_ERROR:
        printf("Sensor Timeout\n");
        break;

    case DHT_CHECKSUM_ERROR:
        printf("CheckSum error\n");
        break;

    case DHT_OK:
        break;

    default:
        printf("Unknown error\n");
    }
}

/**
 * Get the signal level
 * @param usTimeOut Timeout
 * @param state State of the signal
 * @return uSec is number of microseconds passed
 */

int getSignalLevel(int usTimeOut, bool state)
{
    int uSec = 0;
    while (gpio_get_level(DHT_GPIO) == state)
    {
        if (uSec++ > usTimeOut)
        {
            return -1; // Timeout
        }
        esp_rom_delay_us(1);
    }
    return uSec;
}

///////////////////////////////////////////////
/**
 * Pack two data bytes into single value and take into account sign bit.
 */

static inline int16_t dht_convert_data(uint8_t msb, uint8_t lsb)
{
    int16_t data;
    data = msb & 0x7F;
    data <<= 8;
    data |= lsb;

    if (msb & BIT(7))
        data = -data; // convert it to negative

    return data;
}

/**
 * Wait specified time for pin to go to a specified state.
 * If timeout is reached and pin doesn't go to a requested state
 * false is returned.
 * The elapsed time is returned in pointer 'duration' if it is not NULL.
 */
static esp_err_t dht_await_pin_state(uint32_t timeout, int expected_pin_state, uint32_t *duration)
{
    /* XXX dht_await_pin_state() should save pin direction and restore
     * the direction before return. however, the SDK does not provide
     * gpio_get_direction().
     */
    gpio_set_direction(DHT_GPIO, GPIO_MODE_INPUT);
    for (uint32_t i = 0; i < timeout; i += DHT_TIMER_INTERVAL)
    {
        // need to wait at least a single interval to prevent reading a jitter
        // ets_delay_us(DHT_TIMER_INTERVAL);
        esp_rom_delay_us(DHT_TIMER_INTERVAL);

        if (gpio_get_level(DHT_GPIO) == expected_pin_state)
        {
            if (duration)
                *duration = i;
            return ESP_OK;
        }
    }

    return ESP_ERR_TIMEOUT;
}

static inline esp_err_t dht_fetch_data(uint8_t data[DHT_DATA_BYTES])
{
    int uSec = 0;
    uint8_t byteInx = 0;
    uint8_t bitInx = 7;

    for (int k = 0; k < DHT_DATA_BYTES; k++)
        data[k] = 0;

    gpio_set_direction(DHT_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT_GPIO, 0);
    esp_rom_delay_us(3000);

    gpio_set_level(DHT_GPIO, 1);
    esp_rom_delay_us(25);

    gpio_set_direction(DHT_GPIO, GPIO_MODE_INPUT);

    // Step through Phase 'B', 40us
    uSec = getSignalLevel(85, 0);
    if (uSec < 0)
        return ESP_FAIL;

    uSec = getSignalLevel(85, 1);
    if (uSec < 0)
        return ESP_FAIL;

    for (int k = 0; k < DHT_DATA_BITS; k++)
    {
        uSec = getSignalLevel(56, 0);
        if (uSec < 0)
            return ESP_FAIL;

        uSec = getSignalLevel(75, 1);
        if (uSec < 0)
            return ESP_FAIL;

        if (uSec > 40)
        {
            data[byteInx] |= (1 << bitInx);
        }

        if (bitInx == 0)
        {
            bitInx = 7;
            ++byteInx;
        }
        else
            bitInx--;
    }

    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
        return ESP_OK;
    else
        return ESP_FAIL;
}

esp_err_t dht_read_data()
{
    uint8_t data[DHT_DATA_BYTES] = {0};

    gpio_set_direction(DHT_GPIO, GPIO_MODE_OUTPUT_OD);
    gpio_set_level(DHT_GPIO, 1);

    esp_err_t result = dht_fetch_data(data);

    if (result == ESP_OK)
    {
        // PORT_EXIT_CRITICAL();
    }

    /* restore GPIO direction because, after calling dht_fetch_data(), the
     * GPIO direction mode changes */
    gpio_set_direction(DHT_GPIO, GPIO_MODE_OUTPUT_OD);

    gpio_set_level(DHT_GPIO, 1);

    if (result != ESP_OK)
    {
        printf("==== dht_read_data - ESP_NOT_OK ====\n");
        return result;
    }

    if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
    {
        ESP_LOGE(TAG, "Checksum failed, invalid data received from sensor");

        return ESP_ERR_INVALID_CRC;
    }

    humidity = dht_convert_data(data[0], data[1]) / 10;
    temperature = dht_convert_data(data[2], data[3]) / 10;

    ESP_LOGI(TAG, "Sensor data: humidity=%f, temp=%f", humidity, temperature);

    return ESP_OK;
}

esp_err_t dht_read_float_data()
{
    esp_err_t res = dht_read_data();

    if (res != ESP_OK)
    {
        return res;
    }

    return ESP_OK;
}

////////////////////////////// END OF DHT22 CODE //////////////////////////////
