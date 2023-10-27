
// todo : ver a necessidade de verificar o barramento spi via semaphore?

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_log.h>

#include "I2cScan.h"
#include "Analog.h"
#include "Display.h"
#include "Encoder.h"
#include "GPS.h"
#include "Inputs.h"
#include "Mixed.h"
#include "Outputs.h"

static const char* TAG = "Main";

/**
 * Initializes the setup for the program.
 *
 * @param void
 *
 * @return void
 *
 * @throws ErrorType N/A
 */
void setup(void) 
{
  Serial.begin(115200);

  I2cScan();
  AnalogInit();
  DisplayInit();
  EncoderInit();
  MixedInit();
  InputsInit();
  GPSInit();
  OutputsInit();
}

/**
 * A description of the entire C++ function.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
void loop(void) 
{
  // put your main code here, to run repeatedly:
}

