#include "base_type.h"
#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "cli_task.h"

static void board_init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//配置中断优先级

    usart_service_register(USART_SERV_USART1, cli_recv_data);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    USART1_Init(9600);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);
    LED_Init();
}

int main(void)
{
    board_init();

    while (1) {
        LED_Scan();
    }
    return 0;
}