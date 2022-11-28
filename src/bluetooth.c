#include "bluetooth.h"

#define LOG_MODULE_NAME bluetooth
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)
};

static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_WEATHER_SERVICE_VAL)
};

static struct bt_conn *current_conn;

static K_SEM_DEFINE(bt_init_ok, 0, 1)

void on_connected(struct bt_conn *conn, uint8_t err);
void on_disconnected(struct bt_conn *conn, uint8_t reason);
void temp_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);
void press_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);

struct bt_conn_cb bt_callbacks = {
	.connected = on_connected,
	.disconnected = on_disconnected
};

static bool temp_notify;
static bool press_notify;

BT_GATT_SERVICE_DEFINE(weather_srv, 

    // ***** TEMP *****
    BT_GATT_PRIMARY_SERVICE(BT_UUID_WEATHER_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_TEMP_CHRC,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ,
        NULL, NULL, NULL
    ),
    BT_GATT_CCC(temp_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

    // ***** PRESS *****
    BT_GATT_CHARACTERISTIC(BT_UUID_PRESS_CHRC,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ,
        NULL, NULL, NULL
    ),
    BT_GATT_CCC(press_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

void temp_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value) {
    ARG_UNUSED(attr);
    if (value == BT_GATT_CCC_NOTIFY) {
        temp_notify = true;
        LOG_INF("Temperature notifications enabled", NULL);
    } 
    else {
        temp_notify = false;
        LOG_INF("Temperature notifications disabled", NULL);
    }
}

void press_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value) {
    ARG_UNUSED(attr);
    if (value == BT_GATT_CCC_NOTIFY) {
        press_notify = true;
        LOG_INF("Pressure notifications enabled", NULL);
    } 
    else {
        press_notify = false;
        LOG_INF("Pressure notifications disabled", NULL);
    }
}

void on_connected(struct bt_conn *conn, uint8_t err) {
	if(err) {
		LOG_ERR("Connection error: %d", err);
		return;
	}
	LOG_INF("Connected", NULL);
	current_conn = bt_conn_ref(conn);
}

void on_disconnected(struct bt_conn *conn, uint8_t reason) {
	LOG_INF("Disconnected (reason: %d)", reason);
	if(current_conn) {
		bt_conn_unref(current_conn);
		current_conn = NULL;
	}
}

void on_sent_temp(struct bt_conn *conn, void *user_data) {
    ARG_UNUSED(conn);
    ARG_UNUSED(user_data);
    LOG_INF("Temperature notification sent", NULL);
}

void on_sent_press(struct bt_conn *conn, void *user_data) {
    ARG_UNUSED(conn);
    ARG_UNUSED(user_data);
    LOG_INF("Pressure notification sent", NULL);
}

int send_notification(struct bt_conn *conn, int32_t value, enum notification_type type) {
    int err = 0;

    if (type == TEMP && temp_notify == false) {
        return err;
    }
    if (type == PRESS && press_notify == false) {
        return err;
    }

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr;
    if (type == TEMP) {
        attr = &weather_srv.attrs[2]; //Why 2?
        params.attr = attr;
        params.func = on_sent_temp;
    } 
    else {
        attr = &weather_srv.attrs[5]; //Why 5?
        params.attr = attr;
        params.func = on_sent_press;
    }
    params.data = &value;
    params.len = 4;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}

void bt_ready(int err) {
    if (err) {
        LOG_ERR("bt_enable returned: %d", err);
    }
    k_sem_give(&bt_init_ok);
}

int bt_init(struct bt_conn_cb *bt_cb) {
    int err = 0;

    temp_notify = false;
    press_notify = false;

    bt_conn_cb_register(bt_cb);
    err = bt_enable(bt_ready);
    if (err) {
        return err;
    }
    
    k_sem_take(&bt_init_ok, K_FOREVER);

    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err) {
        LOG_ERR("Couldn't start advertising, error: %d", err);
    }

    return err;
}

int update(enum notification_type type, int32_t value) {
    int err = 0;
    err = send_notification(current_conn, value, type);
    if (err) {
        LOG_ERR("Couldn't update, error: %d", err);
    }
    return err;
}

int start_bluetooth(void) {
    int err = 0;
    err = bt_init(&bt_callbacks);
    if (err) {
        LOG_ERR("Couldn't start bluetooth, error: %d", err);
    }
    return err;
}