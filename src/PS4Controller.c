#include "PS4Controller.h"
#include "ps4_controller_buttons.h"
#include "ps4_controller_status.h"
#include "ps4.h"


#include <string.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"

#define TAG    __FUNCTION__

//#define ESP_BD_ADDR_HEX_PTR(addr) (uint8_t*)addr + 0, (uint8_t*)addr + 1, (uint8_t*)addr + 2, (uint8_t*)addr + 3, (uint8_t*)addr + 4, (uint8_t*)addr + 5

#define ESP_BD_ADDR_HEX_STR "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx"
#define ESP_BD_ADDR_HEX_ARR(addr)   addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]
#define ESP_BD_ADDR_HEX_PTR(addr)  &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5]


static void _event_cb(void* object, ps4_t data, ps4_event_t event) {
    ps4_controller_t * ps4 = (ps4_controller_t *)object;

    memcpy(&ps4->data, &data, sizeof(ps4_t));
    memcpy(&ps4->event, &event, sizeof(ps4_event_t));

    if (ps4->_callback_event) {
        ps4->_callback_event();
    }
}

static void _connection_cb(void* object, uint8_t isConnected) {
    ps4_controller_t * ps4 = (ps4_controller_t *)object;

    if (isConnected) {
        /* ToDo: figure out how to know when the channel is free again
         * so this delay can be removed */
        vTaskDelay(250);

        if (ps4->_callback_connect) {
            ps4->_callback_connect();
        }
    }
    else {
        if (ps4->_callback_disconnect) {
            ps4->_callback_disconnect();
        }
    }
}

bool begin(const char* mac) {
    if (mac != NULL) {
        ESP_LOGW(TAG, "Using custom MAC address");
        esp_bd_addr_t addr;

        if (sscanf(mac, ESP_BD_ADDR_HEX_STR, ESP_BD_ADDR_HEX_PTR(addr)) != ESP_BD_ADDR_LEN) {
            ESP_LOGE(TAG, "Could not convert %s to a MAC address", mac);
            return false;
        }

        ps4SetBluetoothMacAddress(addr);
    }

    ps4SetEventObjectCallback(ctx.ps4, _event_cb);
    ps4SetConnectionObjectCallback(ctx.ps4, _connection_cb);

    esp_err_t ret;

    // Initialize NVS.
    ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

//  bluetooth_init();

    ps4Init();
    return true;
}

void end() {}

bool isConnected() { return ps4IsConnected(); }

void setLed(uint8_t r, uint8_t g, uint8_t b) {
    ctx.ps4->output.r = r;
    ctx.ps4->output.g = g;
    ctx.ps4->output.b = b;
}

void setRumble(uint8_t small, uint8_t large) {
    ctx.ps4->output.smallRumble = small;
    ctx.ps4->output.largeRumble = large;
}

void setFlashRate(uint8_t onTime, uint8_t offTime) {
    ctx.ps4->output.flashOn = onTime / 10;
    ctx.ps4->output.flashOff = offTime / 10;
}

void sendToController() { ps4SetOutput(ctx.ps4->output); }

void attach(callback_t callback) { ctx.ps4->_callback_event = callback; }

void attachOnConnect(callback_t callback) {
    ctx.ps4->_callback_connect = callback;
}

void attachOnDisconnect(callback_t callback) {
    ctx.ps4->_callback_disconnect = callback;
}

void init_callbacks() {
    ctx.ps4->_callback_event = NULL;
    ctx.ps4->_callback_connect = NULL;
    ctx.ps4->_callback_disconnect = NULL;
}