//void init_network(void);
//int wifi_init_sta(const char *ssid, const char *password);
//void init_network(const char *ssid, const char *password);



#ifndef APSTA_MODULE_H
#define APSTA_MODULE_H

void init_network(const char *ssid, const char *password);
void wifi_init(void *queue);
QueueHandle_t command_queue;

#endif // APSTA_MODULE_H
