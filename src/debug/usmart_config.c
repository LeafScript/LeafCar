#include "usmart.h"
#include "usmart_str.h" 

////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加) 
#include "delay.h"		
#include "sys.h"
#include "led.h"
#include "car_basic.h"
#include "car_ctrl.h"
#include "car_op.h"
#include "pid.h"
#include "arm.h"
	 
extern void led_set(u8 sta);
extern void test_fun(void(*ledset)(u8),u8 sta);
 
//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif
	(void*)delay_ms,"void delay_ms(u16 nms)",
	(void*)delay_us,"void delay_us(u32 nus)",	
	(void*)LED_Toggle,"void LED_Toggle(uint8_t num)",
	//小车基础操作
	(void*)car_stop,"void car_stop(void)",
	(void*)car_start,"void car_start(void)",
	(void*)car_print,"void car_print(void)",
	(void*)car_set_pwm,"void car_set_pwm(uint8_t dir, int16_t pwm)",
	(void*)car_ctrl_set_mode,"void car_ctrl_set_mode(uint8_t mode)",
	(void*)car_forward,"void car_forward(uint16_t dist, uint16_t speed)",
	(void*)car_back,"void car_back(uint16_t dist, uint16_t speed)",
	(void*)car_turn,"void car_turn(uint16_t dist, uint16_t speed, uint8_t dir)",
	(void*)car_track,"void car_track(uint32_t dist, uint16_t speed, uint8_t dir)",
		
	//pid
	(void*)vpid_set_motor_param,"void vpid_set_motor_param(uint8_t id, float Kp, float Ki, float Kd)",
		
	(void*)Arm_SetStatus,"void Arm_SetStatus(uint8_t status)",
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};