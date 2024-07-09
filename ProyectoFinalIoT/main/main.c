#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <string.h>

#include "NTP.h"
#include "APSTAmodule.h"
#include "webserver.h"


#define TAG "main"

#define QUEUE_LENGTH 30
QueueHandle_t command_queue;


/*
typedef struct eventos {
    EVENT_PLAY,
    EVENT_PAUSE,
    EVENT_NEXT,
    EVENT_PREVIOUS,
    EVENT_STOP,
    EVENT_VOLUMEN_UP,
    EVENT_VOLUMEN_DOWN
} event_t;

*/


/*

// Función que envía a la queue
void envio_queue(event_t event_received){
    BaseType_t xStatus;

    // Intentar enviar a la cola
    xStatus = xQueueSendToBack(command_queue, &event_received, (TickType_t) 0);
    if (xStatus != pdPASS) {
        ESP_LOGE(TAG, "No se pudo enviar el evento a la cola");
        return;
    }
    ESP_LOGI(TAG, "Evento enviado a la cola");

    // Recibir de la cola
    event_t *event_saved_queue;
    event_saved_queue = (event_t*) malloc(sizeof(event_t));
    xStatus = xQueueReceive(command_queue, &event_saved_queue, pdMS_TO_TICKS(500));
    if (xStatus == pdPASS) {
        ESP_LOGI(TAG, "Evento recibido de la cola");
    } else {
        ESP_LOGE(TAG, "No se pudo recibir el evento de la cola");
    }
}

*/





void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    command_queue = xQueueCreate(QUEUE_LENGTH, sizeof(char*));

    wifi_init(command_queue);



   // inicializacionAudio();


}
