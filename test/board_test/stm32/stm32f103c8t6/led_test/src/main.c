#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

#define LED_RCC_PORT  RCC_APB2Periph_GPIOC
#define LED_PORT      GPIOC
#define LED_PIN       GPIO_Pin_13

#define LED PCout(13)

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_RCC_PORT, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin   = LED_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LED_PORT, LED_PIN);
}

void LED_Toggle(void)
{
    LED = !LED;
}
 
int main()
{
    LED_Init();
    delay_init();
 
    while(1)
    {
        delay_ms(500);
        LED_Toggle();
    }
}