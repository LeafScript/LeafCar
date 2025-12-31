#include "tb6612.h"
#include "board_config.h"
#include "timer.h"
#include "error_code.h"

static void tb6612_dir_gpio_init(tb6612_context_s *ctx)
{
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Pin = ctx->dir.in1_pin | ctx->dir.in2_pin;
    gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ctx->dir.port, &gpio_init);
}

static void tb6612_encoder_gpio_init(tb6612_context_s *ctx)
{
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio_init.GPIO_Pin = ctx->enc.a_pin | ctx->enc.b_pin;
    GPIO_Init(ctx->enc.port, &gpio_init);
}

static void tb6612_pwm_gpio_init(tb6612_context_s *ctx)
{
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Pin = ctx->pwm.pin;
    gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽输出由外设硬件控制GPIO电平变化
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ctx->pwm.port, &gpio_init);
}

static void tb6612_encoder_timer_init(tb6612_context_s *ctx)
{
    TIM_TimeBaseInitTypeDef timer_init = {
        0, TIM_CounterMode_Up, ctx->enc.tim_period, TIM_CKD_DIV1, 0
    };
    TIM_ICInitTypeDef timer_ic_init = {
        TIM_Channel_1, TIM_ICPolarity_Rising, TIM_ICSelection_DirectTI, TIM_ICPSC_DIV1, 10
    };

    TIM_TimeBaseInit(ctx->enc.tim, &timer_init);
    TIM_EncoderInterfaceConfig(ctx->enc.tim, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICInit(ctx->enc.tim, &timer_ic_init);
    TIM_ClearFlag(ctx->enc.tim, TIM_FLAG_Update);
    TIM_ITConfig(ctx->enc.tim, TIM_IT_Update, ENABLE);
    TIM_SetCounter(ctx->enc.tim, 0);
}

static void tb6612_pwm_timer_init(tb6612_context_s *ctx)
{
    TIM_TimeBaseInitTypeDef timer_init = {
        ctx->pwm.prescaler, TIM_CounterMode_Up, ctx->pwm.period, TIM_CKD_DIV1, 0
    };
    TIM_OCInitTypeDef timer_oc_init = {
        .TIM_OCMode = TIM_OCMode_PWM1,
        .TIM_OutputState = TIM_OutputState_Enable,
        .TIM_OCPolarity = TIM_OCPolarity_High,
    };
    TIM_TimeBaseInit(ctx->pwm.tim, &timer_init);
    if (ctx->pwm.tim_comp == TB6612_PWM_COMP_1) {
        TIM_OC1Init(ctx->pwm.tim, &timer_oc_init);
        TIM_OC1PreloadConfig(ctx->pwm.tim, TIM_OCPreload_Enable);
    } else if (ctx->pwm.tim_comp == TB6612_PWM_COMP_2) {
        TIM_OC2Init(ctx->pwm.tim, &timer_oc_init);
        TIM_OC2PreloadConfig(ctx->pwm.tim, TIM_OCPreload_Enable);
    } else if (ctx->pwm.tim_comp == TB6612_PWM_COMP_3) {
        TIM_OC3Init(ctx->pwm.tim, &timer_oc_init);
        TIM_OC3PreloadConfig(ctx->pwm.tim, TIM_OCPreload_Enable);
    } else if (ctx->pwm.tim_comp == TB6612_PWM_COMP_4) {
        TIM_OC4Init(ctx->pwm.tim, &timer_oc_init);
        TIM_OC4PreloadConfig(ctx->pwm.tim, TIM_OCPreload_Enable);
    }
}

static void tb6612_encoder_init(tb6612_context_s *ctx)
{
    if (ctx->enc.tim_remap != 0) {
        GPIO_PinRemapConfig(ctx->enc.tim_remap, ENABLE);
    }
    tb6612_encoder_gpio_init(ctx);
    tb6612_encoder_timer_init(ctx);
}

void tb6612_pwm_init(tb6612_context_s *ctx)
{
    if (ctx->pwm.tim_remap != 0) {
        GPIO_PinRemapConfig(ctx->pwm.tim_remap, ENABLE);
    }
    tb6612_pwm_gpio_init(ctx);
    tb6612_pwm_timer_init(ctx);
}

int tb6612_init(tb6612_context_s *ctx)
{
    uint32_t apb1_rcc = 0, apb2_rcc = 0;

    BUILD_BUG_ON(sizeof(bool) != sizeof(uint8_t));
    if (ctx->pwm.tim_comp >= TB6612_PWM_COMP_MAX) {
        return EC_ERROR;
    }

    // RCC init
    apb1_rcc |= ctx->enc.is_port_apb1 ? ctx->enc.tim_port_rcc : apb1_rcc;
    apb1_rcc |= ctx->pwm.is_port_apb1 ? ctx->pwm.tim_port_rcc : apb1_rcc;
    apb2_rcc |= ctx->dir.port_rcc;
    apb2_rcc |= !ctx->enc.is_port_apb1 ? ctx->enc.tim_port_rcc : apb2_rcc;
    apb2_rcc |= ((ctx->enc.tim_remap != 0) || (ctx->pwm.tim_remap != 0)) ? RCC_APB2Periph_AFIO : apb2_rcc;
    RCC_APB1PeriphClockCmd(apb1_rcc, ENABLE);
    RCC_APB2PeriphClockCmd(apb2_rcc, ENABLE);
    // dir gpio init
    tb6612_dir_gpio_init(ctx);
    // encoder timer & gpio init
    tb6612_encoder_init(ctx);
    // pwm timer & gpio init
    tb6612_pwm_init(ctx);
    return EC_OK;
}

static void tb6612_encoder_timer_start(tb6612_context_s *ctx)
{
    TIM_Cmd(ctx->enc.tim, ENABLE); 
}

static void tb6612_pwm_timer_start(tb6612_context_s *ctx)
{
    TIM_Cmd(ctx->pwm.tim, ENABLE);
}

void tb6612_start(tb6612_context_s *ctx)
{
    tb6612_encoder_timer_start(ctx);
    tb6612_pwm_timer_start(ctx);
}

void tb6612_set_dir(tb6612_context_s *ctx, enum tb6612_dir_e dir)
{
    if (dir == TB6612_FORWARD) {
        GPIO_WriteBit(ctx->dir.port, ctx->dir.in1_pin, Bit_RESET);
        GPIO_WriteBit(ctx->dir.port, ctx->dir.in2_pin, Bit_SET);
    } else if (dir == TB6612_BACK) {
        GPIO_WriteBit(ctx->dir.port, ctx->dir.in1_pin, Bit_SET);
        GPIO_WriteBit(ctx->dir.port, ctx->dir.in2_pin, Bit_RESET);
    } else {	// TB6612_STOP
        GPIO_WriteBit(ctx->dir.port, ctx->dir.in1_pin, Bit_SET);
        GPIO_WriteBit(ctx->dir.port, ctx->dir.in2_pin, Bit_SET);
    }
}

void tb6612_set_pwm(tb6612_context_s *ctx, uint16_t pwm)
{
    if (ctx->pwm.tim_comp == TB6612_PWM_COMP_1) {
        TIM_SetCompare1(ctx->pwm.tim, pwm);
    } else if (ctx->pwm.tim_comp == TB6612_PWM_COMP_2) {
        TIM_SetCompare2(ctx->pwm.tim, pwm);
    } else if (ctx->pwm.tim_comp == TB6612_PWM_COMP_3) {
        TIM_SetCompare3(ctx->pwm.tim, pwm);
    } else if (ctx->pwm.tim_comp == TB6612_PWM_COMP_4) {
        TIM_SetCompare4(ctx->pwm.tim, pwm);
    }
}

uint16_t tb6612_encoder_read_and_reset(tb6612_context_s *ctx, uint16_t reset_val)
{
    uint16_t val = ctx->enc.tim->CNT;
    ctx->enc.tim->CNT = reset_val;
    return val;
}