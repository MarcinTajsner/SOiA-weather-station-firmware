#include "bluetooth.h"

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)
};

static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_REMOTE_SERVICE_VAL)
};

static struct bt_conn *current_conn;

static K_SEM_DEFINE(bt_init_ok, 0, 1)

void on_connected(struct bt_conn *conn, uint8_t err);
void on_disconnected(struct bt_conn *conn, uint8_t reason);

struct bt_conn_cb bt_callbacks = {
	.connected = on_connected,
	.disconnected = on_disconnected
};

BT_GATT_SERVICE_DEFINE(remote_srv, BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE));

void on_connected(struct bt_conn *conn, uint8_t err) {
	if(err) {
		printk("ERROR: Connection error: %d", err);
		return;
	}
	printk("INFO: Connected");
	current_conn = bt_conn_ref(conn);
}

void on_disconnected(struct bt_conn *conn, uint8_t reason) {
	printk("INFO: Disconnected (reason: %d)", reason);
	if(current_conn) {
		bt_conn_unref(current_conn);
		current_conn = NULL;
	}
}

void bt_ready(int err) {
    if (err) {
        printk("ERROR: bt_enable returned: %d", err);
    }
    k_sem_give(&bt_init_ok);
}

int bt_init(struct bt_conn_cb *bt_cb) {
    int err;
    bt_conn_cb_register(bt_cb);
    err = bt_enable(bt_ready);
    if (err) {
        return err;
    }
    k_sem_take(&bt_init_ok, K_FOREVER);

    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err) {
        printk("ERROR: Couldn't start advertising, error: %d", err);
        return err;
    }

    return 0;
}

void start_bluetooth(void) {
    int err;
    err = bt_init(&bt_callbacks);
    if (err) {
        printk("ERROR: Couldn't start bluetooth, error: %d", err);
    }
}