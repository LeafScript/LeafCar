#include "arm.h"

void Arm_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(ARM_RCC_PORT, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin   = ARM_PIN0 | ARM_PIN1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ARM_PORT, &GPIO_InitStructure);
	ArmOut1 = 0; ArmOut0 = 0;
}

//设置机械臂的转动状态
void Arm_SetStatus(uint8_t status)
{
	switch(status)
	{
		case 1: ArmOut1 = 0; ArmOut0 = 0; break; // 1  收起状态
		case 2: ArmOut1 = 0; ArmOut0 = 1; break; // 2  中间状态
		case 3: ArmOut1 = 1; ArmOut0 = 0; break; // 3  左边状态
		case 4: ArmOut1 = 1; ArmOut0 = 1; break;
	}
}
