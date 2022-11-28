#ifndef KERNEL_H
    #define KERNEL_H
    #include <zephyr/kernel.h>
#endif

#ifndef LOG_H
    #define LOG_H
    #include <zephyr/logging/log.h>
#endif

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>

const struct device *get_bmp280_device(void);
int32_t *get_sensor_readings(struct device *dev);
