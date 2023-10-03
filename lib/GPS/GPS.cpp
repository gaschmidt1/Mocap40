
#include "GPS.h"

static const char* TAG = "GPS";

void TaskGPS (void * pvParameters);

SemaphoreHandle_t GPSSemaphore;
TaskHandle_t TaskGPSHandle;

void GPSInit(void)
{
    ESP_LOGI(TAG, "GPS Init");

    GPSSemaphore = xSemaphoreCreateBinary();

    xSemaphoreGive(GPSSemaphore); 

    xTaskCreate(TaskGPS, "TaskGPS", 10000, NULL, 2, &TaskGPSHandle);
}

void TaskGPS (void * pvParameters)
{
    ESP_LOGI(TAG, "GPS Task");
    while(true)
    {
        if(xSemaphoreTake(GPSSemaphore, portMAX_DELAY) == pdTRUE)
        {
            // do something
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        xSemaphoreGive(GPSSemaphore);
    }
}