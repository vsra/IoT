#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
//#include "esp_spi_flash.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "APSTAmodule.h"

#define TAG "main"
<<<<<<< Updated upstream
void app_main(void)
{
=======

#define QUEUE_LENGTH 30
//QueueHandle_t command_queue;



typedef enum {
    EVENT_PLAY = 0,
    EVENT_PAUSE,
    EVENT_NEXT,
    EVENT_PREVIOUS,
    EVENT_STOP,
    EVENT_VOLUMEN_UP,
    EVENT_VOLUMEN_DOWN
} event_t;


void app_main(void) {
>>>>>>> Stashed changes
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

<<<<<<< Updated upstream
    ESP_LOGI(TAG, "Hello world!");
    init_network();
    //init_logger();
   // init_player();
=======
  //  command_queue = xQueueCreate(QUEUE_LENGTH, sizeof(event_t));

    wifi_init();





   // inicializacionAudio();


>>>>>>> Stashed changes
}
