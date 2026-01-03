#ifndef _TIMER_H_
#define _TIMER_H_

#include "base_type.h"

enum timer_serv_timx {
    TIMER_SERV_TIM6,
    TIMER_SERV_TIM7,
    TIMER_SERV_MAX
};
typedef void (*timer_serv_func)(void);

void timer_service_init(void);
void timer_service_register(uint8_t timx, timer_serv_func func);
void timer_service_start(void);
void timer_service_stop(void);

#endif