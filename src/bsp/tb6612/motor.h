#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

//控制方向引脚
#define FL_AIN1(output) if(output) GPIO_SetBits(DIR_PORT1,FL_DIR_AIN1); else GPIO_ResetBits(DIR_PORT1,FL_DIR_AIN1)
#define FL_AIN2(output) if(output) GPIO_SetBits(DIR_PORT1,FL_DIR_AIN2); else GPIO_ResetBits(DIR_PORT1,FL_DIR_AIN2)
#define FR_BIN1(output) if(output) GPIO_SetBits(DIR_PORT1,FR_DIR_BIN1); else GPIO_ResetBits(DIR_PORT1,FR_DIR_BIN1)
#define FR_BIN2(output) if(output) GPIO_SetBits(DIR_PORT1,FR_DIR_BIN2); else GPIO_ResetBits(DIR_PORT1,FR_DIR_BIN2)
#define BL_BIN1(output) if(output) GPIO_SetBits(DIR_PORT2,BL_DIR_BIN1); else GPIO_ResetBits(DIR_PORT2,BL_DIR_BIN1)
#define BL_BIN2(output) if(output) GPIO_SetBits(DIR_PORT2,BL_DIR_BIN2); else GPIO_ResetBits(DIR_PORT2,BL_DIR_BIN2)
#define BR_AIN1(output) if(output) GPIO_SetBits(DIR_PORT2,BR_DIR_AIN1); else GPIO_ResetBits(DIR_PORT2,BR_DIR_AIN1)
#define BR_AIN2(output) if(output) GPIO_SetBits(DIR_PORT2,BR_DIR_AIN2); else GPIO_ResetBits(DIR_PORT2,BR_DIR_AIN2)

//设置电机PWM
#define FL_SetPwm(pwm)		TIM_SetCompare1(TIM2,pwm)		//PA15
#define FR_SetPwm(pwm)		TIM_SetCompare2(TIM2,pwm)		//PB3
#define BL_SetPwm(pwm)		TIM_SetCompare3(TIM2,pwm)		//PA2
#define BR_SetPwm(pwm)		TIM_SetCompare4(TIM2,pwm)		//PA3

//电机编号
#define FL_MOTOR	1
#define FR_MOTOR	2
#define BL_MOTOR	3
#define BR_MOTOR	4

//电机状态
#define FORWARD		0
#define BACK		1
#define STOP		2

//电机是否停止
#define M_STOP      0
#define M_START     1

//电机最大PWM限制
#define MAX_PWM			3200

////电机1mm路程编码值（经典轮胎）
//#define FL_DISRANCE		31.1
//#define FR_DISRANCE		30.9
//#define BL_DISRANCE		30.75
//#define BR_DISRANCE		30.55

//电机1mm路程编码值（小蓝电机轮胎）
#define FL_DISRANCE		11.387
#define FR_DISRANCE		11.434
#define BL_DISRANCE		11.465
#define BR_DISRANCE		11.336


//电机状态记录结构体
typedef struct{
	uint8_t num;			//电机编号
	uint8_t dir;			//电机方向
	int16_t pwm;			//电机pwm    pwm为负代表反向
	int16_t encoder;		//电机编码器值
	int32_t encCnter;		//电机累计编码值
	float distance;			//电机路程（经典轮胎）- mm
	float dist_1mm;			//电机带动轮胎1mm路程编码值
	uint8_t stopFlag;		//电机停止标志  1-停止  0-正常
}sMotor;


void Motor_Init(void);
void Motor_Print(sMotor* motor);
void Motor_SetDir(sMotor* motor);
void Motor_SetPwm(sMotor* motor);
void Motor_UpdatePwm(sMotor* motor, int16_t pwm);
void Motor_IncresePwm(sMotor* motor, int16_t inc_pwm);
void Motor_GetEncoder(void);
void Motor_GetDir(sMotor* motor);
float Motor_GetDistance(sMotor* motor);
void Motor_Stop(sMotor* motor);
void Motor_Start(sMotor* motor);

#endif