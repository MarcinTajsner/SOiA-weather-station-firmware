#ifndef KERNEL_H
    #define KERNEL_H
    #include <zephyr/kernel.h>
#endif

#include "bluetooth.h"

#define SLEEP_TIME 1000

void main(void) {
    printk("INFO: Starting bluetooth...");
    start_bluetooth();
    printk("INFO: Bluetooth started");

    while (true) {
        k_sleep(K_MSEC(SLEEP_TIME));
    }
    
}