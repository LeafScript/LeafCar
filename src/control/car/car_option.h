#ifndef _CAR_OPTION_H_
#define _CAR_OPTION_H_

#include "base_type.h"

void car_forward(uint16_t dist, uint16_t speed);
void car_back(uint16_t dist, uint16_t speed);
void car_track(uint32_t dist, uint16_t speed, bool is_forward);
void car_turn(uint16_t dist, uint16_t speed, bool is_right);

#endif