
#pragma once

#include <esp_err.h>
#include <esp_matter.h>

#ifndef __APP_DRIVER_H__
#define __APP_DRIVER_H__

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include "esp_openthread_types.h"
#endif

// DHT22 Values
#define DHT22_GPIO_PIN GPIO_NUM_3
#define DEFAULT_TEMPERATURE_VALUE 1
#define DEFAULT_HUMIDITY_VALUE 1

#define DEFAULT_MEASURE_INTERVAL 20000

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
uint16_t app_driver_read_temperature(uint16_t endpoint_id);
uint16_t app_driver_read_humidity(uint16_t endpoint_id);

esp_err_t temperature_attribute_update_cb(esp_matter::attribute::callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data);
esp_err_t humidity_attribute_update_cb(esp_matter::attribute::callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data);
esp_err_t sensor_attribute_update_cb(esp_matter::attribute::callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data);

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

#endif // __APP_DRIVER_H__