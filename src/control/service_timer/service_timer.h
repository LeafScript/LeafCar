#ifndef _SERVICE_TIMER_H_
#define _SERVICE_TIMER_H_

#include "base_type.h"

void service_timer_init(uint32_t period_ms);
void service_timer_scan(void);
int service_timer_trigger_once(uint32_t trigger_ms, void (*cb)(void *priv), void *priv);
int service_timer_trigger_muti(uint32_t trigger_ms, void (*cb)(void *priv), void *priv, uint32_t trigger_times);
int service_timer_loop(uint32_t trigger_ms, void (*cb)(void *priv), void *priv);

#endif