#include <stdio.h>
#include <string.h>
#include "loggercircular.h"
#include "time.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "driver/gpio.h"
#include "lwip/err.h"
#include "lwip/apps/sntp.h"
//#include "wifiComponent.h"
#include "CustomDelay.h"
#include "NTP.h"
#include"esp_log.h"
#include "QueueComponent.h"



#define STORAGE_NAMESPACE "storage"



SemaphoreHandle_t SemaphoreNTP;


nvs_handle_t my_handle;
esp_err_t err;

esp_err_t save_circular_buffer(circular_buffer_t *buffer) {
   
    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS!\n", esp_err_to_name(err));
        return err;
    }

    // Write buffer to NVS
    err = nvs_set_blob(my_handle, "circular_buffer", buffer, sizeof(circular_buffer_t));
    if (err != ESP_OK) {
        printf("Error (%s) setting blob in NVS!\n", esp_err_to_name(err));
        nvs_close(my_handle);
        return err;
    }

    // Commit written value.
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing NVS!\n", esp_err_to_name(err));
        nvs_close(my_handle);
        return err;
    }

    // Close
    nvs_close(my_handle);
    return ESP_OK;
}


esp_err_t load_circular_buffer(circular_buffer_t *buffer) {
  
    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS!\n", esp_err_to_name(err));
        return err;
    }

    // Read buffer from NVS
    size_t required_size = sizeof(circular_buffer_t);
    err = nvs_get_blob(my_handle, "circular_buffer", buffer, &required_size);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        // If not found, initialize the buffer
        printf("Circular buffer not found in NVS, initializing...\n");
        memset(buffer, 0, sizeof(circular_buffer_t));
        err = ESP_OK;
    } else if (err != ESP_OK) {
        printf("Error (%s) reading blob from NVS!\n", esp_err_to_name(err));
    } else {
        printf("Circular buffer loaded successfully from NVS.\n");
    }

    // // Print buffer content after loading
    // printf("Buffer loaded from NVS. Current buffer state:\n");
    // for (int i = 0; i < BUFFER_SIZE; i++) {
    //     printf("Index %d: Event %d\n", i, buffer->events[i]);
    // }

    // Close
    nvs_close(my_handle);
    return err;
}


void add_event_to_buffer(circular_buffer_t *buffer, int event) {

    if (SemaphoreNTP ==NULL)
    {
        SemaphoreNTP = xSemaphoreCreateBinary();
        xSemaphoreGive(SemaphoreNTP); 

    }


        if (xSemaphoreTake(SemaphoreNTP,  pdMS_TO_TICKS(500*1000))) {
            ESP_LOGI(__FUNCTION__, "Tomo semaforo");
            printf("Adding event %d at index %zu\n", event, buffer->actual_position);
            buffer->events[buffer->actual_position] = event;
            printf("Guardé el valor en events\n");
            buffer->actual_position = (buffer->actual_position + 1) % BUFFER_SIZE;
            printf("Guardé el valor en actual_position\n");

            time_t current_time;
            NTPconfig(&current_time);
            printf("Pasé el valor current al NTPconfig\n");

            buffer->actual_time = current_time;
            printf("Guardé el valor en actual time\n");

            // Print buffer content after loading
            printf("Buffer loaded from NVS. Current buffer state:\n");
            for (int i = 0; i < BUFFER_SIZE; i++) {
                printf("Index %d: Event %d\n", i, buffer->events[i]);
            }

            xSemaphoreGive(SemaphoreNTP); 
            ESP_LOGI(__FUNCTION__, "libero semaforo");

        } else {
            ESP_LOGI(__FUNCTION__, "NO TOMO SEMAFORO TASK A");
        }
}

