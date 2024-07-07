#ifndef NTP_H
#define NTP_H

#include <time.h>

time_t NTPconfig(time_t *current_time);
void seteo_modo_operacion_NTP ();
void ntp_sync_time(void);
void obtain_time(void);
void initNTP();

#endif /* NTP_H */