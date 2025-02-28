#ifndef __ARM_H
#define __ARM_H

#include "sys.h"

#define ARM_RCC_PORT RCC_APB2Periph_GPIOE
#define ARM_PORT     GPIOE
#define ARM_PIN0      GPIO_Pin_0
#define ARM_PIN1      GPIO_Pin_1

#define ArmOut0		 PEout(0)
#define ArmOut1		 PEout(1)


void Arm_Init(void);
void Arm_SetStatus(uint8_t status);
	
#endif