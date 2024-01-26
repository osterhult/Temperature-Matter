/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#pragma once

#include <esp_err.h>
#include <esp_matter.h>

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include "esp_openthread_types.h"
#endif

/** Standard max values (used for remapping attributes) */
// #define STANDARD_BRIGHTNESS 100
// #define STANDARD_HUE 360
// #define STANDARD_SATURATION 100
// #define STANDARD_TEMPERATURE_FACTOR 1000000

/** Matter max values (used for remapping attributes) */
// #define MATTER_BRIGHTNESS 254
// #define MATTER_HUE 254
// #define MATTER_SATURATION 254
// #define MATTER_TEMPERATURE_FACTOR 1000000

/** Default attribute values used during initialization */
// #define DEFAULT_POWER true
// #define DEFAULT_BRIGHTNESS 64
// #define DEFAULT_HUE 128
// #define DEFAULT_SATURATION 254

// // Defining the GPIO pins for leds
// // #define LED_GPIO_PIN GPIO_NUM_2 // Single LED
// #define RGB_LED_RED_PIN GPIO_NUM_5   // GPIO pin connected to the red color of the RGB LED
// #define RGB_LED_GREEN_PIN GPIO_NUM_4 // GPIO pin connected to the green color of the RGB LED
// #define RGB_LED_BLUE_PIN GPIO_NUM_2  // GPIO pin connected to the blue color of the RGB LED

// // Definition of color temperatures
// #define MIN_COLOR_TEMPERATURE 1000.0f
// #define MAX_COLOR_TEMPERATURE 40000.0f

// DHT22 Values
#define DHT22_GPIO_PIN GPIO_NUM_3
#define DEFAULT_TEMPERATURE_VALUE 20
#define DEFAULT_HUMIDITY_VALUE 40

typedef void *app_driver_handle_t;

/** Initialize the temperature and humidity drivers
 *
 * This initializes the temperature and humidity drivers.
 *
 * @return Handle on success.
 * @return NULL in case of failure.
 */
// app_driver_handle_t app_driver_light_init();
app_driver_handle_t app_driver_DHT_sensor_init();

/** Initialize the button driver
 *
 * This initializes the button driver associated with the selected board.
 *
 * @return Handle on success.
 * @return NULL in case of failure.
 */
app_driver_handle_t app_driver_button_init();

/** Driver Update
 *
 * This API should be called to update the driver for the attribute being updated.
 * This is usually called from the common `app_attribute_update_cb()`.
 *
 * @param[in] endpoint_id Endpoint ID of the attribute.
 * @param[in] cluster_id Cluster ID of the attribute.
 * @param[in] attribute_id Attribute ID of the attribute.
 * @param[in] val Pointer to `esp_matter_attr_val_t`. Use appropriate elements as per the value type.
 *
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val);

/** Set defaults for temperature and humidity drivers
 *
 * Set the attribute drivers to their default values from the created data model.
 *
 * @param[in] endpoint_id Endpoint ID of the driver.
 *
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
// esp_err_t app_driver_light_set_defaults(uint16_t endpoint_id);
int16_t app_driver_read_temperature(uint16_t endpoint_id);
uint16_t app_driver_read_humidity(uint16_t endpoint_id);

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#define ESP_OPENTHREAD_DEFAULT_RADIO_CONFIG() \
    {                                         \
        .radio_mode = RADIO_MODE_NATIVE,      \
    }

#define ESP_OPENTHREAD_DEFAULT_HOST_CONFIG()               \
    {                                                      \
        .host_connection_mode = HOST_CONNECTION_MODE_NONE, \
    }

#define ESP_OPENTHREAD_DEFAULT_PORT_CONFIG()                                            \
    {                                                                                   \
        .storage_partition_name = "nvs", .netif_queue_size = 10, .task_queue_size = 10, \
    }
#endif
