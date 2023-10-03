
// todo : verirficar canais 7 e 8 do MCP3208 para garantir a integridade do barramento 

#include "Analog.h"
#include <pins_arduino.h>
#include <MCP3208.h>
#include <SPI.h>

#define RESOLUTION_MCP320X 12   // Resolution of MCP320X
#define SPI_SPEED 100000        // SPI speed
#define SCK_PIN 3
#define MISO_PIN 2
#define MOSI_PIN 4
#define CS1_PIN 10
#define CS2_PIN 9
#define CS3_PIN 8

SPIClass spi;
MCP3208 adc;

uint16_t Channel[3][8];
uint8_t CsPin[3] = {CS1_PIN, CS2_PIN, CS3_PIN};

static const char* TAG = "Analog";

void TaskAnalog (void * pvParameters);

SemaphoreHandle_t AnalogSemaphore;
TaskHandle_t TaskAnalogHandle;

void AnalogInit(void)
{
    ESP_LOGI(TAG, "Analog Init");

    AnalogSemaphore = xSemaphoreCreateBinary();

    xSemaphoreGive(AnalogSemaphore);

    xTaskCreate(TaskAnalog, "TaskAnalog", 10000, NULL, 2, &TaskAnalogHandle);
}

void TaskAnalog (void * pvParameters)
{
    ESP_LOGI(TAG, "Analog Task");
    


    while(true)
    {
        if(xSemaphoreTake(AnalogSemaphore, portMAX_DELAY) == pdTRUE)
        {
            for(int i = 0; i < 3; i++)
            {
                spi.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CsPin[i]);
                spi.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
                adc.begin(CsPin[i], spi);
                adc.analogReadResolution(RESOLUTION_MCP320X);

                for(int j = 0; j < 8; j++)
                {
                    Channel[i][j] = adc.analogRead(j);
                    vTaskDelay(50 / portTICK_PERIOD_MS);
                }

                spi.endTransaction();
                spi.end();
            }
            // do something
            vTaskDelay(10  / portTICK_PERIOD_MS);
        }
        xSemaphoreGive(AnalogSemaphore);
    }
}
