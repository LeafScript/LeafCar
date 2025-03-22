#ifndef _SERVICE_TIMER_H_
#define _SERVICE_TIMER_H_

#include "base_type.h"

typedef void (*service_timer_cb_func)(void *priv, bool is_finish);

void service_timer_init(uint32_t period_ms);
void service_timer_scan(void);
int service_timer_trigger_once(uint32_t trigger_ms, service_timer_cb_func cb, void *priv);
int service_timer_trigger_muti(uint32_t trigger_ms, service_timer_cb_func cb, void *priv, uint32_t trigger_times);
int service_timer_loop(uint32_t trigger_ms, service_timer_cb_func cb, void *priv);

#endif