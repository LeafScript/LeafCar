#ifndef _CARLED_H_
#define _CARLED_H_

#include "base_type.h"

enum led_color_e {
    LED_RED,
    LED_YELLOW,
    LED_GREEN
};

void Carled_Init(void);

#endif