
#include "GPS.h"
#include "TinyGPS.h"

static const char* TAG = "GPS";

TinyGPS gps;

void TaskGPS (void * pvParameters);

SemaphoreHandle_t GPSSemaphore;
TaskHandle_t TaskGPSHandle;

/**
 * Initializes the GPS module.
 *
 * @return void
 *
 * @throws None
 */
void GPSInit(void)
{
    ESP_LOGI(TAG, "GPS Init");

    GPSSemaphore = xSemaphoreCreateBinary();

    xSemaphoreGive(GPSSemaphore); 

    xTaskCreate(TaskGPS, "TaskGPS", 10000, NULL, 2, &TaskGPSHandle);
}

/**
 * TaskGPS function performs GPS-related operations.
 *
 * @param pvParameters a pointer to task parameters (not used in this function)
 *
 * @return void
 *
 * @throws None
 */
void TaskGPS (void * pvParameters)
{
    ESP_LOGI(TAG, "GPS Task");
    while(true)
    {
        if(xSemaphoreTake(GPSSemaphore, portMAX_DELAY) == pdTRUE)
        {
            // do something
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            long latitude, longitude;
            gps.get_position(&latitude, &longitude, NULL);
            ESP_LOGI(TAG, "Latitude:  %d, Longitude:  %d", latitude, longitude);
            gps.
        }
        xSemaphoreGive(GPSSemaphore);
    }
}