#ifndef __PID_H
#define __PID_H

#include "sys.h"

////经典小车
//#define INIT_KP		1.25
//#define INIT_KI		2.85
//#define INIT_KD		0.3

//小蓝电机小车
#define INIT_KP		1.25
#define INIT_KI		2.0
#define INIT_KD		0.4

//限制一个周期最大编码值 100
#define MAX_ENCODER  1000

//速度的增量式PID结构体
typedef struct{
	int target_val;   	// 编码器目标值
	int actual_val;   	// 编码器实际值
	int err;          	// 当前偏差值
	int err_next;     	// 下一个偏差值
	int err_last;     	// 最后一个偏差值
	float Kp,Ki,Kd;		// 比例、积分、微分系数		->  增量式其实是 微分、比例、二阶微分
						// 当前误差、上次误差、上上次误差系数	Q0=Kp+Ki+Kd    Q1=-Kp-2Kd    Q2=Kd
}sMotorPid;

void Pid_Init(void);
void Pid_DeInit(void);
void Pid_SetMotorSpeed(uint8_t num, int16_t encoderVal);
void Pid_SetCarSpeed(int16_t encoderVal);
void Pid_SetRoundSpeed(int16_t encoderVal);
void Pid_SetCarSpeedByGray_Simple(int16_t encoderVal);
void Pid_SetMotorPara(uint8_t num, float Kp, float Ki, float Kd);
int Pid_GetMotorActualSpeed(uint8_t num);
int16_t Pid_GetIncPwm(uint8_t num, int encoder);

#endif