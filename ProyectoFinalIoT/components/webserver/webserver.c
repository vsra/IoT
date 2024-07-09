#include <stdio.h>
#include "webserver.h"
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_eth.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include <esp_http_server.h>
#include "lwip/err.h"
#include "lwip/sys.h"

#include "CustomDelay.h"
#include "APSTAmodule.h"

#define TAG "ws_echo_server"

#define BUF_SIZE (1024)

/*
 * Structure holding server handle
 * and internal socket fd in order
 * to use out of request send
 */
struct async_resp_arg {
    httpd_handle_t hd;
    int fd;
};

char *comandoRecibido;



esp_err_t hello_get_handler(httpd_req_t *req) {
    ESP_LOGI(TAG, "Dentro del hello_get_handler");
    extern const uint8_t index_html_start[] asm("_binary_index_html_start");
    extern const uint8_t index_html_end[] asm("_binary_index_html_end");
    const size_t index_html_size = (index_html_end - index_html_start);
    httpd_resp_send(req, (const char *)index_html_start, index_html_size);
    return ESP_OK;
}

// Función para buscar una variable entre paréntesis en un string
char* getValueFromJSon(const char* str, char inicioStr, char finStr, int largoStr) {
    const char* inicio = strchr(str, inicioStr);  // Buscar el primer '('
    if (inicio == NULL) {
        return NULL; 
    }
    for(int i = 0; i < largoStr; i++ ) {
        inicio++;
    }
    
    const char* fin = strchr(inicio, finStr);  
    if (fin == NULL) {
        return NULL; 
    }
    
    // Copiar la variable a un nuevo string
    size_t longitud = fin - inicio;
    char* variable = malloc(longitud + 1);
    if (variable == NULL) {
        return NULL;  // Manejo de error: no se pudo asignar memoria
    }
    strncpy(variable, inicio, longitud);
    variable[longitud] = '\0';  // Asegurar que el string esté terminado
    
    return variable;
}


char inicioString = '{"username":"';
char finString = '","password":"';

char *usernameIngresado;
char *passwordIngresado;

esp_err_t wifi_conection_handler(httpd_req_t *req) {
    usernameIngresado = (char *) malloc(BUF_SIZE);
    passwordIngresado = (char *) malloc(BUF_SIZE);

    extern const uint8_t index_html_start[] asm("_binary_index_html_start");
    extern const uint8_t index_html_end[] asm("_binary_index_html_end");
    const size_t index_html_size = (index_html_end - index_html_start);

    httpd_resp_send(req, (const char *)index_html_start, index_html_size);
    char content[100];
    size_t recv_size = MIN(req->content_len, sizeof(content));
    httpd_req_recv(req, content, recv_size);

    ESP_LOGI(TAG, "Los datos recibidos son: %s", content);
    
    usernameIngresado = getValueFromJSon(content, inicioString, '"', 13);
    ESP_LOGI(TAG, "El username es: %s", usernameIngresado);

    //Las siguientes lineas de codigo están comentadas ya que daban un error al compilar, su funcionalidad era contar el largo 
    //de la cadena donde se ingresaba el usuario y contraseña de la red wifi a conectar.

    //int sumaLargoTotal = (strlen(inicioString) + strlen(usernameIngresado) + strlen(finString));

    //ESP_LOGI(TAG, "El largoTotal es: %d", strlen(inicioString));

    passwordIngresado = getValueFromJSon(content, finString, '"', 34);
    ESP_LOGI(TAG, "El password es: %s", passwordIngresado);

    usernameIngresado = "caliope";
    passwordIngresado = "sinlugar";
    //wifi_init_sta(usernameIngresado, passwordIngresado);

    //{"usernamee":"caliope","password":"sinlugar"}
return ESP_OK;
}

#define QUEUE_LENGTH 30
char received_value[25];//Sttring para ver lo de la cola

