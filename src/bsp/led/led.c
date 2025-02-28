#include "led.h"

static uint8_t timer[2] = {0};
static uint8_t led_flag = 0;

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

//隔time - 100ms闪烁
void LED_Toggle_ms(uint8_t num, uint16_t time)
{
	led_flag = 1;
	switch(num){
		case 1: timer[0] = LED_PERIOD_TIMES*time; break;
		case 2: timer[1] = LED_PERIOD_TIMES*time; break;
	}
}

//关闭LED扫描
void LED_CloseScan()
{
	led_flag = 0;
}

//LED扫描
void LED_Scan()
{
	static uint16_t cnt[2] = {0};
	
	if(led_flag==0){
		return;
	}
	if(cnt[0]>=timer[0]){
		LED_Toggle(1);
	}
	if(cnt[1]>=timer[1]){
		LED_Toggle(2);
	}
	cnt[0]++; cnt[1]++;
}
