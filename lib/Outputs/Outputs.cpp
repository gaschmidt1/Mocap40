
#include "Outputs.h"

static const char* TAG = "Outputs";

void TaskOutputs (void * pvParameters);

SemaphoreHandle_t OutputsSemaphore;
extern SemaphoreHandle_t I2cBusSemaphore;

TaskHandle_t TaskOutputsHandle;

void OutputsInit(void)
{
    ESP_LOGI(TAG, "Outputs Init");
    OutputsSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(OutputsSemaphore);
    xTaskCreate(TaskOutputs, "TaskOutputs", 10000, NULL, 2, &TaskOutputsHandle);

}

void TaskOutputs (void * pvParameters)
{
    ESP_LOGI(TAG, "Outputs Task");
    while(true)
    {
        if((xSemaphoreTake(OutputsSemaphore, portMAX_DELAY) == pdTRUE) 
        && (xSemaphoreTake(I2cBusSemaphore, portMAX_DELAY) == pdTRUE))
        {
            // do something
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        xSemaphoreGive(OutputsSemaphore);
        xSemaphoreGive(I2cBusSemaphore);
    }
}