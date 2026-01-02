#include "arm.h"
#include "board_config.h"

void Arm_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(ARM_RCC_PORT, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = ARM_PIN0 | ARM_PIN1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ARM_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(ARM_PORT, ARM_PIN0, Bit_RESET);
    GPIO_WriteBit(ARM_PORT, ARM_PIN1, Bit_RESET);
}

// 设置机械臂的转动状态
void Arm_SetStatus(arm_status_e status)
{
    uint8_t arm_pin0 = (status == ARM_STATUS_MIDDLE) ? Bit_SET : Bit_RESET;
    uint8_t arm_pin1 = (status == ARM_STATUS_LEFT) ? Bit_SET : Bit_RESET;
    GPIO_WriteBit(ARM_PORT, ARM_PIN0, arm_pin0);
    GPIO_WriteBit(ARM_PORT, ARM_PIN1, arm_pin1);
}
