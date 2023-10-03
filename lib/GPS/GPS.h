
#ifndef _GPS_H
    #define _GPS_H

    #include <Arduino.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <freertos/semphr.h>
    #include <esp_log.h>

    void GPSInit(void);

#endif