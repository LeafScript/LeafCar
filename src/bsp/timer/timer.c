#include "timer.h"
#include "usmart.h"
#include "car.h"
#include <stdio.h>
#include "led.h"
#include "cartask.h"

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
//arr：自动重装值。
//psc：时钟预分频数
void TIM6_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;			//不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInitStructure.TIM_Period=arr;							//重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;						//预分频值
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	
	//TIM6 1:1
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM6, ENABLE);
}

/* 定时器6中断服务函数 */
void TIM6_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)	//TIM6更新中断已发生
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

//		LED_Scan();
//		Key_Scan();
		Car_Scan();
	}
}

/* 通用定时器7中断初始化，做扫描 */
//arr：自动重装值。
//psc：时钟预分频数
void TIM7_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;			//不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInitStructure.TIM_Period=arr;							//重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;						//预分频值
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	
	//TIM7 3:1
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM7, ENABLE);
}

/* 定时器7中断服务函数 */
void TIM7_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)	//TIM7更新中断已发生
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

		usmart_dev.scan();	//执行usmart扫描
//		CarTask_Scan();
	}
}

/**********************************PWM***************************************/

/* TIM2 PWM 初始化 */
//arr:自动重装载值	ARR寄存器，控制周期
//psc:时钟预分频数	
//PWM输出:		PA15	TIM2_CH1		FL
//			  	PB3		TIM2_CH2		FR
//				PA2		TIM2_CH3        BL
//				PA3		TIM2_CH4        BR
void TIM2_Pwm_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//使能AFIO时钟 
	
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);	//TIM2完全重映射
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//禁止JTAG,从而PA15可以做普通IO使用,否则PA15不能做普通IO!!!
		
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;										//复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15 | GPIO_Pin_2 | GPIO_Pin_3;					//PA15|PA2|PA3
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;										//复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;							//PB3
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;			//不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInitStructure.TIM_Period=arr;							//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;						//预分频数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;						//PWM1模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;				//有效电平为高
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);							//TIM2输出通道1初始化
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);							//TIM2输出通道2初始化
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);							//TIM2输出通道3初始化
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);							//TIM2输出通道4初始化
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);					//使能TIM2和OC1的预装载寄存器
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);					//使能TIM2和OC2的预装载寄存器
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);					//使能TIM2和OC3的预装载寄存器
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);					//使能TIM2和OC4的预装载寄存器
	TIM_Cmd(TIM2, ENABLE);
}

/**********************************TB6612编码器引脚***************************************/

/* 定时器3初始化 ，使用前左电机编码器功能 */
//PA6----接 编码器A相 或者电机驱动的B1A标识
//PA7----接 编码器B相 或者电机驱动的B1B标识
void TIM3_Encoder_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef       TIM_ICInitStructure;  
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);	//开启复用功能时钟
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE);	//TIM3部分重映射

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 						// 预分频器 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 			//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);					//清除TIM的更新标志位
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//Reset counter
	TIM_SetCounter(TIM3,0);
	TIM_Cmd(TIM3, ENABLE); 
}

/* 定时器8初始化 ，使用前右电机编码器功能 */
//PC6----接 编码器A相 或者电机驱动的B2A标识
//PC7----接 编码器B相 或者电机驱动的B2B标识
void TIM8_Encoder_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef       TIM_ICInitStructure;  
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//浮空输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 						// 预分频器 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 			//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数  
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;					//重复计数器为0
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM8, TIM_FLAG_Update);					//清除TIM的更新标志位
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	//Reset counter
	TIM_SetCounter(TIM8,0);
	TIM_Cmd(TIM8, ENABLE); 
}

/* 定时器1初始化 ，使用后左电机编码器功能 */
//PE9 ----接 编码器A相 或者电机驱动的B1A标识
//PE11----接 编码器B相 或者电机驱动的B1B标识
void TIM1_Encoder_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef       TIM_ICInitStructure;  
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//开启复用功能时钟
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);	//TIM1部分重映射

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//浮空输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 						// 预分频器 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 			//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数  
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;					//重复计数器为0
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM1, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);					//清除TIM的更新标志位
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	//Reset counter
	TIM_SetCounter(TIM1,0);
	TIM_Cmd(TIM1, ENABLE); 
}

/* 定时器4初始化 ，使用后右电机编码器功能 */
//PD12----接 编码器A相 或者电机驱动的B1A标识
//PD13----接 编码器B相 或者电机驱动的B1B标识
void TIM4_Encoder_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef       TIM_ICInitStructure;  
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//使能AFIO时钟
	
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);	//TIM4完全重映射

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 						// 预分频器 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 			//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);					//清除TIM的更新标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	//Reset counter
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4, ENABLE); 
}
