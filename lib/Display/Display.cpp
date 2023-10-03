
#include "Display.h"

static const char* TAG = "Display";

void TaskDisplay (void * pvParameters);

SemaphoreHandle_t DisplaySemaphore;
extern SemaphoreHandle_t I2cBusSemaphore;

TaskHandle_t TaskDisplayHandle;

void DisplayInit(void)
{
    ESP_LOGI(TAG, "Display Init");

    DisplaySemaphore = xSemaphoreCreateBinary();

    xSemaphoreGive(DisplaySemaphore); 

    xTaskCreate(TaskDisplay, "TaskDisplay", 10000, NULL, 2, &TaskDisplayHandle);
}

void TaskDisplay (void * pvParameters)
{
    ESP_LOGI(TAG, "Display Task");
    while(true)
    {
        if((xSemaphoreTake(DisplaySemaphore, portMAX_DELAY) == pdTRUE)
        && (xSemaphoreTake(I2cBusSemaphore, portMAX_DELAY) == pdTRUE))
        {
            // do something
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        xSemaphoreGive(DisplaySemaphore);
        xSemaphoreGive(I2cBusSemaphore);
    }
}