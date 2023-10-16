
#ifndef _I2CSCAN_H
    #define _I2CSCAN_H

    #include <Arduino.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <freertos/semphr.h>
    #include <esp_log.h>

    uint8_t I2cScan(void);

#endif