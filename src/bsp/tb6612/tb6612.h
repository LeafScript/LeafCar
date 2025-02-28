#ifndef __TB6612_H
#define __TB6612_H

#include "sys.h"

/* TB6612方向引脚配置 */
#define DIR_PORT1		GPIOB
#define DIR_PORT2		GPIOD
#define DIR_RCC_PORT1	RCC_APB2Periph_GPIOB
#define DIR_RCC_PORT2	RCC_APB2Periph_GPIOD
//PB
#define FL_DIR_AIN1  	GPIO_Pin_9
#define FL_DIR_AIN2  	GPIO_Pin_13
#define FR_DIR_BIN1  	GPIO_Pin_14
#define FR_DIR_BIN2  	GPIO_Pin_15
//PD
#define BL_DIR_BIN1  	GPIO_Pin_0
#define BL_DIR_BIN2  	GPIO_Pin_1
#define BR_DIR_AIN1  	GPIO_Pin_4
#define BR_DIR_AIN2  	GPIO_Pin_3

void TB6612_Init(void);
int Read_Encoder(u8 TIMX);

#endif