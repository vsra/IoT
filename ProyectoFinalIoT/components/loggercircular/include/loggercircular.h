#include "esp_err.h"

esp_err_t save_circular_buffer();
esp_err_t load_circular_buffer();
void add_event_to_buffer();

typedef enum {
    EVENT_PLAY = 0,
    EVENT_PAUSE,
    EVENT_NEXT,
    EVENT_PREVIOUS,
    EVENT_STOP,
    EVENT_VOLUMEN_UP,
    EVENT_VOLUMEN_DOWN
} event_t;


#define BUFFER_SIZE 20

typedef struct {
    int events[BUFFER_SIZE];
    size_t actual_position;
    time_t actual_time;
} circular_buffer_t;