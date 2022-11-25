#ifndef KERNEL_H
    #define KERNEL_H
    #include <zephyr/kernel.h>
#endif

#ifndef LOG_H
    #define LOG_H
    #include <zephyr/logging/log.h>
#endif

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>

#ifndef AUTOCONF_H
    #define AUTOCONF_H
    #include <autoconf.h>
#endif

#define BT_UUID_WEATHER_SERVICE_VAL \
    BT_UUID_128_ENCODE(0x2c2a0001, 0x7212, 0x443e, 0x8b37, 0xc0ab864d7c26)
#define BT_UUID_WEATHER_SERVICE BT_UUID_DECLARE_128(BT_UUID_WEATHER_SERVICE_VAL)

#define BT_UUID_TEMP_CHRC_VAL \
    BT_UUID_128_ENCODE(0x2c2a0002, 0x7212, 0x443e, 0x8b37, 0xc0ab864d7c26)
#define BT_UUID_TEMP_CHRC BT_UUID_DECLARE_128(BT_UUID_TEMP_CHRC_VAL)

#define BT_UUID_PRESS_CHRC_VAL \
    BT_UUID_128_ENCODE(0x2c2a0003, 0x7212, 0x443e, 0x8b37, 0xc0ab864d7c26)
#define BT_UUID_PRESS_CHRC BT_UUID_DECLARE_128(BT_UUID_PRESS_CHRC_VAL)

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME)-1)

enum notification_type {
    TEMP,
    PRESS
};

int update(enum notification_type type, int16_t value);

int start_bluetooth(void);