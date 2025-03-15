#include "pid.h"
#include "motor.h"
#include "k2102.h"
#include "usart.h"
#include "base_type.h"

////经典小车
//#define INIT_KP		1.25
//#define INIT_KI		2.85
//#define INIT_KD		0.3

//小蓝电机小车
#define INIT_KP		1.25
#define INIT_KI		2.0
#define INIT_KD		0.4

static sMotorPid g_vpid_ctrl[MOTOR_NUM];

static void vpid_ctrl_init_one(uint8_t id)
{
	sMotorPid *vpid = &g_vpid_ctrl[id];
	vpid->target_val = 0;
	vpid->actual_val = 0;
	vpid->err = 0;
	vpid->err_next = 0;
	vpid->err_last = 0;
	vpid->Kp = INIT_KP;
	vpid->Ki = INIT_KI;
	vpid->Kd = INIT_KD;
}

void vpid_ctrl_init(void)
{
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		vpid_ctrl_init_one(id);
	}
}

static void vpid_ctrl_deinit_one(uint8_t id)
{
	sMotorPid *vpid = &g_vpid_ctrl[id];
	vpid->target_val = 0;
	vpid->actual_val = 0;
	vpid->err = 0;
	vpid->err_next = 0;
	vpid->err_last = 0;
}

void vpid_ctrl_deinit(void)
{
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		vpid_ctrl_deinit_one(id);
	}
}

//设置电机PID目标值 - 一个周期的编码值
//id - 电机编号
//encoderVal - 为正即正转，为负即反转
void vpid_set_motor_speed(uint8_t id, int16_t encoderVal)
{
	g_vpid_ctrl[id].target_val = MIN(encoderVal, MAX_ENCODER);	//限制电机最大速度
}

//设置小车4个电机PID目标值
void vpid_set_car_speed(int16_t encoderVal)
{
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		g_vpid_ctrl[id].target_val = MIN(encoderVal, MAX_ENCODER);	//限制电机最大速度
	}
}

//设置小车4个电机PID目标值
void vpid_set_round_speed(int16_t encoderVal)
{
	int16_t val = MIN(encoderVal, MAX_ENCODER);
	g_vpid_ctrl[FL_MOTOR].target_val = val;
	g_vpid_ctrl[FR_MOTOR].target_val = -val;
	g_vpid_ctrl[BL_MOTOR].target_val = val;
	g_vpid_ctrl[BR_MOTOR].target_val = -val;
}

//接收偏移程度
extern int8_t carOffset;
//接收线的"斜率"
extern int8_t line_k;

//有希望的值：
//Kp = 0.07, Kd = 0.03;
//Kp = 0.07, Kd = 0.07;
//Kp = 0.06, Kd = 0.07;
//Kp = 0.08, Kd = 0.07;  较好
//Kp = 0.08, Kd = 0.09;

//Kp = 0.35, Kd = 1;
//Kp = 0.4, Kd = 1.8;
//Kp = 0.5, Kd = 5;    牛 -- 400速度

//Kp = 0.55, Kd = 6.5;  加重版

//循迹时通过检测偏移度设置小车4个电机PID目标值
//精简入门版
void vpid_set_car_speed_by_offset(int16_t encoderVal)
{
	static float Kp = 0.55, Kd = 6.5;
	static float cur_err = 0, next_err = 0;		//偏差加上->速度PD控制
	int8_t feedback = 0;

	next_err = cur_err;
	cur_err = carOffset;
	feedback = Kp * carOffset + Kd * (cur_err - next_err);
	g_vpid_ctrl[FL_MOTOR].target_val = MIN(encoderVal, MAX_ENCODER) + feedback;
	g_vpid_ctrl[FR_MOTOR].target_val = MIN(encoderVal, MAX_ENCODER) - feedback;
	g_vpid_ctrl[BL_MOTOR].target_val = MIN(encoderVal, MAX_ENCODER) + feedback;
	g_vpid_ctrl[BR_MOTOR].target_val = MIN(encoderVal, MAX_ENCODER) - feedback;
}

//设置电机PID参数值
void vpid_set_motor_param(uint8_t id, float Kp, float Ki, float Kd)
{
	if (id >= MOTOR_NUM) {
		return;
	}
	g_vpid_ctrl[id].Kp = Kp;
	g_vpid_ctrl[id].Ki = Ki;
	g_vpid_ctrl[id].Kd = Kd;
}

//得到电机PID实际值 - 一个周期的编码值
int vpid_get_motor_actual_speed(uint8_t id)
{
	if (id >= MOTOR_NUM) {
		return 0;
	}
	return g_vpid_ctrl[id].actual_val;
}

//电机PID计算
//输入 - 本周期的编码值
//输出 - pwm的增量
int16_t vpid_get_inc_pwm(uint8_t id, int encoder)
{
	sMotorPid *vpid = &g_vpid_ctrl[id];
	float inc_val;	//调整增量
	/*获得实际值*/
	vpid->actual_val = encoder;
	/*计算目标值与实际值的误差*/
	vpid->err = vpid->target_val - vpid->actual_val;
	/*--------------------------PID算法实现-------------------------*/
	
	inc_val = vpid->Kp * (vpid->err - vpid->err_next) +
		vpid->Ki * vpid->err +
		vpid->Kd * (vpid->err - 2 * vpid->err_next + vpid->err_last);
	
	/*--------------------------------------------------------------*/	
	/*传递误差*/
	vpid->err_last = vpid->err_next;
	vpid->err_next = vpid->err;
	/*返回增量*/
	return (int16_t)inc_val;
}
