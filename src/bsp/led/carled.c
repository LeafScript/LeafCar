#include "carled.h"

/*******************************引脚配置**********************************/
//	信号灯引脚:    1~5: PF7 PF8 PF9    红 黄 绿
/*************************************************************************/

//小车信号灯初始化
void Carled_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(CARLED_RCC_PORT, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin   = RED_LED_PIN | YEL_LED_PIN | GRE_LED_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CARLED_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(CARLED_PORT, RED_LED_PIN | YEL_LED_PIN | GRE_LED_PIN);
}
