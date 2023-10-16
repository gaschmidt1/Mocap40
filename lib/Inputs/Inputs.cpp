
#include "Inputs.h"
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_MCP23X08.h>
#include <Wire.h>
#include <pins_arduino.h>

#define SDA_PIN 7
#define SCL_PIN 6 
#define I2C_SPEED 100000

#define MaxI2cBus 8
#define BaseInputsDeviceBoardId 0x20
#define MaxInputsDeviceBoardId  0x27

#define MaxInputsImputs 24
#define MaxInputsOutputs 24

#define InputsIn0 0
#define InputsIn1 1
#define InputsIn2 2

#define InputsIn3 3
#define InputsIn4 4
#define InputsIn5 5

#define InputsConfig0 6
#define InputsConfig1 7

uint8_t InputsAdressVector[ MaxI2cBus];

Adafruit_MCP23X08 input[ MaxI2cBus];

static const char* TAG = "Inputs";

void TaskInputs (void * pvParameters);

SemaphoreHandle_t InputsSemaphore; // pode ser necessario usar os outros semaphores do barramento i2c
extern SemaphoreHandle_t I2cBusSemaphore;

TaskHandle_t TaskInputsHandle;

/**
 * Reads the digital value of a specified pin.
 *
 * @param pin the pin number to read from
 *
 * @return the digital value of the pin (0 or 1)
 *
 * @throws None
 */
uint8_t InputsDigitalRead(uint8_t pin)
{
    if(pin < 8)
    {
        return input[0].digitalRead(pin);
    }
    else if(pin < 16)
    {
        return input[1].digitalRead(pin);
    }
    else if(pin < 24)
    {
        return input[2].digitalRead(pin);
    }
    else if(pin < 32)
    {
        return input[3].digitalRead(pin);
    }
    else if(pin < 40)
    {
        return input[4].digitalRead(pin);
    }
    else if(pin < 48)
    {
        return input[5].digitalRead(pin);
    }
    else if(pin < 56)
    {
        return input[6].digitalRead(pin);
    }
    else if(pin < 64)
    {
        return input[7].digitalRead(pin);
    }
    else
    {
        return(255);
    }
}

/**
 * Scans the I2C bus for connected devices and initializes them.
 *
 * @return The total number of devices found on the I2C bus.
 *
 * @throws None
 */
uint8_t InputsI2cScan(void)
{
ESP_LOGI(TAG, "Inputs I2c Scan");   

    uint8_t TotalDeviceNumber = 0;
    Wire.begin(SDA_PIN, SCL_PIN, I2C_SPEED);
    for (uint8_t i = 0; i < MaxI2cBus; i++)
    {
        
        Wire.beginTransmission(i + BaseInputsDeviceBoardId);

        if (Wire.endTransmission() == 0)
        {
            ESP_LOGI(TAG, "Inputs I2c Found: %d", i + BaseInputsDeviceBoardId);    
                                
            // todo : verificar se pde disparar este sem registrar o endereço na library caso contrario fazer essa rotina de descoberta d forma
            // global e armazenando em vetor o endereço e o tipo
            input[i].begin_I2C( i + BaseInputsDeviceBoardId, &Wire); 

            input[i].pinMode(InputsIn0, INPUT);
            input[i].pinMode(InputsIn1, INPUT);
            input[i].pinMode(InputsIn2, INPUT);
            input[i].pinMode(InputsIn3, INPUT);
            input[i].pinMode(InputsIn4, INPUT);
            input[i].pinMode(InputsIn5, INPUT);

            input[i].pinMode(InputsConfig0, INPUT);
            input[i].pinMode(InputsConfig1, INPUT);

            if((input[i].digitalRead(InputsConfig0) == false) && (input[i].digitalRead(InputsConfig1) == true)) // todo : ver se os endereçamentos estão corretos
            {
                InputsAdressVector[TotalDeviceNumber++] = i + BaseInputsDeviceBoardId; //talvez tenha que usar isso na hora de endereçar
            }                        
        }
    }
    return TotalDeviceNumber;
}

/**
 * Initializes the inputs for the system.
 *
 * @throws None
 */
void InputsInit(void)
{
    ESP_LOGI(TAG, "Inputs Init");

    if(xSemaphoreTake(I2cBusSemaphore, portMAX_DELAY) == pdTRUE)
    {
        if(InputsI2cScan() > 0)
        {
            InputsSemaphore = xSemaphoreCreateBinary();
            xSemaphoreGive(InputsSemaphore);
            xTaskCreate(TaskInputs, "InputsMixed", 10000, NULL, 2, &TaskInputsHandle);
        }
        else
        {
            ESP_LOGI(TAG, "Inputs I2c Scan Failed. No Inputs boards found");
        }
    }
    xSemaphoreGive(I2cBusSemaphore);
    InputsSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(InputsSemaphore); 


}

/**
 * TaskInputs function is responsible for handling inputs.
 *
 * @param pvParameters a pointer to the parameters of the task
 *
 * @return void
 *
 * @throws None
 */
void TaskInputs (void * pvParameters)
{
    ESP_LOGI(TAG, "Inputs Task");
    while(true)
    {
        if((xSemaphoreTake(InputsSemaphore, portMAX_DELAY) == pdTRUE)
        && (xSemaphoreTake(I2cBusSemaphore, portMAX_DELAY) == pdTRUE))
        {
            // todo aqui fica acompanhando o estad das entradas
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        xSemaphoreGive(InputsSemaphore); 
        xSemaphoreGive(I2cBusSemaphore);
    }
}