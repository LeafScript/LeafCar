#include "tb6612.h"
#include "timer.h"

/*******************************引脚配置**********************************/
//	TB6612方向引脚:    FL_AIN1:PB9  FL_AIN2:PB13    FR_BIN1:PB14 FR_BIN2:PB15
//					   BL_BIN1:PD0  BL_BIN2:PD1     BR_AIN1:PD4  BR_AIN2:PD3
//
//  TB6612的PWM引脚:   FL_PWMA:PA15    FR_PWMB:PB3    BL_PWMB:PA2    BR_PWMA:PA3 (TIM2)
//
//  TB6612编码器引脚:  FLA:PB4  FLB:PB5 (TIM3)    FRA:PC6  FRB:PC7 (TIM8)
//					   BLA:PE9  BLB:PE11(TIM1)    BRA:PB6  BRB:PB7 (TIM4)
/*************************************************************************/

//初始化TB6612模块
void TB6612_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(DIR_RCC_PORT1 | DIR_RCC_PORT2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = FL_DIR_AIN1 | FL_DIR_AIN2 | FR_DIR_BIN1 | FR_DIR_BIN2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                          /*设置引脚模式为通用推挽输出*/   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                         /*设置引脚速率为50MHz */ 
	GPIO_Init(DIR_PORT1, &GPIO_InitStructure);                                                 /*调用库函数，初始化GPIO*/
	
	GPIO_InitStructure.GPIO_Pin = BL_DIR_BIN1 | BL_DIR_BIN2 | BR_DIR_AIN1 | BR_DIR_AIN2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                          /*设置引脚模式为通用推挽输出*/   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                         /*设置引脚速率为50MHz */ 
	GPIO_Init(DIR_PORT2, &GPIO_InitStructure);                                                 /*调用库函数，初始化GPIO*/
	
	TIM3_Encoder_Init();
	TIM8_Encoder_Init();
	TIM1_Encoder_Init();
	TIM4_Encoder_Init();
}

/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
	int Encoder_TIM;    
	switch(TIMX)
	{
		case 3:  Encoder_TIM = (short)TIM3->CNT;  TIM3->CNT = 30000; break;
		case 8:  Encoder_TIM = (short)TIM8->CNT;  TIM8->CNT = 30000; break;	
		case 2:  Encoder_TIM = (short)TIM2->CNT;  TIM2->CNT = 30000; break;
		case 1:  Encoder_TIM = (short)TIM1->CNT;  TIM1->CNT = 30000; break;
		case 4:  Encoder_TIM = (short)TIM4->CNT;  TIM4->CNT = 30000; break;
		default: Encoder_TIM=0;
	}
	return Encoder_TIM;
}

/* TIM3中断服务函数 */
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}

/* TIM8中断服务函数 */
void TIM8_IRQHandler(void)
{ 		    		  			    
	if(TIM8->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM8->SR&=~(1<<0);//清除中断标志位 	    
}
/* TIM2中断服务函数 */
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}

/* TIM1中断服务函数 */
void TIM1_IRQHandler(void)
{ 		    		  			    
	if(TIM1->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM1->SR&=~(1<<0);//清除中断标志位 	    
}

/* TIM4中断服务函数 */
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 	    
}
