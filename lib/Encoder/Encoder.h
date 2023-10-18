
#ifndef _ENCODER_H
    #define _ENCODER_H

    #include <Arduino.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <freertos/semphr.h>
    #include <esp_log.h>

    void EncoderInit(void);
    uint32_t EncoderGetPosition(uint8_t channel);

#endif