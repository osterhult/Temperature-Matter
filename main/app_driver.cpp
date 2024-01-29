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

#include "DHT22.h"
#include <app_priv.h>

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
    ESP_LOGI(TAG, "Toggle button pressed");
    uint16_t endpoint_id = temperature_sensor_endpoint_id;
    uint32_t cluster_id = OnOff::Id;
    uint32_t attribute_id = OnOff::Attributes::OnOff::Id;

    node_t *node = node::get();
    endpoint_t *endpoint = endpoint::get(node, endpoint_id);
    cluster_t *cluster = cluster::get(endpoint, cluster_id);
    attribute_t *attribute = attribute::get(cluster, attribute_id);

    esp_matter_attr_val_t val = esp_matter_invalid(NULL);
    attribute::get_val(attribute, &val);
    val.val.b = !val.val.b;
    attribute::update(endpoint_id, cluster_id, attribute_id, &val);
}

/**
 * Update driver atttributes
 */
esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val)
{

    ESP_LOGI(TAG, "===== Update Temp sensor driver attributes ==== \n endpoint ID: %u, Cluster ID: %lu, Attribute ID: %lu \n", endpoint_id, cluster_id, attribute_id);

    esp_err_t err = ESP_OK;
    if (endpoint_id == temperature_sensor_endpoint_id)
    {
        ESP_LOGI(TAG, "===== Inside Update Temperature function #2 ==== \n");

        ESP_LOGI(TAG, "===== Read and set Temperature value ==== \n");
        err = app_driver_read_temperature(endpoint_id);

        // led_driver_handle_t handle = (led_driver_handle_t)driver_handle;
        // if (cluster_id == OnOff::Id)
        // {
        //     if (attribute_id == OnOff::Attributes::OnOff::Id)
        //     {
        //         ESP_LOGI(TAG, "===== Update Power control ==== \n");
        //         err = app_driver_light_set_power(handle, val);
        //     }
        // }
        // else if (cluster_id == LevelControl::Id)
        // {
        //     if (attribute_id == LevelControl::Attributes::CurrentLevel::Id)
        //     {
        //         ESP_LOGI(TAG, "===== Update Level control ==== \n");
        //         err = app_driver_rgb_set_brightness(handle, val);
        //     }
        // }
        // else if (cluster_id == ColorControl::Id)
        // {
        //     ESP_LOGI(TAG, "===== Update Color control ==== \n");

        //     if (attribute_id == ColorControl::Attributes::CurrentHue::Id)
        //     {
        //         ESP_LOGI(TAG, "===== Update Hue control ==== \n");
        //         err = app_driver_rgb_set_hue(handle, val);
        //     }
        //     else if (attribute_id == ColorControl::Attributes::CurrentSaturation::Id)
        //     {
        //         ESP_LOGI(TAG, "===== Update Saturation control ==== \n");
        //         err = app_driver_light_set_saturation(handle, val);
        //     }
        //     else if (attribute_id == ColorControl::Attributes::ColorTemperatureMireds::Id)
        //     {
        //         ESP_LOGI(TAG, "===== Update Temperature control ==== \n");
        //         // ESP_LOGI(TAG, "===== Update Temperature control ==== \nVal: %d", val->val.i);
        //         err = app_driver_rgb_set_temperature(handle, val);
        //     }
        // }
    }
    return err;
}

int16_t app_driver_read_temperature(uint16_t endpoint_id)
{
    return (u_int16_t)getTemperature();
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

    // gpio_config_t io_conf = {
    //     .pin_bit_mask = (1ULL << DHT22_GPIO_PIN),
    //     .mode = GPIO_MODE_OUTPUT,
    //     .pull_up_en = GPIO_PULLUP_DISABLE,
    //     .pull_down_en = GPIO_PULLDOWN_DISABLE,
    //     .intr_type = GPIO_INTR_DISABLE,
    // };
    // gpio_config(&io_conf);

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
