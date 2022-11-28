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

int32_t *get_sensor_readings(struct device *dev) {
	struct sensor_value temp, press;
	int32_t press_frac, temp_frac;
	static int32_t readings[2] = {0, 0};
	int err;

	err = sensor_sample_fetch(dev);
	if (err) {
		LOG_ERR("Couldn't fetch sample data, error: %d", err);
		return readings;
	}
	err = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
	if (err) {
		LOG_ERR("Couldn't read temperature, error: %d", err);
		return readings;
	}
	err = sensor_channel_get(dev, SENSOR_CHAN_PRESS, &press);
	if (err) {
		LOG_ERR("Couldn't read pressure, error: %d", err);
		return readings;
	}

	//Unit conversion, result / 10 = value in degress C
	readings[0] = temp.val1;
	readings[0] *= 10;
	temp_frac = temp.val2;
	if (temp_frac >= 100000) {
		readings[0] += (temp_frac / 100000);
	}

	//Unit conversion, result / 10 = value in hPa
	readings[1] = press.val1;
	readings[1] *= 100;
	press_frac = press.val2;
	if (press_frac >= 10000) {
		readings[1] += (press_frac / 10000);
	}

	LOG_INF("Temperature: %d (result / 10 = value in degress C)", readings[0]);
	LOG_INF("Pressure: %d (result / 10 = value in hPa)", readings[1]);

	return readings;
}