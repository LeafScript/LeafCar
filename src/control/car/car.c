#include "car.h"
#include "motor.h"
#include "carmode.h"
#include "cartask.h"
#include "pid.h"
#include <stdio.h>

extern sMotor FLMotor;
extern sMotor FRMotor;
extern sMotor BLMotor;
extern sMotor BRMotor;


//小车初始化
void Car_Init()
{
	Motor_Init();
	Car_SetTask();
	CarMode_Init();
}

//小车停止
void Car_Stop()
{
	Motor_Stop(&FLMotor); Motor_Stop(&FRMotor); Motor_Stop(&BLMotor); Motor_Stop(&BRMotor);
	Pid_DeInit();
}

//小车启动
void Car_Start()
{
	Motor_Start(&FLMotor); Motor_Start(&FRMotor); Motor_Start(&BLMotor); Motor_Start(&BRMotor);
}

//小车扫描
void Car_Scan()
{
	//更新电机encoder数值
	Motor_GetEncoder();
	//更新电机路程
	Motor_GetDistance(&FLMotor); Motor_GetDistance(&FRMotor); Motor_GetDistance(&BLMotor); Motor_GetDistance(&BRMotor);
	/*-------------------更新电机PWM-------------------*/
	
	CarMode_Scan();
//	Car_SetPwm(FORWARD,1000);
//	Motor_UpdatePwm(&FRMotor, 1000);
	
	/*-------------------------------------------------*/
	//更新电机方向 - pwm
	Motor_GetDir(&FLMotor); Motor_GetDir(&FRMotor); Motor_GetDir(&BLMotor); Motor_GetDir(&BRMotor);
	//根据上面数值配置电机
	Motor_SetPwm(&FLMotor); Motor_SetDir(&FLMotor);
	Motor_SetPwm(&FRMotor); Motor_SetDir(&FRMotor);
	Motor_SetPwm(&BLMotor); Motor_SetDir(&BLMotor);
	Motor_SetPwm(&BRMotor); Motor_SetDir(&BRMotor);
}

//小车状态打印
void Car_Print()
{
	Motor_Print(&FLMotor);
	Motor_Print(&FRMotor);
	Motor_Print(&BLMotor);
	Motor_Print(&BRMotor);
	printf("\r\n---------------------------------------------------------");
}

//设置小车4个电机PWM以及方向
void Car_SetPwm(uint8_t dir, int16_t pwm)
{
	if(dir==FORWARD){
		Motor_UpdatePwm(&FLMotor,pwm);
		Motor_UpdatePwm(&FRMotor,pwm);
		Motor_UpdatePwm(&BLMotor,pwm);
		Motor_UpdatePwm(&BRMotor,pwm);
	}else if(dir==BACK){
		Motor_UpdatePwm(&FLMotor,-pwm);
		Motor_UpdatePwm(&FRMotor,-pwm);
		Motor_UpdatePwm(&BLMotor,-pwm);
		Motor_UpdatePwm(&BRMotor,-pwm);
	}
}
