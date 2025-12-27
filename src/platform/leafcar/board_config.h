#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include "stm32f10x.h"
#include "base_type.h"

// 红外
#define REDWIRE_RCC_PORT            RCC_APB2Periph_GPIOF
#define REDWIRE_PORT                GPIOF
#define REDWIRE_PIN    	            GPIO_Pin_0

// 机械臂
#define ARM_RCC_PORT                RCC_APB2Periph_GPIOE
#define ARM_PORT                    GPIOE
#define ARM_PIN0                    GPIO_Pin_0
#define ARM_PIN1                    GPIO_Pin_1

// TB6612
#define TB6612_ID_0_DIR_PORT        GPIOB
#define TB6612_ID_0_DIR_IN1_PIN     GPIO_Pin_13
#define TB6612_ID_0_DIR_IN2_PIN     GPIO_Pin_9

#define TB6612_ID_1_DIR_PORT        GPIOB
#define TB6612_ID_1_DIR_IN1_PIN     GPIO_Pin_15
#define TB6612_ID_1_DIR_IN2_PIN     GPIO_Pin_14

#define TB6612_ID_2_DIR_PORT        GPIOD
#define TB6612_ID_2_DIR_IN1_PIN     GPIO_Pin_0
#define TB6612_ID_2_DIR_IN2_PIN     GPIO_Pin_1

#define TB6612_ID_3_DIR_PORT        GPIOD
#define TB6612_ID_3_DIR_IN1_PIN     GPIO_Pin_4
#define TB6612_ID_3_DIR_IN2_PIN     GPIO_Pin_3

#endif