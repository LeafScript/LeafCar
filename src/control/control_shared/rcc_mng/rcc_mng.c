#include "rcc_mng.h"
#include "board_config.h"

void rcc_enable(enum rcc_apb_e apbn, uint32_t rcc_mask)
{
    if (apbn == RCC_APB1) {
        RCC_APB1PeriphClockCmd(rcc_mask, ENABLE);
    } else if (apbn == RCC_APB2) {
        RCC_APB2PeriphClockCmd(rcc_mask, ENABLE);
    }
}

void rcc_disable(enum rcc_apb_e apbn, uint32_t rcc_mask)
{
    if (apbn == RCC_APB1) {
        RCC_APB1PeriphClockCmd(rcc_mask, DISABLE);
    } else if (apbn == RCC_APB2) {
        RCC_APB2PeriphClockCmd(rcc_mask, DISABLE);
    }
}