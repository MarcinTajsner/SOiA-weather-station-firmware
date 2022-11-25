#include "bmp280.h"

#define LOG_MODULE_NAME bmp280
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

const struct device *get_bmp280_device(void) {
	const struct device *dev = DEVICE_DT_GET_ANY(bosch_bme280); //Intellisense error?
	if (dev == NULL) {
		LOG_ERR("No BMP280 device found", NULL);
		return NULL;
	}
	if (!device_is_ready(dev)) {
		LOG_ERR("Device \"%s\" is not ready", dev->name);
		return NULL;
	}

	LOG_INF("Found device \"%s\", getting sensor data", dev->name);
	return dev;
}