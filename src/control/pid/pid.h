#ifndef __PID_H
#define __PID_H

#include "sys.h"

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
} sMotorPid;

void vpid_ctrl_init(void);
void vpid_ctrl_deinit(void);
void vpid_set_motor_speed(uint8_t id, int16_t enc_val);
void vpid_set_car_speed(int16_t enc_val);
void vpid_set_round_speed(int16_t enc_val);
void vpid_set_car_speed_by_offset(int16_t enc_val);
void vpid_set_motor_param(uint8_t id, float Kp, float Ki, float Kd);
int vpid_get_motor_actual_speed(uint8_t id);
int16_t vpid_get_inc_pwm(uint8_t id, int encoder);

#endif