
#include "Inputs.h"

static const char* TAG = "Inputs";

void TaskInputs (void * pvParameters);

SemaphoreHandle_t InputsSemaphore; // pode ser necessario usar os outros semaphores do barramento i2c
extern SemaphoreHandle_t I2cBusSemaphore;

TaskHandle_t TaskInputsHandle;

void InputsInit(void)
{
    ESP_LOGI(TAG, "Inputs Init");

    InputsSemaphore = xSemaphoreCreateBinary();

    xSemaphoreGive(InputsSemaphore); 

    xTaskCreate(TaskInputs, "TaskInputs", 10000, NULL, 2, &TaskInputsHandle);
}

void TaskInputs (void * pvParameters)
{
    ESP_LOGI(TAG, "Inputs Task");
    while(true)
    {
        if((xSemaphoreTake(InputsSemaphore, portMAX_DELAY) == pdTRUE)
        && (xSemaphoreTake(I2cBusSemaphore, portMAX_DELAY) == pdTRUE))
        {
            // do something
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        xSemaphoreGive(InputsSemaphore); 
        xSemaphoreGive(I2cBusSemaphore);
    }
}