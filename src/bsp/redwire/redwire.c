#include "redwire.h"
#include "board_config.h"

void Redwire_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(REDWIRE_RCC_PORT, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = REDWIRE_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		//上拉输入
	GPIO_Init(REDWIRE_PORT, &GPIO_InitStructure);
}

//若用东西遮挡，返回1
bool Redwire_isFull(void)
{
	if(GPIO_ReadInputDataBit(REDWIRE_PORT, REDWIRE_PIN) == Bit_SET){
		return 1;
	}
	return 0;
}
