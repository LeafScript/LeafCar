#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"

#define ENCODER_TIM_PERIOD (u16)(60000)   // number of pulses per revolution

/* 按键扫描、小车控制扫描 */
void TIM6_Int_Init(u16 arr, u16 psc);
/* usmart扫描 */
void TIM7_Int_Init(u16 arr, u16 psc);
/* TB6612的PWM引脚 */
void TIM2_Pwm_Init(u16 arr, u16 psc);
/* TB6612编码器引脚 */
void TIM3_Encoder_Init(void);
void TIM8_Encoder_Init(void);
void TIM1_Encoder_Init(void);
void TIM4_Encoder_Init(void);

#endif