#ifndef KERNEL_H
    #define KERNEL_H
    #include <zephyr/kernel.h>
#endif

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>

#ifndef AUTOCONF_H
    #define AUTOCONF_H
    #include <autoconf.h>
#endif

#define BT_UUID_REMOTE_SERVICE_VAL \
    BT_UUID_128_ENCODE(0x2c2a0001, 0x7212, 0x443e, 0x8b37, 0xc0ab864d7c26)

#define BT_UUID_REMOTE_SERVICE BT_UUID_DECLARE_128(BT_UUID_REMOTE_SERVICE_VAL)

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME)-1)

void start_bluetooth(void);