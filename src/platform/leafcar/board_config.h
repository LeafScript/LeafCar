#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include "stm32f10x.h"
#include "base_type.h"

#define BOARD_FREQ                      SYSCLK_FREQ_72MHz

// Car Led
#define CARLED_RCC_PORT                 RCC_APB2Periph_GPIOF
#define CARLED_PORT                     GPIOF
#define RED_LED_PIN                     GPIO_Pin_7
#define YEL_LED_PIN                     GPIO_Pin_8
#define GRE_LED_PIN                     GPIO_Pin_9

// Led
#define LED1_RCC_PORT                   RCC_APB2Periph_GPIOB
#define LED1_PORT                       GPIOB
#define LED1_PIN                        GPIO_Pin_5
#define LED2_RCC_PORT                   RCC_APB2Periph_GPIOE
#define LED2_PORT                       GPIOE
#define LED2_PIN                        GPIO_Pin_5

// 红外
#define REDWIRE_RCC_PORT                RCC_APB2Periph_GPIOF
#define REDWIRE_PORT                    GPIOF
#define REDWIRE_PIN    	                GPIO_Pin_0

// 机械臂
#define ARM_RCC_PORT                    RCC_APB2Periph_GPIOE
#define ARM_PORT                        GPIOE
#define ARM_PIN0                        GPIO_Pin_0
#define ARM_PIN1                        GPIO_Pin_1

// TB6612
#define TB6612_ID_0_1_DIR_RCC           RCC_APB2Periph_GPIOB
#define TB6612_ID_2_3_DIR_RCC           RCC_APB2Periph_GPIOD
#define TB6612_PWM_TIMER_RCC            RCC_APB1Periph_TIM2
#define TB6612_ID_0_1_PWM_GPIO_RCC      RCC_APB2Periph_GPIOA
#define TB6612_ID_2_3_PWM_GPIO_RCC      RCC_APB2Periph_GPIOB
#define TB6612_ID_0_ENC_TIMER_RCC       RCC_APB1Periph_TIM3
#define TB6612_ID_1_ENC_TIMER_RCC       RCC_APB2Periph_TIM8
#define TB6612_ID_2_ENC_TIMER_RCC       RCC_APB2Periph_TIM1
#define TB6612_ID_3_ENC_TIMER_RCC       RCC_APB1Periph_TIM4
#define TB6612_ID_0_ENC_GPIO_RCC        RCC_APB2Periph_GPIOA
#define TB6612_ID_1_ENC_GPIO_RCC        RCC_APB2Periph_GPIOC
#define TB6612_ID_2_ENC_GPIO_RCC        RCC_APB2Periph_GPIOE
#define TB6612_ID_3_ENC_GPIO_RCC        RCC_APB2Periph_GPIOD

#define TB6612_PWM_TIMER                TIM2
#define TB6612_ID_0_ENC_TIMER           TIM3
#define TB6612_ID_1_ENC_TIMER           TIM8
#define TB6612_ID_2_ENC_TIMER           TIM1
#define TB6612_ID_3_ENC_TIMER           TIM4

#define TB6612_ID_0_DIR_PORT            GPIOB
#define TB6612_ID_0_DIR_IN1_PIN         GPIO_Pin_13
#define TB6612_ID_0_DIR_IN2_PIN         GPIO_Pin_9
#define TB6612_ID_0_PWM_PORT            GPIOA
#define TB6612_ID_0_PWM_PIN             GPIO_Pin_15
#define TB6612_ID_0_ENC_PORT            GPIOA
#define TB6612_ID_0_ENC_A_PIN           GPIO_Pin_6
#define TB6612_ID_0_ENC_B_PIN           GPIO_Pin_7

#define TB6612_ID_1_DIR_PORT            GPIOB
#define TB6612_ID_1_DIR_IN1_PIN         GPIO_Pin_15
#define TB6612_ID_1_DIR_IN2_PIN         GPIO_Pin_14
#define TB6612_ID_1_PWM_PORT            GPIOB
#define TB6612_ID_1_PWM_PIN             GPIO_Pin_3
#define TB6612_ID_1_ENC_PORT            GPIOC
#define TB6612_ID_1_ENC_A_PIN           GPIO_Pin_6
#define TB6612_ID_1_ENC_B_PIN           GPIO_Pin_7

#define TB6612_ID_2_DIR_PORT            GPIOD
#define TB6612_ID_2_DIR_IN1_PIN         GPIO_Pin_0
#define TB6612_ID_2_DIR_IN2_PIN         GPIO_Pin_1
#define TB6612_ID_2_PWM_PORT            GPIOA
#define TB6612_ID_2_PWM_PIN             GPIO_Pin_2
#define TB6612_ID_2_ENC_PORT            GPIOE
#define TB6612_ID_2_ENC_A_PIN           GPIO_Pin_9
#define TB6612_ID_2_ENC_B_PIN           GPIO_Pin_11

#define TB6612_ID_3_DIR_PORT            GPIOD
#define TB6612_ID_3_DIR_IN1_PIN         GPIO_Pin_4
#define TB6612_ID_3_DIR_IN2_PIN         GPIO_Pin_3
#define TB6612_ID_3_PWM_PORT            GPIOA
#define TB6612_ID_3_PWM_PIN             GPIO_Pin_3
#define TB6612_ID_3_ENC_PORT            GPIOD
#define TB6612_ID_3_ENC_A_PIN           GPIO_Pin_12
#define TB6612_ID_3_ENC_B_PIN           GPIO_Pin_13

#endif