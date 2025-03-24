#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "delay.h"
#include "sys.h"
#include "ucos_ii.h"

#define LED1_RCC_PORT  RCC_APB2Periph_GPIOB
#define LED1_PORT      GPIOB
#define LED1_PIN       GPIO_Pin_5
#define LED2_RCC_PORT  RCC_APB2Periph_GPIOE
#define LED2_PORT      GPIOE
#define LED2_PIN       GPIO_Pin_5

#define LED1 PBout(5)
#define LED2 PEout(5)

#define TASK_STACK_SIZE  512
#define TASK_PRIO_LED1   5
#define TASK_PRIO_LED2   6

OS_STK TaskStackLED1[TASK_STACK_SIZE];
OS_STK TaskStackLED2[TASK_STACK_SIZE];

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(LED1_RCC_PORT | LED2_RCC_PORT, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = LED1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);
    GPIO_SetBits(LED1_PORT, LED1_PIN);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = LED2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED2_PORT, &GPIO_InitStructure);
    GPIO_SetBits(LED2_PORT, LED2_PIN);
}

void LED_Toggle(uint8_t num)
{
    switch(num) {
        case 1: LED1 = !LED1; break;
        case 2: LED2 = !LED2; break;
    }
}

// LED 1 闪烁任务
void TaskLED1(void *p_arg)
{
    (void)p_arg;
    while (1)
    {
        OSTimeDlyHMSM(0, 0, 0, 500);
        LED_Toggle(1);
    }
}

// LED 2 闪烁任务
void TaskLED2(void *p_arg)
{
    (void)p_arg;
    while (1)
    {
        OSTimeDlyHMSM(0, 0, 0, 500);
        LED_Toggle(2);
    }
}

int main()
{
    OSInit();

    LED_Init();
    delay_init();

    OSTaskCreate(TaskLED1, (void *)0, &TaskStackLED1[TASK_STACK_SIZE - 1], TASK_PRIO_LED1);
    OSTaskCreate(TaskLED2, (void *)0, &TaskStackLED2[TASK_STACK_SIZE - 1], TASK_PRIO_LED2);

    OSStart();

    while(1);
}    