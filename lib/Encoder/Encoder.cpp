
#include "Encoder.h"

#define ENC_A1_PIN GPIO_NUM_18  // Pino A do encoder (confirmar)
#define ENC_B1_PIN GPIO_NUM_19  // Pino B do encoder (confirmar)

#define ENC_A2_PIN GPIO_NUM_0  // Pino A do encoder (confirmar)
#define ENC_B2_PIN GPIO_NUM_1  // Pino B do encoder (confirmar)

volatile int count1 = 0, count2 = 0;  // Variável para armazenar a contagem do encoder

static const char* TAG = "Encoder";

/**
 * Handles the interrupt for the encoder1.
 *
 * @throws None
 */
void IRAM_ATTR encoderInterrupt1() 
{
  static int8_t encoderState[16] = {0, -1, +1, 0, +1, 0, 0, -1, -1, 0, 0, +1, 0, +1, -1, 0};
  static uint8_t old_AB1 = 0;
  uint8_t new_AB1 = 0;

  new_AB1 = (digitalRead(ENC_A1_PIN) << 1) | digitalRead(ENC_B1_PIN);
  old_AB1 <<= 2;
  old_AB1 |= (new_AB1 & 0x03);
  count1 += encoderState[(old_AB1 & 0x0f)];
}

/**
 * Handles the interrupt for encoder2.
 *
 * @throws None
 */
void IRAM_ATTR encoderInterrupt2() 
{
  static int8_t encoderState[16] = {0, -1, +1, 0, +1, 0, 0, -1, -1, 0, 0, +1, 0, +1, -1, 0};
  static uint8_t old_AB2 = 0;
  uint8_t new_AB2 = 0;

  new_AB2 = (digitalRead(ENC_A2_PIN) << 1) | digitalRead(ENC_B2_PIN);
  old_AB2 <<= 2;
  old_AB2 |= (new_AB2 & 0x03);
  count2 += encoderState[(old_AB2 & 0x0f)];
}

/**
 * Initializes the encoder pins and interrupts.
 *
 * @return void
 *
 * @throws None
 */
void EncoderInit(void)
{
    ESP_LOGI(TAG, "Encoder Init");

    // Inicializa os e a  pinos do encoder 1
    pinMode(ENC_A1_PIN, INPUT);
    pinMode(ENC_B1_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(ENC_A1_PIN), encoderInterrupt1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC_B1_PIN), encoderInterrupt1, CHANGE);

    // Inicializa os pinos do encoder 2
    pinMode(ENC_A2_PIN, INPUT);
    pinMode(ENC_B2_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(ENC_A2_PIN), encoderInterrupt2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC_B2_PIN), encoderInterrupt2, CHANGE);

}

uint32_t EncoderGetPosition(uint8_t channel)
{
    if (channel == 1U)
    {
        return count1;
    }
    else
    {
        return count2;
    }
}

void EncoderResetPosition(uint8_t channel)
{
    if (channel == 1U)
    {
        count1 = 0U;
    }
    else
    {
        count2 = 0U;
    }
}