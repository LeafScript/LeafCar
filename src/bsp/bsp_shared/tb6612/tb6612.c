#include "tb6612.h"
#include "board_config.h"
#include "timer.h"
#include "error_code.h"

static void tb6612_dir_gpio_init(tb6612_dir_context_s *ctx)
{
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Pin = ctx->in1_pin | ctx->in2_pin;
    gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ctx->port, &gpio_init);
}

static void tb6612_encoder_gpio_init(tb6612_enc_context_s *ctx)
{
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio_init.GPIO_Pin = ctx->a_pin | ctx->b_pin;
    GPIO_Init(ctx->port, &gpio_init);
}

static void tb6612_pwm_gpio_init(tb6612_pwm_context_s *ctx)
{
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Pin = ctx->port1_pin;
    gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽输出由外设硬件控制GPIO电平变化
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ctx->port1, &gpio_init);
    if (ctx->port_num == 1) {
        return;
    }
    gpio_init.GPIO_Pin = ctx->port2_pin;
    gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ctx->port2, &gpio_init);
}

static void tb6612_encoder_timer_init(tb6612_enc_context_s *ctx)
{
    TIM_TimeBaseInitTypeDef timer_init = {
        0, TIM_CounterMode_Up, ctx->tim_period, TIM_CKD_DIV1, 0
    };
    TIM_ICInitTypeDef timer_ic_init = {
        TIM_Channel_1, TIM_ICPolarity_Rising, TIM_ICSelection_DirectTI, TIM_ICPSC_DIV1, 10
    };

    TIM_TimeBaseInit(ctx->tim, &timer_init);
    TIM_EncoderInterfaceConfig(ctx->tim, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICInit(ctx->tim, &timer_ic_init);
    TIM_ClearFlag(ctx->tim, TIM_FLAG_Update);
    TIM_ITConfig(ctx->tim, TIM_IT_Update, ENABLE);
    TIM_SetCounter(ctx->tim, 0);
}

static void tb6612_pwm_timer_oc_init(tb6612_pwm_context_s *ctx, enum tb6612_pwm_tim_oc_e ocn)
{
    TIM_OCInitTypeDef timer_oc_init = {0};
    timer_oc_init.TIM_OCMode = TIM_OCMode_PWM1;
    timer_oc_init.TIM_OutputState = TIM_OutputState_Enable;
    timer_oc_init.TIM_OCPolarity = TIM_OCPolarity_High;
    timer_oc_init.TIM_Pulse = 0;

    if (ocn == TB6612_PWM_OC_1) {
        TIM_OC1Init(ctx->tim, &timer_oc_init);
        TIM_OC1PreloadConfig(ctx->tim, TIM_OCPreload_Enable);
    } else if (ocn == TB6612_PWM_OC_2) {
        TIM_OC2Init(ctx->tim, &timer_oc_init);
        TIM_OC2PreloadConfig(ctx->tim, TIM_OCPreload_Enable);
    } else if (ocn == TB6612_PWM_OC_3) {
        TIM_OC3Init(ctx->tim, &timer_oc_init);
        TIM_OC3PreloadConfig(ctx->tim, TIM_OCPreload_Enable);
    } else if (ocn == TB6612_PWM_OC_4) {
        TIM_OC4Init(ctx->tim, &timer_oc_init);
        TIM_OC4PreloadConfig(ctx->tim, TIM_OCPreload_Enable);
    }
}

static void tb6612_pwm_timer_init(tb6612_pwm_context_s *ctx)
{
    TIM_TimeBaseInitTypeDef timer_init = {
        ctx->prescaler, TIM_CounterMode_Up, ctx->period, TIM_CKD_DIV1, 0
    };
    uint8_t i;

    TIM_TimeBaseInit(ctx->tim, &timer_init);
    TIM_ARRPreloadConfig(ctx->tim, ENABLE);     // timer溢出时arr生效
    for (i = 0; i < TB6612_PWM_OC_NUM; i++) {
        if (ctx->tim_oc_enable[i] == true) {
            tb6612_pwm_timer_oc_init(ctx, i);
        }
    }
}

int tb6612_dir_init(tb6612_dir_context_s *ctx)
{
    tb6612_dir_gpio_init(ctx);
    return EC_OK;
}

int tb6612_enc_init(tb6612_enc_context_s *ctx)
{
    if (ctx->tim_remap != 0) {
        GPIO_PinRemapConfig(ctx->tim_remap, ENABLE);
    }
    tb6612_encoder_gpio_init(ctx);
    tb6612_encoder_timer_init(ctx);
    return EC_OK;
}

int tb6612_pwm_init(tb6612_pwm_context_s *ctx)
{
    BUILD_BUG_ON(sizeof(bool) != sizeof(uint8_t));
    if (ctx->port_num != 1 && ctx->port_num != 2) {
        return EC_ERROR;
    }
    if (ctx->tim_remap != 0) {
        GPIO_PinRemapConfig(ctx->tim_remap, ENABLE);
    }
    tb6612_pwm_gpio_init(ctx);
    tb6612_pwm_timer_init(ctx);
    return EC_OK;
}

void tb6612_pwm_timer_start(tb6612_pwm_context_s *ctx)
{
    TIM_Cmd(ctx->tim, ENABLE);
}

void tb6612_encoder_timer_start(tb6612_enc_context_s *ctx)
{
    TIM_Cmd(ctx->tim, ENABLE); 
}

void tb6612_set_dir(tb6612_dir_context_s *ctx, enum tb6612_dir_e dir)
{
    if (dir == TB6612_FORWARD) {
        GPIO_WriteBit(ctx->port, ctx->in1_pin, Bit_RESET);
        GPIO_WriteBit(ctx->port, ctx->in2_pin, Bit_SET);
    } else if (dir == TB6612_BACK) {
        GPIO_WriteBit(ctx->port, ctx->in1_pin, Bit_SET);
        GPIO_WriteBit(ctx->port, ctx->in2_pin, Bit_RESET);
    } else {	// TB6612_STOP
        GPIO_WriteBit(ctx->port, ctx->in1_pin, Bit_SET);
        GPIO_WriteBit(ctx->port, ctx->in2_pin, Bit_SET);
    }
}

void tb6612_set_pwm(tb6612_pwm_context_s *ctx, enum tb6612_pwm_tim_oc_e ocn, uint16_t pwm)
{
    if (ocn == TB6612_PWM_OC_1 && ctx->tim_oc_enable[TB6612_PWM_OC_1]) {
        TIM_SetCompare1(ctx->tim, pwm);
    } else if (ocn == TB6612_PWM_OC_2 && ctx->tim_oc_enable[TB6612_PWM_OC_2]) {
        TIM_SetCompare2(ctx->tim, pwm);
    } else if (ocn == TB6612_PWM_OC_3 && ctx->tim_oc_enable[TB6612_PWM_OC_3]) {
        TIM_SetCompare3(ctx->tim, pwm);
    } else if (ocn == TB6612_PWM_OC_4 && ctx->tim_oc_enable[TB6612_PWM_OC_4]) {
        TIM_SetCompare4(ctx->tim, pwm);
    }
}

uint16_t tb6612_encoder_read_and_reset(tb6612_enc_context_s *ctx, uint16_t reset_val)
{
    uint16_t val = ctx->tim->CNT;
    ctx->tim->CNT = reset_val;
    return val;
}