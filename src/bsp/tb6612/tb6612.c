#include "tb6612.h"
#include "board_config.h"
#include "timer.h"

/*******************************引脚配置**********************************/
//                  TB6612方向引脚          TB6612 PWM引脚       TB6612编码器引脚
//  TB6612_ID_0     IN1:PB13  IN2:PB9       PA15 (TIM2)         A:PA6  B:PA7  (TIM3)
//  TB6612_ID_1     IN1:PB15 IN2:PB14       PB3  (TIM2)         A:PC6  B:PC7  (TIM8)
//  TB6612_ID_2     IN1:PD0  IN2:PD1        PA2  (TIM2)         A:PE9  B:PE11 (TIM1)
//  TB6612_ID_3     IN1:PD4  IN2:PD3        PA3  (TIM2)         A:PD12 B:PD13 (TIM4)
/*************************************************************************/

static struct tb6612_bind {
    uint8_t id;
    uint16_t dir_in1;
    uint16_t dir_in2;
    GPIO_TypeDef *port;
    TIM_TypeDef *tim;
    uint8_t tim_comp;
} g_tb6612_bind_map[] = {
    { TB6612_ID_0, TB6612_ID_0_DIR_IN1_PIN, TB6612_ID_0_DIR_IN2_PIN, TB6612_ID_0_DIR_PORT, TIM2, 1 },
    { TB6612_ID_1, TB6612_ID_1_DIR_IN1_PIN, TB6612_ID_1_DIR_IN2_PIN, TB6612_ID_1_DIR_PORT, TIM2, 2 },
    { TB6612_ID_2, TB6612_ID_2_DIR_IN1_PIN, TB6612_ID_2_DIR_IN2_PIN, TB6612_ID_2_DIR_PORT, TIM2, 3 },
    { TB6612_ID_3, TB6612_ID_3_DIR_IN1_PIN, TB6612_ID_3_DIR_IN2_PIN, TB6612_ID_3_DIR_PORT, TIM2, 4 },
};

static void tb6612_init_one(uint8_t id)
{
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Pin = g_tb6612_bind_map[id].dir_in1 | g_tb6612_bind_map[id].dir_in2;
    gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(g_tb6612_bind_map[id].port, &gpio_init);
}

void tb6612_init(void)
{
    uint8_t id;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD, ENABLE);
    for (id = 0; id < TB6612_ID_MAX; id++) {
        tb6612_init_one(id);
    }
    timer_encoder_init(ENCODER_TIM_PERIOD);
    timer_pwm_init(3599, 1);	//20kHz  占空比控制电机速度
}

void tb6612_start(void)
{
    timer_encoder_start();
    timer_pwm_start();
}

void tb6612_set_dir(uint8_t id, uint8_t dir)
{
    struct tb6612_bind *bind = NULL;

    if (id > TB6612_ID_MAX) {
        return;
    }
    bind = &g_tb6612_bind_map[id];
    if (dir == TB6612_FORWARD) {
        GPIO_ResetBits(bind->port, bind->dir_in1);
        GPIO_SetBits(bind->port, bind->dir_in2);
    } else if (dir == TB6612_BACK) {
        GPIO_SetBits(bind->port, bind->dir_in1);
        GPIO_ResetBits(bind->port, bind->dir_in2);
    } else {	// TB6612_STOP
        GPIO_SetBits(bind->port, bind->dir_in1);
        GPIO_SetBits(bind->port, bind->dir_in2);
    }
}

void tb6612_set_pwm(uint8_t id, int16_t pwm)
{
    struct tb6612_bind *bind = NULL;

    if (id > TB6612_ID_MAX) {
        return;
    }
    bind = &g_tb6612_bind_map[id];
    if (bind->tim_comp == 1) {
        TIM_SetCompare1(bind->tim, pwm);
    } else if (bind->tim_comp == 2) {
        TIM_SetCompare2(bind->tim, pwm);
    } else if (bind->tim_comp == 3) {
        TIM_SetCompare3(bind->tim, pwm);
    } else if (bind->tim_comp == 4) {
        TIM_SetCompare4(bind->tim, pwm);
    }
}

int tb6612_encoder_read_and_reset(uint8_t id)
{
    static TIM_TypeDef *enc_tim[] = { TIM3, TIM8, TIM1, TIM4 };
    int val = (int)enc_tim[id]->CNT;

    enc_tim[id]->CNT = ENCODER_TIM_INIT_VAL;
    return val;
}

/* TIM3中断服务函数 */
void TIM3_IRQHandler(void)
{ 		    		  			    
    if(TIM3->SR&0X0001)//溢出中断
    {    				   				     	    	
    }				   
    TIM3->SR&=~(1<<0);//清除中断标志位 	    
}

/* TIM8中断服务函数 */
void TIM8_IRQHandler(void)
{ 		    		  			    
    if(TIM8->SR&0X0001)//溢出中断
    {    				   				     	    	
    }				   
    TIM8->SR&=~(1<<0);//清除中断标志位 	    
}

/* TIM1中断服务函数 */
void TIM1_IRQHandler(void)
{ 		    		  			    
    if(TIM1->SR&0X0001)//溢出中断
    {    				   				     	    	
    }				   
    TIM1->SR&=~(1<<0);//清除中断标志位 	    
}

/* TIM4中断服务函数 */
void TIM4_IRQHandler(void)
{ 		    		  			    
    if(TIM4->SR&0X0001)//溢出中断
    {    				   				     	    	
    }				   
    TIM4->SR&=~(1<<0);//清除中断标志位 	    
}
