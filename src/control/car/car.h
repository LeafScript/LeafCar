#ifndef __CAR_H
#define __CAR_H

#include "sys.h"

void Car_Init(void);
void Car_Print(void);
void Car_Stop(void);
void Car_Start(void);
void Car_Scan(void);
void Car_SetPwm(uint8_t dir, int16_t pwm);

#endif