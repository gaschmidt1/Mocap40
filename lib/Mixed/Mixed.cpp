
// todo : verirficar como funciona o endereçamento nessa biblioteca (não tenho certeza de que diferentes placas não vão dar conflito de i/Os)
// Todo : preciso verirficar como a lib trata os i/os e como os numera

#include "Mixed.h"
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_MCP23X08.h>
#include <Wire.h>
#include <pins_arduino.h>

#define SDA_PIN 7
#define SCL_PIN 6 
#define I2C_SPEED 100000

#define MaxI2cBus 8
#define BaseMixedDeviceBoardId 0x20
#define MaxMixedDeviceBoardId  0x27

#define MaxMixedImputs 24
#define MaxMixedOutputs 24

#define MixedOut0 0
#define MixedOut1 1
#define MixedOut2 2

#define MixedIn0 3
#define MixedIn1 4
#define MixedIn2 5

#define MixedConfig0 6
#define MixedConfig1 7

uint8_t MixedAdressVector[ MaxI2cBus];

Adafruit_MCP23X08 mixed[ MaxI2cBus];

static const char* TAG = "Mixed";

//uint8_t TotalDeviceNumber = 0;

void TaskMixed (void * pvParameters);

SemaphoreHandle_t MixedSemaphore;
extern SemaphoreHandle_t I2cBusSemaphore;

TaskHandle_t TaskMixedHandle;

uint8_t MixedDigitalRead(uint8_t pin)
{
    if(pin < 3)
    {
        return mixed[0].digitalRead(pin - 0);
    }
    else if(pin < 6)
    {
        return mixed[1].digitalRead(pin - 3);
    }
    else if(pin < 9)
    {
        return mixed[2].digitalRead(pin - 6);
    }
    else if(pin < 12)
    {
        return mixed[3].digitalRead(pin - 9);
    }
    else if(pin < 15)
    {
        return mixed[4].digitalRead(pin - 12);
    }
    else if(pin < 18)
    {
        return mixed[5].digitalRead(pin - 15);
    }
    else if(pin < 21)
    {
        return mixed[6].digitalRead(pin - 18);
    }
    else if(pin < 24)
    {
        return mixed[7].digitalRead(pin - 21);
    }
    else
    {
        return 255;
    }
}

void MixedDigitalWrite(uint8_t pin, uint8_t value)
{
    if(pin < 3)
    {
        mixed[0].digitalWrite(pin - 0 + 3, value);
    }
    else if(pin < 6)
    {
        mixed[1].digitalWrite(pin - 3 + 3, value);
    }
    else if(pin < 9)
    {
        mixed[2].digitalWrite(pin - 6 + 3, value);
    }
    else if(pin < 12)
    {
        mixed[3].digitalWrite(pin - 9 + 3, value);
    }
    else if(pin < 15)
    {
        mixed[4].digitalWrite(pin - 12 + 3, value);
    }
    else if(pin < 18)
    {
        mixed[5].digitalWrite(pin - 15 + 3, value);
    }
    else if(pin < 21)
    {
        mixed[6].digitalWrite(pin - 18 + 3, value);
    }
    else if(pin < 24)
    {
        mixed[7].digitalWrite(pin - 21 + 3, value);
    }
}


uint8_t MixedI2cScan(void)
{
ESP_LOGI(TAG, "Mixed I2c Scan");   
    uint8_t TotalDeviceNumber = 0;    

    for (uint8_t i = 0; i < MaxI2cBus; i++)
    {
        Wire.begin(i + BaseMixedDeviceBoardId, SDA_PIN, SCL_PIN, I2C_SPEED);  
        Wire.beginTransmission(i + BaseMixedDeviceBoardId);
        if (Wire.endTransmission() == 0)
        {
            ESP_LOGI(TAG, "Mixed I2c Found: %d", i + BaseMixedDeviceBoardId);                      
            
            mixed[i].begin_I2C( i + BaseMixedDeviceBoardId, &Wire);

            mixed[i].pinMode(MixedOut0, OUTPUT);
            mixed[i].pinMode(MixedOut1, OUTPUT);
            mixed[i].pinMode(MixedOut2, OUTPUT);

            mixed[i].pinMode(MixedIn0, INPUT);
            mixed[i].pinMode(MixedIn1, INPUT);
            mixed[i].pinMode(MixedIn2, INPUT);

            mixed[i].pinMode(MixedConfig0, INPUT);
            mixed[i].pinMode(MixedConfig1, INPUT);

            if((mixed[i].digitalRead(MixedConfig0) == false) && (mixed[i].digitalRead(MixedConfig1) == true)) // todo : ver se os endereçamentos estão corretos
            {
                MixedAdressVector[TotalDeviceNumber++] = i + BaseMixedDeviceBoardId;
            }                        
        }
    }
    return TotalDeviceNumber;
}

void MixedInit(void)
{
    ESP_LOGI(TAG, "Mixed Init");
    if(xSemaphoreTake(I2cBusSemaphore, portMAX_DELAY) == pdTRUE)
    {
        if(MixedI2cScan() > 0)
        {
            MixedSemaphore = xSemaphoreCreateBinary();

            xSemaphoreGive(MixedSemaphore);

            xTaskCreate(TaskMixed, "TaskMixed", 10000, NULL, 2, &TaskMixedHandle);
        }
        else
        {
            ESP_LOGI(TAG, "Mixed I2c Scan Failed. No Mixed boards found");
        }
    }

    xSemaphoreGive(I2cBusSemaphore);

    MixedSemaphore = xSemaphoreCreateBinary();

    xSemaphoreGive(MixedSemaphore);

}

void TaskMixed (void * pvParameters)
{
    ESP_LOGI(TAG, "Mixed Task");

    Serial.println("Setup completed.");

    while(true)
    {
        if((xSemaphoreTake(MixedSemaphore, portMAX_DELAY) == pdTRUE)
        && (xSemaphoreTake(I2cBusSemaphore, portMAX_DELAY) == pdTRUE))
        {
            // todo  aqi escreve todas as saidas e todas as entradas
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        xSemaphoreGive(MixedSemaphore);
        xSemaphoreGive(I2cBusSemaphore);
    }
}

