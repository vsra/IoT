#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
//#include "audioComponent.h"
//#include <FreeRTOS.h>

static char *TAG = "MQTT";
QueueHandle_t command_queue;

typedef enum {
    EVENT_PLAY = 0,
    EVENT_PAUSE,
    EVENT_NEXT,
    EVENT_PREVIOUS,
    EVENT_STOP,
    EVENT_VOLUMEN_UP,
    EVENT_VOLUMEN_DOWN
} event_t;
    int evento_recibido;


//Función que envía a la queue
void envio_queue(event_t evento_actual){
    BaseType_t xStatus;
    printf("Entre a la queue\n");
    // Intentar enviar a la cola
    xStatus = xQueueSendToBack(command_queue, &evento_actual, (TickType_t) 0);
    if (xStatus != pdPASS) {
        ESP_LOGI(TAG, "No se pudo enviar el evento a la cola\n");
        return;
    }
    ESP_LOGI(TAG, "Evento enviado a la cola\n");
}
    // Recibir de la cola
int recibo_queue(){
    BaseType_t xStatus;
    event_t event_saved_queue = (event_t) malloc(sizeof(event_t));
    xStatus = xQueueReceive(command_queue, &event_saved_queue, pdMS_TO_TICKS(500));;

    if (xStatus == pdPASS) {
        ESP_LOGI(TAG, "Evento recibido de la cola\n");
    } else {
        ESP_LOGI(TAG, "No se pudo recibir el evento de la cola\n");
    }
    
    
    // Procesar el evento recibido
    switch (event_saved_queue) {
        case EVENT_PLAY:
            printf("Evento recibido: EVENT_PLAY con numero: %d \n" , event_saved_queue);
            evento_recibido = EVENT_PLAY;
            break;
        case EVENT_PAUSE:
            printf("Evento recibido: EVENT_PAUSE con numero: %d \n" , event_saved_queue);
            evento_recibido = EVENT_PAUSE;
            break;
        case EVENT_NEXT:
            printf("Evento recibido: EVENT_NEXT con numero: %d \n" , event_saved_queue);
            evento_recibido = EVENT_NEXT;
            break;
        case EVENT_PREVIOUS:
            printf("Evento recibido: EVENT_PREVIOUS con numero: %d \n" , event_saved_queue);
            evento_recibido = EVENT_PREVIOUS;
            break;
        case EVENT_STOP:
            printf("Evento recibido: EVENT_STOP con numero: %d \n" , event_saved_queue);
            evento_recibido = EVENT_STOP;
            break;
        case EVENT_VOLUMEN_UP:
            printf("Evento recibido: EVENT_VOLUMEN_UP con numero: %d \n" , event_saved_queue);
            evento_recibido = EVENT_VOLUMEN_UP;
            break;
        case EVENT_VOLUMEN_DOWN:
            printf("Evento recibido: EVENT_VOLUMEN_DOWN con numero: %d \n" , event_saved_queue);
            evento_recibido = EVENT_VOLUMEN_DOWN;
            break;
        default:
            printf("Evento desconocido recibido\n");
            break;
    }
        return evento_recibido;
}
