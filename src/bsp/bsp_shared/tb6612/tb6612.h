#ifndef _TB6612_H_
#define _TB6612_H_

#include "board_config.h"

// clear timer flag
#define TB6612_DEF_TIMX_ENC_IRQ_HANDLE(tim_num) \
void TIM##tim_num##_IRQHandler(void) \
{ \
    if(TIM##tim_num->SR & 0x0001) {} \
    TIM##tim_num->SR &= ~(1 << 0); \
}

enum tb6612_dir_e {
	TB6612_FORWARD,
	TB6612_BACK,
	TB6612_STOP
};

enum tb6612_pwm_tim_oc_e {
    TB6612_PWM_OC_1,
    TB6612_PWM_OC_2,
    TB6612_PWM_OC_3,
    TB6612_PWM_OC_4,
    TB6612_PWM_OC_NUM
};

typedef struct {
    GPIO_TypeDef *port;     // see: Peripheral_declaration
    uint16_t in1_pin;       // see: GPIO_pins_define
    uint16_t in2_pin;       // see: GPIO_pins_define
} tb6612_dir_context_s;

typedef struct {
    TIM_TypeDef *tim;       // see: Peripheral_declaration
    uint32_t tim_remap;     // see: GPIO_Remap_define
    uint16_t prescaler;     // max_pwm_val = FREQ / (prescaler * period)
    uint16_t period;
    bool tim_oc_enable[TB6612_PWM_OC_NUM];
    uint8_t port_num;
    GPIO_TypeDef *port1;    // see: Peripheral_declaration
    uint16_t port1_pin;     // see: GPIO_pins_define
    GPIO_TypeDef *port2;    // see: Peripheral_declaration
    uint16_t port2_pin;     // see: GPIO_pins_define
} tb6612_pwm_context_s;

typedef struct {
    TIM_TypeDef *tim;       // see: Peripheral_declaration
    uint32_t tim_remap;     // see: GPIO_Remap_define
    uint16_t tim_period;
    GPIO_TypeDef *port;     // see: Peripheral_declaration
    uint16_t a_pin;         // see: GPIO_pins_define
    uint16_t b_pin;         // see: GPIO_pins_define
} tb6612_enc_context_s;

int tb6612_dir_init(tb6612_dir_context_s *ctx);
int tb6612_pwm_init(tb6612_pwm_context_s *ctx);
int tb6612_enc_init(tb6612_enc_context_s *ctx);
void tb6612_pwm_timer_start(tb6612_pwm_context_s *ctx);
void tb6612_encoder_timer_start(tb6612_enc_context_s *ctx);
void tb6612_set_dir(tb6612_dir_context_s *ctx, enum tb6612_dir_e dir);
void tb6612_set_pwm(tb6612_pwm_context_s *ctx, enum tb6612_pwm_tim_oc_e ocn, uint16_t pwm);
uint16_t tb6612_encoder_read_and_reset(tb6612_enc_context_s *ctx, uint16_t reset_val);

#endif