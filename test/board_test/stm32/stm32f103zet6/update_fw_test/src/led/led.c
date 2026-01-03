#include "led.h"
#include "stm32f10x.h"

static uint8_t timer[2] = {0};
static uint8_t led_flag = 0;

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
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
		case 1: 
			GPIO_WriteBit(LED1_PORT, LED1_PIN,
				(GPIO_ReadOutputDataBit(LED1_PORT, LED1_PIN) == Bit_SET) ? Bit_RESET : Bit_SET);
			break;
		case 2:
			GPIO_WriteBit(LED2_PORT, LED2_PIN,
				(GPIO_ReadOutputDataBit(LED2_PORT, LED2_PIN) == Bit_SET) ? Bit_RESET : Bit_SET);
			break;
	}
}

//隔time - 100ms闪烁
void LED_Toggle_ms(uint8_t num, uint16_t time)
{
	led_flag = 1;
	switch(num){
		case 1: timer[0] = LED_PERIOD_TIMES * time; break;
		case 2: timer[1] = LED_PERIOD_TIMES * time; break;
	}
}

//关闭LED扫描
void LED_CloseScan(void)
{
	led_flag = 0;
}

//LED扫描
void LED_Scan()
{
	static uint16_t cnt[2] = {0};
	uint8_t i;
	
	if (led_flag == 0){
		return;
	}
	for (i = 0; i < 2; i++) {
		if (timer[i] == 0) {
			continue;
		}
		if (cnt[i] >= timer[i]) {
			cnt[i] = 0;
			LED_Toggle(i + 1);
		} else {
			cnt[i]++;
		}
	}
}
