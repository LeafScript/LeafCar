#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED1_RCC_PORT  RCC_APB2Periph_GPIOB
#define LED1_PORT      GPIOB
#define LED1_PIN       GPIO_Pin_5
#define LED2_RCC_PORT  RCC_APB2Periph_GPIOE
#define LED2_PORT      GPIOE
#define LED2_PIN       GPIO_Pin_5

#define LED1 PBout(5)
#define LED2 PEout(5)

//LED扫描周期-ms
#define LED_PERIOD    10
//100ms/LED扫描周期
#define LED_PERIOD_TIMES    (uint16_t)100.0/LED_PERIOD

void LED_Init(void);
void LED_Toggle(uint8_t num);
void LED_Toggle_ms(uint8_t num, uint16_t time);
void LED_CloseScan(void);
void LED_Scan(void);

#endif