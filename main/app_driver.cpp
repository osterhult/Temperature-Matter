/*
 * app_driver.cpp
 *
 *  Created on: 2021. 4. 1.
 */

/* Includes ------------------------------------------------------------------*/
#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include <device.h>
#include <driver/gpio.h>
#include <cstdint>
#include <cmath>

#include <app_priv.h>

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_matter.h>
#include <DHT22X.h>

/* Constants -----------------------------------------------------------------*/
using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;

static const char *TAG = "app_driver";
extern uint16_t temperature_sensor_endpoint_id;
extern uint16_t humidity_sensor_endpoint_id;

/**
 * Update Matter values with temperature and humidity
 */
void updateMatterWithValues()
{
    // Update temperature values
    esp_matter_attr_val_t temperature_value;
    temperature_value = esp_matter_invalid(NULL);
    temperature_value.type = esp_matter_val_type_t::ESP_MATTER_VAL_TYPE_UINT16;
    temperature_value.val.u16 = app_driver_read_temperature(temperature_sensor_endpoint_id);
    esp_matter::attribute::update(temperature_sensor_endpoint_id, TemperatureMeasurement::Id, TemperatureMeasurement::Attributes::MeasuredValue::Id, &temperature_value);

    // Update humidity values
    esp_matter_attr_val_t humidity_value;
    humidity_value = esp_matter_invalid(NULL);
    humidity_value.type = esp_matter_val_type_t::ESP_MATTER_VAL_TYPE_UINT16;
    humidity_value.val.u16 = app_driver_read_humidity(humidity_sensor_endpoint_id);
    esp_matter::attribute::update(humidity_sensor_endpoint_id, RelativeHumidityMeasurement::Id, RelativeHumidityMeasurement::Attributes::MeasuredValue::Id, &humidity_value);
}

/**
 * Functions to handle a button to toggle the light
 */
static void app_driver_button_toggle_cb(void *arg, void *data)
{

    ESP_LOGI(TAG, "Toggle button pressed");

    updateMatterWithValues();
}

uint16_t app_driver_read_temperature(uint16_t endpoint_id)
{
    return (u_int16_t)getTemperature();
}

uint16_t app_driver_read_humidity(uint16_t endpoint_id)
{
    return (u_int16_t)getHumidity();
}

// Example callback for temperature attribute change
esp_err_t temperature_attribute_update_cb(attribute::callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data)
{
    esp_err_t err = ESP_OK;

    if (type == POST_UPDATE)
    {
        ESP_LOGI(TAG, "Temperature attribute updated to %d", val->val.i16);
        // Add your logic here to use or display the temperature value
    }

    return err;
}

// // Example callback for humidity attribute change
esp_err_t humidity_attribute_update_cb(attribute::callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data)
{
    if (type == POST_UPDATE)
    {
        ESP_LOGI(TAG, "Humidity attribute updated to %d", val->val.i16);
        // Add your logic here to use or display the humidity value
    }
    return ESP_OK;
}

esp_err_t sensor_attribute_update_cb(attribute::callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data)
{
    // if (type == POST_UPDATE)
    if (type == PRE_UPDATE)
    {
        if (cluster_id == TemperatureMeasurement::Id)
        {
            ESP_LOGI(TAG, "Temperature attribute updated to %d", val->val.i16);
        }
        else if (cluster_id == RelativeHumidityMeasurement::Id)
        {
            ESP_LOGI(TAG, "Humidity attribute updated to %d", val->val.i16);
        }

        // Add your logic here to use or display the humidity value
        // updateMatterWithValues(); // gives kerlnel panic :(
    }
    return ESP_OK;
}

/**
 * DHT22 Sensor task
 */
static void DHT22_task(void *pvParameter)
{
    // setDHTgpio(DHT_GPIO);
    printf("Starting DHT task\n\n");

    for (;;)
    {
        printf("=== Reading DHT ===\n");
        int ret = dht_read_float_data();

        errorHandler(ret);

        // printf("Hum %.1f\n", getHumidity());
        // printf("Tmp %.1f\n", getTemperature());

        // TODO: Update Matter values
        updateMatterWithValues();

        // Wait at least 30 seconds before reading again
        // The interval of the whole process must be more than 30 seconds
        vTaskDelay(DEFAULT_MEASURE_INTERVAL / portTICK_PERIOD_MS);
    }
}

/**
 * Initialize DHT22 sensor
 */
app_driver_handle_t app_driver_DHT_sensor_init()
{
    // Start DHT22 sensor task
    xTaskCreatePinnedToCore(&DHT22_task, "DHT22_task", DHT22_TASK_STACK_SIZE, NULL, DHT22_TASK_PRIORITY, NULL, DHT22_TASK_CORE_ID);

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
