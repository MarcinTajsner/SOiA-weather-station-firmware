#ifndef KERNEL_H
    #define KERNEL_H
    #include <zephyr/kernel.h>
#endif

#ifndef LOG_H
    #define LOG_H
    #include <zephyr/logging/log.h>
#endif

#include "bmp280.h"
#include "bluetooth.h"

#define LOG_MODULE_NAME app
#define SLEEP_TIME 3000

LOG_MODULE_REGISTER(LOG_MODULE_NAME);

void main(void) {
    const struct device *bmp280 = get_bmp280_device();
    int32_t *readings;

    LOG_INF("Starting bluetooth...", NULL);
    start_bluetooth();
    LOG_INF("Bluetooth started", NULL);

    while (true) {
        k_sleep(K_MSEC(SLEEP_TIME));
        readings = get_sensor_readings(bmp280);
        update(TEMP, readings[0]);
        update(PRESS, readings[1]);
    }
    
}