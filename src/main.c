#include "misc.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "led.h"
#include "car_basic.h"
#include "car_ctrl.h"
#include "redwire.h"
#include "carled.h"
#include "arm.h"
#include "service_timer.h"
#include "openmv.h"
#include "k210.h"
#include "task_schedule.h"
#include "cli_task.h"

/***************************定时器通道引脚********************************/
//				CH1				CH2				CH3				CH4
//	TIM1:     PA8/PE9         PA9/PE11        PA10/PE13       PA11/PE14
//	TIM2:     PA0/PA15        PA1/PB3         PA2/PB10        PA3/PB11
//	TIM3:    PA6/PC6/PB4     PA7/PC7/PB5      PB0/PC8         PB1/PC9
//	TIM4:     PB6/PD12        PB7/PD13        PB8/PD14        PB9/PD15
//	TIM5:       PA0             PA1             PA2             PA3
//	TIM6:        X               X               X               X
//	TIM7:        X               X               X               X
//	TIM8:       PC6             PC7             PC8             PC9
/*************************************************************************/

/****************************STM32串口引脚********************************/
//		                TX                  RX
//	USART1:            PA9                 PA10
//	USART2:            PA2                 PA3
//	USART3:            PB10                PB11
//	UART4:             PC10                PC11
//	UART5:             PC12                PD2
/*************************************************************************/

/*****************************定时器分配**********************************/
//  TIM引脚分配见: 《STM32英文参考手册》 Timer alternate function remapping 章节
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
 
/*******************************引脚配置**********************************/
//                  TB6612方向引脚          TB6612 PWM引脚       TB6612编码器引脚
//  TB6612_ID_0     IN1:PB13  IN2:PB9       PA15 (TIM2)         A:PA6  B:PA7  (TIM3)
//  TB6612_ID_1     IN1:PB15 IN2:PB14       PB3  (TIM2)         A:PC6  B:PC7  (TIM8)
//  TB6612_ID_2     IN1:PD0  IN2:PD1        PA2  (TIM2)         A:PE9  B:PE11 (TIM1)
//  TB6612_ID_3     IN1:PD4  IN2:PD3        PA3  (TIM2)         A:PD12 B:PD13 (TIM4)
//
//	灰度传感器引脚:    1~5: PE10 PE8 PE12 PE13 PE14
//
//	信号灯引脚:    	   PF7 PF8 PF9    红 黄 绿
//
//	红外引脚：		   PF0
//
//
/*************************************************************************/

static void board_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//配置中断优先级

	usart_service_register(USART_SERV_USART1, cli_recv_data);
	usart_service_register(USART_SERV_USART3, openmv_update_data);
	usart_service_register(USART_SERV_UART5, k210_update_data);
	USART1_Init(9600);
	USART3_Init(9600);
	UART5_Init(9600);

	LED_Init();
	Carled_Init();
	Redwire_Init();
}

static void background_service_scan(void)
{
	service_timer_scan();
}

static void service_init(void)
{
	delay_init();
	car_init();
	Arm_Init();
	timer_service_init();
	timer_service_register(TIMER_SERV_TIM6, car_scan);
	timer_service_register(TIMER_SERV_TIM7, background_service_scan);
	service_timer_init(20);
	task_schedule_init();
}

static void service_start(void)
{
	timer_service_start();
	task_schedule_start();
}

int main(void)
{
	board_init();
	service_init();
	service_start();
	while(1)
	{
		task_schedule_scan();
	}
}