//Imprime el siguiente elemento en la cola, NO SE ELIMINA EL ELEMENTO DE LA QUEUE!!!!!
void imprimirCola(QueueHandle_t  cola) {

    // Esperar hasta recibir un valor en la cola
    if (xQueuePeek(cola, &received_value, portMAX_DELAY)) {
        // Imprimir el valor recibido
        ESP_LOGI(TAG, "Siguiente en QUEUE: %s", received_value);
    }else{

    };
    
}

esp_err_t command_sent_handler(httpd_req_t *req) {
    usernameIngresado = (char *) malloc(BUF_SIZE);
    passwordIngresado = (char *) malloc(BUF_SIZE);

    extern const uint8_t index_html_start[] asm("_binary_index_html_start");
    extern const uint8_t index_html_end[] asm("_binary_index_html_end");
    const size_t index_html_size = (index_html_end - index_html_start);

    httpd_resp_send(req, (const char *)index_html_start, index_html_size);
    char comandos[100];
    size_t recv_size = MIN(req->content_len, sizeof(comandos));
    httpd_req_recv(req, comandos, recv_size);

    ESP_LOGI(TAG, "Los datos recibidos son: %s", comandos);

    char inicioStringCom = '{"accionSeleccionada":"';

    comandoRecibido = getValueFromJSon(comandos, inicioStringCom, '"', 22);

    ESP_LOGI(TAG, "El comando es: %s", comandoRecibido);

   // xQueueSend(command_queue, &comandoRecibido, (TickType_t) 0);
    //MALLOC para enviar a la queue
    void* comandoRecibidoQUEUE = malloc(sizeof(comandoRecibido));
    strcpy(comandoRecibidoQUEUE , comandoRecibido);

    if (xQueueSend(command_queue, comandoRecibidoQUEUE, pdMS_TO_TICKS(100) )){
        ESP_LOGI(TAG, "Enviado a QUEUE");
    }else{
        ESP_LOGI(TAG, "No se pudo enviar a QUEUE");//Solo si la QUEUE llena o NULL
    };

   imprimirCola(command_queue);
  



return ESP_OK;
}



esp_err_t js_get_handler(httpd_req_t *req) {
    extern const uint8_t script_js_start[] asm("_binary_script_js_start");
    extern const uint8_t script_js_end[] asm("_binary_script_js_end");
    const size_t script_js_size = (script_js_end - script_js_start);
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char *)script_js_start, script_js_size);
    return ESP_OK;
}

esp_err_t send_data_post_handler(httpd_req_t *req) {
    char content[100];
    size_t recv_size = MIN(req->content_len, sizeof(content));
    int ret = httpd_req_recv(req, content, recv_size);
    if (ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }
    content[recv_size] = '\0';
    ESP_LOGI(TAG, "Received data: %s", content);
    httpd_resp_send(req, "Data received", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}



httpd_handle_t start_webserver(void) {
    delay_miliseg(200);
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t hello_uri = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = hello_get_handler,
            .user_ctx  = NULL   
        };
        httpd_register_uri_handler(server, &hello_uri);

        httpd_uri_t wifi_uri_conection = {
            .uri       = "/solicitudEnviada",
            .method    = HTTP_POST,
            .handler   = wifi_conection_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &hello_uri);

        httpd_uri_t comando_uri_conection = {
            .uri       = "/controlSonido",
            .method    = HTTP_POST,
            .handler   = command_sent_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &comando_uri_conection);

        httpd_register_uri_handler(server, &wifi_uri_conection);

        httpd_uri_t js_uri = {
            .uri       = "/script.js",
            .method    = HTTP_GET,
            .handler   = js_get_handler,
            .user_ctx  = NULL
        };

        httpd_register_uri_handler(server, &js_uri);
        httpd_uri_t send_data_uri = {
            .uri       = "/send-data",
            .method    = HTTP_POST,
            .handler   = send_data_post_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &send_data_uri);
    }
    return server;
}


void init_webserver(void)
{
    ESP_LOGI(TAG, "Starting HTTP Server");
    static httpd_handle_t server = NULL;
    server = start_webserver();
}
