#include <stdio.h>
//#include <esp_http_server.h>
#include <string.h>
#include <esp_err.h>
#include "esp_event.h"
#include "esp_system.h"
#include "esp_log.h"
#include "config.h"

#include "webserver.h"
#include "APSTAmodule.h"

#define TAG "webserver"

    const char *username = "Sofía Ruiz Alejandro";
    const char *password = "sof12345";

void startWebserver(void)
{
    ESP_LOGI(TAG, "Starting HTTP Server");


    ESP_LOGI(TAG, "Usuario: %s y contrasena de wifi: %s", username, password);

    int status = wifi_init_sta(username, password);
        if (status == 200)
        {
            const char resp[] = "Connected to WiFi";
            //httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
        }
        else
        {
            const char resp[] = "Failed to connect to WiFi";
           // httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
        }
    
}