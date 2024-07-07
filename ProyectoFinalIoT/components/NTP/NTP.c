#include <stdio.h>
#include "NTP.h"
#include <string.h>
#include "CustomDelay.h"
 
//#include "nvs_flash.h"
//#include "nvs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/apps/sntp.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
//#include "protocol_examples_common.h"
#include "esp_sntp.h"
// Definir ESP_SNTP_OPMODE_POLL como SNTP_OPMODE_POLL si no está definido
#ifndef ESP_SNTP_OPMODE_POLL
#define ESP_SNTP_OPMODE_POLL SNTP_OPMODE_POLL
#endif

static const char *TAG = "NTP";


void seteo_modo_operacion_NTP (){
    sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
}


time_t NTPconfig(time_t *current_time) {
    
    // Esperar hasta que se obtenga la hora
    while ((*current_time = time(NULL)) == ((time_t)-1)) {
        delay_seg(1); // Esperar 1 segundo antes de intentar nuevamente
    }

    struct tm timeinfo;
    localtime_r(current_time, &timeinfo);

    printf("La hora actual es: %04d-%02d-%02d %02d:%02d:%02d\n",
            timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
            timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    return *current_time;
}


void ntp_sync_time(void)
{
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    // Esperar a que el tiempo se sincronice
    time_t now = 0;
    struct tm timeinfo = {0};
    int retry = 0;
    const int retry_count = 10;
    while (timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count)
    {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }
}

void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

void obtain_time(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();

    // Wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    while (timeinfo.tm_year < (2020 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (retry == retry_count) {
        ESP_LOGE(TAG, "Failed to synchronize with NTP server");
    } else {
        ESP_LOGI(TAG, "Time synchronized: %s", asctime(&timeinfo));
    }
}




void initNTP() {       
    // Wait for the WiFi to connect
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    obtain_time();
    // Print the current time every 10 seconds
    while (1) {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);
        ESP_LOGI(TAG, "Current time: %s", asctime(&timeinfo));
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}