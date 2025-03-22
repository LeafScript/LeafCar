#include "redwire.h"

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
		return 1;
	}
	return 0;
}
