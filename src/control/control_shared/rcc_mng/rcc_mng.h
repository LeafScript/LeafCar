#ifndef _RCC_MNG_H_
#define _RCC_MNG_H_

#include "base_type.h"

enum rcc_apb_e {
    RCC_APB1 = 0x1,
    RCC_APB2 = 0x2
};

void rcc_enable(enum rcc_apb_e apbn, uint32_t rcc_mask);
void rcc_disable(enum rcc_apb_e apbn, uint32_t rcc_mask);

#endif