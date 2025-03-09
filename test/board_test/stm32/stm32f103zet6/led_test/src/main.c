#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

#define LED1_RCC_PORT  RCC_APB2Periph_GPIOB
#define LED1_PORT      GPIOB
#define LED1_PIN       GPIO_Pin_5
#define LED2_RCC_PORT  RCC_APB2Periph_GPIOE
#define LED2_PORT      GPIOE
#define LED2_PIN       GPIO_Pin_5

#define LED1 PBout(5)
#define LED2 PEout(5)

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
	switch(num){
		case 1: LED1 = !LED1; break;
		case 2: LED2 = !LED2; break;
	}
}
 
int main()
{
    LED_Init();
    delay_init();
 
    while(1)
    {
        delay_ms(500);
        LED_Toggle(1);
        LED_Toggle(2);
    }
}