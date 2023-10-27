
#include "Display.h"
#include <LiquidCrystal_I2C.h> // acrescentei para poder usar parte do codigo i2c

static const char* TAG = "Display";

void TaskDisplay (void * pvParameters);

//Display variables control
SemaphoreHandle_t DisplaySemaphore;
extern SemaphoreHandle_t I2cBusSemaphore;

TaskHandle_t TaskDisplayHandle;

/**
 * Initializes the display.
 *
 * @return void
 *
 * @throws None
 */
void DisplayInit(void)
{
    ESP_LOGI(TAG, "Display Init");
    DisplaySemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(DisplaySemaphore); 
    xTaskCreate(TaskDisplay, "TaskDisplay", 10000, NULL, 2, &TaskDisplayHandle);
    LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
}

/**
 * TaskDisplay function is responsible for displaying something and releasing two semaphores.
 *
 * @param pvParameters void pointer to the parameters of the task
 *
 * @return void
 *
 * @throws None
 */
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