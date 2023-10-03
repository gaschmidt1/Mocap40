
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

Adafruit_MCP23X08 mcp[ MaxI2cBus];

static const char* TAG = "Mixed";

uint8_t TotalDeviceNumber = 0;

void TaskMixed (void * pvParameters);

SemaphoreHandle_t MixedSemaphore;
extern SemaphoreHandle_t I2cBusSemaphore;

TaskHandle_t TaskMixedHandle;

uint8_t MixedDigitalRead(uint8_t pin)
{
    if(pin < 3)
    {
        return mcp[0].digitalRead(pin - 0);
    }
    else if(pin < 6)
    {
        return mcp[1].digitalRead(pin - 3);
    }
    else if(pin < 9)
    {
        return mcp[2].digitalRead(pin - 6);
    }
    else if(pin < 12)
    {
        return mcp[3].digitalRead(pin - 9);
    }
    else if(pin < 15)
    {
        return mcp[4].digitalRead(pin - 12);
    }
    else if(pin < 18)
    {
        return mcp[5].digitalRead(pin - 15);
    }
    else if(pin < 21)
    {
        return mcp[6].digitalRead(pin - 18);
    }
    else if(pin < 24)
    {
        return mcp[7].digitalRead(pin - 21);
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
        mcp[0].digitalWrite(pin - 0 + 3, value);
    }
    else if(pin < 6)
    {
        mcp[1].digitalWrite(pin - 3 + 3, value);
    }
    else if(pin < 9)
    {
        mcp[2].digitalWrite(pin - 6 + 3, value);
    }
    else if(pin < 12)
    {
        mcp[3].digitalWrite(pin - 9 + 3, value);
    }
    else if(pin < 15)
    {
        mcp[4].digitalWrite(pin - 12 + 3, value);
    }
    else if(pin < 18)
    {
        mcp[5].digitalWrite(pin - 15 + 3, value);
    }
    else if(pin < 21)
    {
        mcp[6].digitalWrite(pin - 18 + 3, value);
    }
    else if(pin < 24)
    {
        mcp[7].digitalWrite(pin - 21 + 3, value);
    }
}


void MixedI2cScan(void)
{
ESP_LOGI(TAG, "Mixed I2c Scan");   

    for (uint8_t i = 0; i < MaxI2cBus; i++)
    {
        Wire.beginTransmission(i + BaseMixedDeviceBoardId);
        if (Wire.endTransmission() == 0)
        {
            ESP_LOGI(TAG, "Mixed I2c Found: %d", i + BaseMixedDeviceBoardId);    

            Wire.begin(i + BaseMixedDeviceBoardId, SDA_PIN, SCL_PIN, I2C_SPEED);        
            
            mcp.begin_I2C( i + BaseMixedDeviceBoardId, &Wire);

            mcp[i].pinMode(MixedOut0, OUTPUT);
            mcp[i].pinMode(MixedOut1, OUTPUT);
            mcp[i].pinMode(MixedOut2, OUTPUT);

            mcp[i].pinMode(MixedIn0, INPUT);
            mcp[i].pinMode(MixedIn1, INPUT);
            mcp[i].pinMode(MixedIn2, INPUT);

            mcp[i].pinMode(MixedConfig0, INPUT);
            mcp[i].pinMode(MixedConfig1, INPUT);

            if((mcp[1].digitalRead(MixedConfig0) == false) && (mcp[1].digitalRead(MixedConfig1) == True)) // todo : ver se os endereçamentos estão corretos
            {
                MixedAdressVector[TotalDeviceNumber] = i + BaseMixedDeviceBoardId;
            }
            
            TotalDeviceNumber++;
        }
    }
}

void MixedInit(void)
{
    ESP_LOGI(TAG, "Mixed Init");

    MixedSemaphore = xSemaphoreCreateBinary();

    xSemaphoreGive(MixedSemaphore);

    xTaskCreate(TaskMixed, "TaskMixed", 10000, NULL, 2, &TaskMixedHandle);
}

void TaskMixed (void * pvParameters)
{
    ESP_LOGI(TAG, "Mixed Task");

    // Nesse trecho pode ser necessario varrer todos os endereços do barramento I2C e certificar se é o hardware correto (6 e 7)

    //Wire.begin(BaseMixedDeviceBoardId, SDA_PIN, SCL_PIN, I2C_SPEED);

    //mcp.begin_I2C(BaseMixedDeviceBoardId, &Wire);

    //if (!mcp.begin_I2C()) 
    //{
    //    Serial.println("Error initializing MCP23xxx device.");
    //    while (1);
    //}

    //mcp.pinMode(MixedOut0, OUTPUT);

    //mcp.pinMode(MixedIn0, INPUT_PULLUP);

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

