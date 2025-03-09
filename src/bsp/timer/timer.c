#include "timer.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"

/*****************************定时器分配**********************************/
//	TIM3 TIM8 TIM1 TIM4: TB6612编码器计数
//	TIM2: TB6612的4路PWM
//	TIM6 TIM7: 做定时器中断
//
//	TIM2: PA15  PB3  PA2  PA3
//
//	TIM3: PA6  PA7
//	TIM8: PC6  PC7
//	TIM1: PE9  PE11
//	TIM4: PD12 PD13
/*************************************************************************/

/*********************************定时器中断***************************************/

/* 通用定时器6中断初始化，做扫描 */
void TIM6_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef timer_init = { psc, TIM_CounterMode_Up, arr, TIM_CKD_DIV1, 0 };
	NVIC_InitTypeDef nvic_init = { TIM6_IRQn, 1, 1, ENABLE };
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInit(TIM6, &timer_init);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	NVIC_Init(&nvic_init);
}

/* 通用定时器7中断初始化，做扫描 */
void TIM7_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef timer_init = { psc, TIM_CounterMode_Up, arr, TIM_CKD_DIV1, 0 };
	NVIC_InitTypeDef nvic_init = { TIM7_IRQn, 3, 1, ENABLE };
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	TIM_TimeBaseInit(TIM7, &timer_init);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	NVIC_Init(&nvic_init);
}

void timer_service_init(void)
{
	TIM6_Int_Init(9999, 71);	//100Hz	-  10ms定时
	TIM7_Int_Init(19999, 71);	//50Hz	-  20ms定时
}

void timer_service_start(void)
{
	TIM_Cmd(TIM6, ENABLE);
	TIM_Cmd(TIM7, ENABLE);
}

void timer_service_stop(void)
{
	TIM_Cmd(TIM6, DISABLE);
	TIM_Cmd(TIM7, DISABLE);
}

static timer_serv_func g_serv_func[2] = { NULL, NULL };

void timer_service_register(uint8_t timx, timer_serv_func func)
{
	if (timx >= TIMER_SERV_MAX) {
		return;
	}
	g_serv_func[timx] = func;
}

/* 定时器6中断服务函数 */
void TIM6_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)	//TIM6更新中断已发生
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		if (g_serv_func[TIMER_SERV_TIM6] != NULL) {
			g_serv_func[TIMER_SERV_TIM6]();
		}
	}
}

/* 定时器7中断服务函数 */
void TIM7_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)	//TIM7更新中断已发生
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		if (g_serv_func[TIMER_SERV_TIM7] != NULL) {
			g_serv_func[TIMER_SERV_TIM7]();
		}
	}
}

/**********************************PWM***************************************/
/* TIM2 PWM 初始化 */
//PWM输出:		PA15	TIM2_CH1		FL
//			  	PB3		TIM2_CH2		FR
//				PA2		TIM2_CH3        BL
//				PA3		TIM2_CH4        BR
void timer_pwm_init(uint16_t arr, uint16_t psc)
{
	TIM_TimeBaseInitTypeDef timer_init = { psc, TIM_CounterMode_Up, arr, TIM_CKD_DIV1, 0 };
	TIM_OCInitTypeDef timer_oc_init = {
		.TIM_OCMode = TIM_OCMode_PWM1,
		.TIM_OutputState = TIM_OutputState_Enable,
		.TIM_OCPolarity = TIM_OCPolarity_High,
	};
	GPIO_InitTypeDef gpio_init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);	//TIM2完全重映射
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//禁止JTAG,从而PA15可以做普通IO使用,否则PA15不能做普通IO!!!
	
	gpio_init.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_2 | GPIO_Pin_3;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio_init);
	
	gpio_init.GPIO_Pin = GPIO_Pin_3;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &gpio_init);

	TIM_TimeBaseInit(TIM2, &timer_init);

	TIM_OC1Init(TIM2, &timer_oc_init);
	TIM_OC2Init(TIM2, &timer_oc_init);
	TIM_OC3Init(TIM2, &timer_oc_init);
	TIM_OC4Init(TIM2, &timer_oc_init);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_Cmd(TIM2, ENABLE);
}

/**********************************TB6612编码器引脚***************************************/
/*
 * 电机编码器定时器初始化：
 *               编码器 / 电机驱动		位置
 * TIM3: PA6        A相 / B1A          前左
 *       PA7        B相 / B1B
 * TIM8: PC6        A相 / B2A          前右
 *       PC7        B相 / B2B
 * TIM1: PE9        A相 / B1A          后左
 *       PE11       B相 / B1B
 * TIM4: PD12       A相 / B1A          后右
 *       PD13       B相 / B1B
 */
static void timer_encoder_gpio_init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_init.GPIO_Pin = GPIO_Pin;
	GPIO_Init(GPIOx, &gpio_init);
}

#define ENCODER_TIM_PERIOD 60000   // number of pulses per revolution

static void timer_encoder_timer_init(TIM_TypeDef* TIMx)
{
	TIM_TimeBaseInitTypeDef timer_init = { 0, TIM_CounterMode_Up, ENCODER_TIM_PERIOD, TIM_CKD_DIV1, 0 };
	TIM_ICInitTypeDef timer_ic_init = {
		TIM_Channel_1, TIM_ICPolarity_Rising, TIM_ICSelection_DirectTI, TIM_ICPSC_DIV1, 10
	};

	TIM_TimeBaseInit(TIMx, &timer_init);
	TIM_EncoderInterfaceConfig(TIMx, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICInit(TIMx, &timer_ic_init);
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);					//清除TIM的更新标志位
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
	TIM_SetCounter(TIMx, 0);
	TIM_Cmd(TIMx, ENABLE); 
}

void timer_encoder_init(void)
{
	uint32_t rcc_apb1 = RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4;
	uint32_t rcc_apb2 = RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
		RCC_APB2Periph_GPIOE | RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM8 | RCC_APB2Periph_AFIO;

	// RCC init
	RCC_APB1PeriphClockCmd(rcc_apb1, ENABLE);
	RCC_APB2PeriphClockCmd(rcc_apb2, ENABLE);

	// GPIO init
	timer_encoder_gpio_init(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);
	timer_encoder_gpio_init(GPIOC, GPIO_Pin_6 | GPIO_Pin_7);
	timer_encoder_gpio_init(GPIOE, GPIO_Pin_9 | GPIO_Pin_11);
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
	timer_encoder_gpio_init(GPIOD, GPIO_Pin_12 | GPIO_Pin_13);

	// Timer init
	timer_encoder_timer_init(TIM3);
	timer_encoder_timer_init(TIM8);
	timer_encoder_timer_init(TIM1);
	timer_encoder_timer_init(TIM4);
}