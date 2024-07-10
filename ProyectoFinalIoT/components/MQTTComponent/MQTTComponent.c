#include "MQTTComponent.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "C:\Users\sofia.ruiz\esp\v4.4.7\esp-idf\components\mqtt\esp-mqtt\include\mqtt_client.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "loggercircular.h"
#include "QueueComponent.h"

#define BUF_SIZE (1024)
#define QUEUE_LENGTH 10
static char *TAG = "MQTT";



circular_buffer_t buffer;


static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGI(TAG, "Last error %s: 0x%x", message, error_code);
    }
}


static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data){

    ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    
    event_t event_received;
    int valor_recibido;
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_publish(client, "/ValentinaSofia1", "Connected", 0, 1, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            msg_id = esp_mqtt_client_subscribe(client, "/ValentinaSofia1", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, "/ValentinaSofia1", "Subscribed", 0, 1, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("/ValentinaSofia1=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            
           // Enviar ACK de recepción
            char ack_message[50];  // Mensaje de ACK arbitrario
            snprintf(ack_message, sizeof(ack_message), "ACK %.*s", event->data_len, event->data);
            msg_id = esp_mqtt_client_publish(client, "/ValentinaSofia1_ack", ack_message, 0, 1, 0);
            if (msg_id < 0) {
                ESP_LOGE(TAG, "Error al enviar ACK: %d", msg_id);
            } else {
                ESP_LOGI(TAG, "Sent ACK for msg_id=%d", event->msg_id);
            }

            if (strncmp(event->data, "EVENT_PLAY", event->data_len) == 0) {
                event_received = EVENT_PLAY;
                valor_recibido = event_received;

            } else if (strncmp(event->data, "EVENT_PAUSE", event->data_len) == 0) {
                event_received = EVENT_PAUSE;
                valor_recibido = event_received;

            } else if (strncmp(event->data, "EVENT_NEXT", event->data_len) == 0) {
                event_received = EVENT_NEXT;
                valor_recibido = event_received;

            } else if (strncmp(event->data, "EVENT_PREVIOUS", event->data_len) == 0) {
                event_received = EVENT_PREVIOUS;
                valor_recibido = event_received;

            } else if (strncmp(event->data, "EVENT_STOP", event->data_len) == 0) {
                event_received = EVENT_STOP;
                valor_recibido = event_received;

            } else if (strncmp(event->data, "EVENT_VOLUMEN_UP", event->data_len) == 0) {
                event_received = EVENT_VOLUMEN_UP;
                valor_recibido = event_received;

            }   else if (strncmp(event->data, "EVENT_VOLUMEN_DOWN", event->data_len) == 0) {
                event_received = EVENT_VOLUMEN_DOWN;
                valor_recibido = event_received;

            } else {
                ESP_LOGI(TAG, "No se reconoce el evento ingresado: %.*s", event->data_len, event->data);
                break;
            }
            printf("Tengo el evento actual guardado es %d\n", valor_recibido);
        
            
            envio_queue (event_received);

            recibo_queue();

            add_event_to_buffer(buffer, valor_recibido);

            save_circular_buffer(buffer);
            load_circular_buffer(buffer);
            // add_event_to_buffer(circular_buffer_t *buffer, event_t valor_recibido);



            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return event_received;
}


// Configuración del cliente MQTT
void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://broker.hivemq.com",
    };
    printf("accedí a la pagina Broker URL correctamente");

    //Declaro el handle para el cliente MQTT.
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg); // Inicializo el cliente MQTT
    printf("declara el handler correctamente\n");
    // La función mqtt_event_handler maneja los eventos MQTT
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
        printf("la funcion maneja los evento MQTT correctamente\n");
    esp_mqtt_client_start(client);
        printf("la funcion start funciono correctamente\n");

       
}

