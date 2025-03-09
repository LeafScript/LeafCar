#include "redwire.h"
#include "led.h"
#include "carled.h"
#include "usart.h"

void Redwire_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(REDWIRE_RCC_PORT,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = REDWIRE_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		//上拉输入
	GPIO_Init(REDWIRE_PORT, &GPIO_InitStructure);
}

//若用东西遮挡，返回1
u8 Redwire_isFull(void)
{
	if(RedIn == 1){
//		printf("0");
//		RedLed = 1;
//		YelLed = 1;
//		GreLed = 1;
		return 1;
	}//printf("1");
//	RedLed = 0;
//	YelLed = 0;
//	GreLed = 0;
	return 0;
}
