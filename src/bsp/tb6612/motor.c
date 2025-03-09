#include "motor.h"
#include "tb6612.h"
#include "usart.h"
#include <stdlib.h>
#include <math.h>

//4个电机
sMotor FLMotor;
sMotor FRMotor;
sMotor BLMotor;
sMotor BRMotor;

//电机驱动初始化
void Motor_Init()
{
	//驱动初始化
	TB6612_Init();
	//编码器初始化
	Read_Encoder(3); Read_Encoder(8); Read_Encoder(1); Read_Encoder(4);
	//电机记录结构体初始化
	FLMotor.num = FL_MOTOR; FLMotor.dir = STOP; FLMotor.pwm = 0; FLMotor.encoder = 0; FLMotor.encCnter = 0; FLMotor.stopFlag = M_START;
	FLMotor.distance = 0; FLMotor.dist_1mm = FL_DISRANCE;
	FRMotor.num = FR_MOTOR; FRMotor.dir = STOP; FRMotor.pwm = 0; FRMotor.encoder = 0; FRMotor.encCnter = 0; FRMotor.stopFlag = M_START;
	FRMotor.distance = 0; FRMotor.dist_1mm = FR_DISRANCE;
	BLMotor.num = BL_MOTOR; BLMotor.dir = STOP; BLMotor.pwm = 0; BLMotor.encoder = 0; BLMotor.encCnter = 0; BLMotor.stopFlag = M_START;
	BLMotor.distance = 0; BLMotor.dist_1mm = BL_DISRANCE;
	BRMotor.num = BR_MOTOR; BRMotor.dir = STOP; BRMotor.pwm = 0; BRMotor.encoder = 0; BRMotor.encCnter = 0; BRMotor.stopFlag = M_START;
	BRMotor.distance = 0; BRMotor.dist_1mm = BR_DISRANCE;
	//电机配置初始化
	Motor_SetDir(&FLMotor); Motor_SetPwm(&FLMotor);
	Motor_SetDir(&FRMotor); Motor_SetPwm(&FRMotor);
	Motor_SetDir(&BLMotor); Motor_SetPwm(&BLMotor);
	Motor_SetDir(&BRMotor); Motor_SetPwm(&BRMotor);
}

/*---------------------------------------------电机控制--------------------------------------------------*/
//控制电机正反转和停止
//num - 电机编号
//state - 电机状态
void Motor_SetDir(sMotor* motor)
{
	if(motor->stopFlag == M_STOP){
		return;
	}
	if(motor->num == FL_MOTOR){
		switch(motor->dir){
			case FORWARD: FL_AIN1(0); FL_AIN2(1); break;
			case BACK: 	  FL_AIN1(1); FL_AIN2(0); break;
			case STOP: 	  FL_AIN1(1); FL_AIN2(1); break;
		}
	}else if(motor->num == FR_MOTOR){
		switch(motor->dir){
			case FORWARD: FR_BIN1(0); FR_BIN2(1); break;
			case BACK: 	  FR_BIN1(1); FR_BIN2(0); break;
			case STOP: 	  FR_BIN1(1); FR_BIN2(1); break;
		}
	}else if(motor->num == BL_MOTOR){
		switch(motor->dir){
			case FORWARD: BL_BIN1(1); BL_BIN2(0); break;
			case BACK: 	  BL_BIN1(0); BL_BIN2(1); break;
			case STOP: 	  BL_BIN1(1); BL_BIN2(1); break;
		}
	}else if(motor->num == BR_MOTOR){
		switch(motor->dir){
			case FORWARD: BR_AIN1(1); BR_AIN2(0); break;
			case BACK: 	  BR_AIN1(0); BR_AIN2(1); break;
			case STOP: 	  BR_AIN1(1); BR_AIN2(1); break;
		}
	}
}

//配置电机PWM值
//num - 电机编号
//pwm - 设置的PWM值
void Motor_SetPwm(sMotor* motor)
{
	if(motor->stopFlag == M_STOP){
		return;
	}
	if(motor->pwm > MAX_PWM){	// 限幅
		motor->pwm = MAX_PWM;
	}else if(motor->pwm < -MAX_PWM){
		motor->pwm = -MAX_PWM;
	}
	if(motor->num == FL_MOTOR){
		FL_SetPwm(abs(motor->pwm));
	}else if(motor->num == FR_MOTOR){
		FR_SetPwm(abs(motor->pwm));
	}else if(motor->num == BL_MOTOR){
		BL_SetPwm(abs(motor->pwm));
	}else if(motor->num == BR_MOTOR){
		BR_SetPwm(abs(motor->pwm));
	}
}

//更新电机结构体PWM数值，供配置电机使用
//pwm: 设置的pwm值
void Motor_UpdatePwm(sMotor* motor, int16_t pwm)
{
	if(motor->stopFlag == M_STOP){
		return;
	}
	motor->pwm = pwm;
}

//为电机pwm增加一定值
void Motor_IncresePwm(sMotor* motor, int16_t inc_pwm)
{
	if(motor->stopFlag == M_STOP){
		return;
	}
	motor->pwm += inc_pwm;
}

//读取编码器
void Motor_GetEncoder()
{
	FLMotor.encoder = Read_Encoder(3) - 30000; FLMotor.encCnter += FLMotor.encoder;
	FRMotor.encoder = Read_Encoder(8) - 30000; FRMotor.encCnter -= FRMotor.encoder;
	BLMotor.encoder = Read_Encoder(1) - 30000; BLMotor.encCnter += BLMotor.encoder;
	BRMotor.encoder = Read_Encoder(4) - 30000; BRMotor.encCnter -= BRMotor.encoder;
}

//计算电机带动轮子的路程
float Motor_GetDistance(sMotor* motor)
{
	motor->distance = motor->encCnter / motor->dist_1mm;		// 1mm - 31个编码值
	return motor->distance;
}

//判断电机方向 - 根据pwm
void Motor_GetDir(sMotor* motor)
{
	if(motor->pwm > 0) motor->dir = FORWARD;
	else if(motor->pwm < 0) motor->dir = BACK;
	else if(motor->pwm == 0) motor->dir = STOP;
}

//设置电机停止
void Motor_Stop(sMotor* motor)
{
//	Pid_V_DeInit();
	motor->dir = STOP; motor->pwm = 0;
	//配置电机停止
	Motor_SetDir(motor); Motor_SetPwm(motor);
	motor->stopFlag = M_STOP;
}

void Motor_Start(sMotor* motor)
{
	motor->stopFlag = M_START;
}

//打印电机参数
void Motor_Print(sMotor* motor)
{
	printf("\r\n");
	switch(motor->num){
		case FL_MOTOR: printf("FLMotor - "); break;
		case FR_MOTOR: printf("FRMotor - "); break;
		case BL_MOTOR: printf("BLMotor - "); break;
		case BR_MOTOR: printf("BRMotor - "); break;
	}
	printf("pwm:%d - ",motor->pwm);
	if(motor->num==FL_MOTOR || motor->num==BL_MOTOR){
		printf("encoder:%d - ",motor->encoder);
	}else if(motor->num==FR_MOTOR || motor->num==BR_MOTOR){
		printf("encoder:%d - ",-motor->encoder);
	}
	printf("encCnter:%d - ",motor->encCnter);
	printf("distance:%.2f - ",motor->distance);
	switch(motor->dir){
		case FORWARD:	printf("Forward"); break;
        case BACK:      printf("Back");    break;
        case STOP:      printf("Stop");    break;
	}
}