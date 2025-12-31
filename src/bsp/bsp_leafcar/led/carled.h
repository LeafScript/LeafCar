#ifndef __CARLED_H
#define __CARLED_H

#include "base_type.h"

enum led_color_e {
    LED_RED,
    LED_YELLOW,
    LED_GREEN
};

void Carled_Init(void);

#endif