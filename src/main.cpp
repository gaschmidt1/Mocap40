
// todo : ver a necessidade de verificar o barramento spi via semaphore?

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_log.h>

#include "Analog.h"
#include "Display.h"
#include "Encoder.h"
#include "GPS.h"
#include "Inputs.h"
#include "Mixed.h"
#include "Outputs.h"

// I2C Bus Semaphore 
SemaphoreHandle_t I2cBusSemaphore;

static const char* TAG = "Main";


void setup() 
{
  Serial.begin(115200);

  I2cBusSemaphore = xSemaphoreCreateBinary();

  xSemaphoreGive(I2cBusSemaphore); 

  AnalogInit();
  DisplayInit();
  EncoderInit();
  MixedInit();
  InputsInit();
  GPSInit();
  OutputsInit();

}

void loop() 
{
  // put your main code here, to run repeatedly:
}

