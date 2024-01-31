/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include <device.h>
#include <driver/gpio.h>
#include <esp_matter.h>
#include <cstdint>
#include <cmath>

#include <app_priv.h>

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <DHT22X.h>

using namespace chip::app::Clusters;
using namespace esp_matter;

static const char *TAG = "app_driver";
extern uint16_t temperature_sensor_endpoint_id;
extern uint16_t humidity_sensor_endpoint_id;

/**
 * Functions to handle a button to toggle the light
 */

static void app_driver_button_toggle_cb(void *arg, void *data)
{
    // TODO: Change this to measure temperature and humidity when button is pressed

    ESP_LOGI(TAG, "Toggle button pressed");

    esp_matter_attr_val_t temperature_value;
    temperature_value = esp_matter_invalid(NULL);
    temperature_value.type = esp_matter_val_type_t::ESP_MATTER_VAL_TYPE_INT16;
    temperature_value.val.i16 = app_driver_read_temperature(temperature_sensor_endpoint_id);
    esp_matter::attribute::update(temperature_sensor_endpoint_id, TemperatureMeasurement::Id, TemperatureMeasurement::Attributes::MeasuredValue::Id, &temperature_value);

    printf("===== Button toggle - Read Temperature value: %d ==== \n", temperature_value.val.i16);

    esp_matter_attr_val_t humidity_value;
    humidity_value = esp_matter_invalid(NULL);
    humidity_value.type = esp_matter_val_type_t::ESP_MATTER_VAL_TYPE_UINT16;
    humidity_value.val.u16 = app_driver_read_humidity(humidity_sensor_endpoint_id);
    esp_matter::attribute::update(humidity_sensor_endpoint_id, RelativeHumidityMeasurement::Id, RelativeHumidityMeasurement::Attributes::MeasuredValue::Id, &humidity_value);

    printf("===== Button toggle - Read Humidity value: %d ==== \n", humidity_value.val.u16);
}

/**
 * Update driver atttributes
 */
esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val)
{

    ESP_LOGI(TAG, "===== Update Temp sensor driver attributes ==== \n endpoint ID: %u, Cluster ID: %lu, Attribute ID: %lu \n", endpoint_id, cluster_id, attribute_id);
    printf("===== Update Temp sensor driver attributes ==== \n endpoint ID: %u, Cluster ID: %lu, Attribute ID: %lu \n", endpoint_id, cluster_id, attribute_id);

    esp_err_t err = ESP_OK;
    if (endpoint_id == temperature_sensor_endpoint_id)
    {
        ESP_LOGI(TAG, "===== Inside Update Temperature function #2 ==== \n");

        ESP_LOGI(TAG, "===== Read and set Temperature value ==== \n");
        err = app_driver_read_temperature(endpoint_id);

        node_t *node = node::get();
        endpoint_t *endpoint = endpoint::get(node, endpoint_id);
        cluster_t *cluster = cluster::get(endpoint, cluster_id);
        attribute_t *attribute = attribute::get(cluster, attribute_id);

        esp_matter_attr_val_t val = esp_matter_invalid(NULL);
        attribute::get_val(attribute, &val);
        val.val.b = !val.val.b;
        attribute::update(endpoint_id, cluster_id, attribute_id, &val);
    }
    else if (endpoint_id == humidity_sensor_endpoint_id)
    {
        ESP_LOGI(TAG, "===== Inside Update Humidity function #2 ==== \n");

        ESP_LOGI(TAG, "===== Read and set Humidity value ==== \n");
        err = app_driver_read_humidity(endpoint_id);

        node_t *node = node::get();
        endpoint_t *endpoint = endpoint::get(node, endpoint_id);
        cluster_t *cluster = cluster::get(endpoint, cluster_id);
        attribute_t *attribute = attribute::get(cluster, attribute_id);

        esp_matter_attr_val_t val = esp_matter_invalid(NULL);
        attribute::get_val(attribute, &val);
        val.val.b = !val.val.b;
        attribute::update(endpoint_id, cluster_id, attribute_id, &val);
    }
    else
    {
        ESP_LOGI(TAG, "===== Inside Update function #3 ==== \n");
        err = ESP_ERR_NOT_SUPPORTED;
    }

    return err;
}

int16_t app_driver_read_temperature(uint16_t endpoint_id)
{
    int16_t temp = getTemperature();
    printf("===== Read Temperature value: %d ==== \n", temp);
    return temp;
}

uint16_t app_driver_read_humidity(uint16_t endpoint_id)
{
    return (u_int16_t)getHumidity();
}

/**
 * Initialize DHT22 sensor
 */
app_driver_handle_t app_driver_DHT_sensor_init()
{
    // Start DHT22 sensor task
    DHT22_task_start();

    // Return a placeholder value (you may need to adapt the return type)
    return (app_driver_handle_t)1;
}

/**
 * Initialize the button
 */
app_driver_handle_t app_driver_button_init()
{
    /* Initialize button */
    button_config_t config = button_driver_get_config();
    button_handle_t handle = iot_button_create(&config);

    iot_button_register_cb(handle, BUTTON_PRESS_DOWN, app_driver_button_toggle_cb, NULL);

    return (app_driver_handle_t)handle;
}
