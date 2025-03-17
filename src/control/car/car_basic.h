#ifndef _CAR_BASIC_H_
#define _CAR_BASIC_H_

#include "base_type.h"

void car_init(void);
void car_print(void);
void car_scan(void);
void car_set_pwm(uint8_t dir, int16_t pwm);

#endif