#include "car.h"
#include "motor.h"
#include "carmode.h"
#include "cartask.h"
#include "pid.h"
#include "usart.h"

void Car_Init()
{
	motor_init();
	CarMode_Init();
}

//小车停止
void Car_Stop(void)
{
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		motor_set_stop(id);
	}
	Pid_DeInit();
}

//小车启动
void Car_Start(void)
{
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		motor_cancel_stop(id);
	}
}

//小车扫描
void Car_Scan()
{
	uint8_t id;
	//更新电机encoder数值
	motor_update_encoder();
	/*-------------------更新电机PWM-------------------*/
	
	CarMode_Scan();
	
	/*-------------------------------------------------*/
	//更新电机方向 - pwm
	for (id = 0; id < MOTOR_NUM; id++) {
		motor_update_dir_val(id);
		motor_set_pwm(id, motor_get_pwm_val(id));
		motor_set_dir(id, motor_get_dir_val(id));
	}
}

//小车状态打印
void Car_Print()
{
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		motor_print(id);
	}
}

//设置小车4个电机PWM以及方向
void Car_SetPwm(uint8_t dir, int16_t pwm)
{
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		motor_set_pwm_val(id, pwm);
	}
}
