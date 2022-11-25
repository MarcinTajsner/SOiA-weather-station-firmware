#ifndef KERNEL_H
    #define KERNEL_H
    #include <zephyr/kernel.h>
#endif

#ifndef LOG_H
    #define LOG_H
    #include <zephyr/logging/log.h>
#endif

#include "bluetooth.h"

#define LOG_MODULE_NAME app
#define SLEEP_TIME 3000

LOG_MODULE_REGISTER(LOG_MODULE_NAME);

void main(void) {
    LOG_INF("Starting bluetooth...", NULL);
    start_bluetooth();
    LOG_INF("Bluetooth started", NULL);

    while (true) {
        k_sleep(K_MSEC(SLEEP_TIME));
        update(TEMP, 23);
        update(PRESS, 1000);
    }
    
}