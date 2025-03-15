#ifndef _CAR_BASIC_H_
#define _CAR_BASIC_H_

#include "sys.h"

void Car_Init(void);
void Car_Print(void);
void Car_Scan(void);
void Car_SetPwm(uint8_t dir, int16_t pwm);

#endif