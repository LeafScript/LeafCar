#ifndef __REDWIRE_H
#define __REDWIRE_H

#include "sys.h"

//红外引脚
#define REDWIRE_RCC_PORT RCC_APB2Periph_GPIOF
#define REDWIRE_PORT     GPIOF
#define REDWIRE_PIN    	 GPIO_Pin_0

#define RedIn  PFin(0)

void Redwire_Init(void);
u8 Redwire_isFull(void);

#endif
