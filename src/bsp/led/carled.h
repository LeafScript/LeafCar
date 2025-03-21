#ifndef __CARLED_H
#define __CARLED_H

#include "sys.h"

#define CARLED_RCC_PORT  RCC_APB2Periph_GPIOF
#define CARLED_PORT      GPIOF
#define RED_LED_PIN       GPIO_Pin_7
#define YEL_LED_PIN       GPIO_Pin_8
#define GRE_LED_PIN       GPIO_Pin_9

#define RedLed PFout(7)
#define YelLed PFout(8)
#define GreLed PFout(9)

enum led_color_e {
    LED_RED,
    LED_YELLOW,
    LED_GREEN
};

void Carled_Init(void);

#endif