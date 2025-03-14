#ifndef _TIMER_H_
#define _TIMER_H_

#include "base_type.h"

enum timer_serv_timx {
    TIMER_SERV_TIM6,
    TIMER_SERV_TIM7,
    TIMER_SERV_MAX
};
typedef void (*timer_serv_func)(void);

/* TIM7: usmart扫描, TIM6: 小车控制扫描 */
void timer_service_init(void);
void timer_service_register(uint8_t timx, timer_serv_func func);
void timer_service_start(void);
void timer_service_stop(void);
/* TB6612的PWM引脚 */
void timer_pwm_init(uint16_t arr, uint16_t psc);
void timer_pwm_start(void);
/* TB6612编码器引脚 */
void timer_encoder_init(uint16_t period);
void timer_encoder_start(void);

#endif