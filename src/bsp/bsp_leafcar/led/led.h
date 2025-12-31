#ifndef __LED_H
#define __LED_H

#include "base_type.h"

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