#include "timer.h"
#include "board_config.h"

/*****************************定时器分配**********************************/
//	TIM3 TIM8 TIM1 TIM4: TB6612编码器计数
//	TIM2: TB6612的4路PWM
//	TIM6 TIM7: 做定时器中断
//
//	TIM2: PA15  PB3  PA2  PA3
//
//	TIM3: PA6  PA7
//	TIM8: PC6  PC7
//	TIM1: PE9  PE11
//	TIM4: PD12 PD13
/*************************************************************************/

/*********************************定时器中断***************************************/

/* 通用定时器6中断初始化，做扫描 */
void TIM6_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef timer_init = { psc, TIM_CounterMode_Up, arr, TIM_CKD_DIV1, 0 };
    NVIC_InitTypeDef nvic_init = { TIM6_IRQn, 1, 1, ENABLE };
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    TIM_TimeBaseInit(TIM6, &timer_init);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    NVIC_Init(&nvic_init);
}

/* 通用定时器7中断初始化，做扫描 */
void TIM7_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef timer_init = { psc, TIM_CounterMode_Up, arr, TIM_CKD_DIV1, 0 };
    NVIC_InitTypeDef nvic_init = { TIM7_IRQn, 3, 1, ENABLE };
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    TIM_TimeBaseInit(TIM7, &timer_init);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    NVIC_Init(&nvic_init);
}

void timer_service_init(void)
{
    TIM6_Int_Init(9999, 71);	//100Hz	-  10ms定时
    TIM7_Int_Init(19999, 71);	//50Hz	-  20ms定时
}

void timer_service_start(void)
{
    TIM_Cmd(TIM6, ENABLE);
    TIM_Cmd(TIM7, ENABLE);
}

void timer_service_stop(void)
{
    TIM_Cmd(TIM6, DISABLE);
    TIM_Cmd(TIM7, DISABLE);
}

static timer_serv_func g_serv_func[TIMER_SERV_MAX] = { 0 };

void timer_service_register(uint8_t timx, timer_serv_func func)
{
    if (timx >= TIMER_SERV_MAX) {
        return;
    }
    g_serv_func[timx] = func;
}

/* 定时器6中断服务函数 */
void TIM6_IRQHandler(void)
{	
    if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)	//TIM6更新中断已发生
    {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        if (g_serv_func[TIMER_SERV_TIM6] != NULL) {
            g_serv_func[TIMER_SERV_TIM6]();
        }
    }
}

/* 定时器7中断服务函数 */
void TIM7_IRQHandler(void)
{	
    if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)	//TIM7更新中断已发生
    {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        if (g_serv_func[TIMER_SERV_TIM7] != NULL) {
            g_serv_func[TIMER_SERV_TIM7]();
        }
    }
}
