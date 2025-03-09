#include "pid.h"
#include "motor.h"
#include "k2102.h"
#include "usart.h"

//4个轮子的PID
sMotorPid fl_vpid;
sMotorPid fr_vpid;
sMotorPid bl_vpid;
sMotorPid br_vpid;

//PID初始化
void Pid_Init()
{
	sMotorPid* vpid[4] = {&fl_vpid, &fr_vpid, &bl_vpid, &br_vpid};
	for(uint8_t i=0; i<4; i++){
		vpid[i]->target_val = 0; vpid[i]->actual_val = 0;
		vpid[i]->err = 0; vpid[i]->err_next = 0; vpid[i]->err_last = 0;
		vpid[i]->Kp = INIT_KP; vpid[i]->Ki = INIT_KI; vpid[i]->Kd = INIT_KD;
	}
}

//PID重新初始化
void Pid_DeInit()
{
	sMotorPid* vpid[4] = {&fl_vpid, &fr_vpid, &bl_vpid, &br_vpid};
	for(uint8_t i=0; i<4; i++){
		vpid[i]->target_val = 0; vpid[i]->actual_val = 0;
		vpid[i]->err = 0; vpid[i]->err_next = 0; vpid[i]->err_last = 0;
	}
}

//设置电机PID目标值 - 一个周期的编码值
//num - 电机编号
//encoderVal - 为正即正转，为负即反转
void Pid_SetMotorSpeed(uint8_t num, int16_t encoderVal)
{
	if(encoderVal>MAX_ENCODER){		//限制电机最大速度
		encoderVal = MAX_ENCODER;
	}
	switch(num){
		case FL_MOTOR: fl_vpid.target_val = encoderVal; break;
		case FR_MOTOR: fr_vpid.target_val = encoderVal; break;
		case BL_MOTOR: bl_vpid.target_val = encoderVal; break;
		case BR_MOTOR: br_vpid.target_val = encoderVal; break;
	}
}

//设置小车4个电机PID目标值
void Pid_SetCarSpeed(int16_t encoderVal)
{
	if(encoderVal>MAX_ENCODER){		//限制电机最大速度
		encoderVal = MAX_ENCODER;
	}
	fl_vpid.target_val = encoderVal;
	fr_vpid.target_val = encoderVal;
	bl_vpid.target_val = encoderVal;
	br_vpid.target_val = encoderVal;
}

//设置小车4个电机PID目标值
void Pid_SetRoundSpeed(int16_t encoderVal)
{
	if(encoderVal>MAX_ENCODER){		//限制电机最大速度
		encoderVal = MAX_ENCODER;
	}
	fl_vpid.target_val = encoderVal;
	fr_vpid.target_val = -encoderVal;
	bl_vpid.target_val = encoderVal;
	br_vpid.target_val = -encoderVal;
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
void Pid_SetCarSpeedByGray_Simple(int16_t encoderVal)
{
	static float Kp = 0.55, Kd = 6.5;
	static float cur_err=0, next_err=0;		//偏差加上->速度PD控制

	int8_t feedback = 0;
		
	if(encoderVal>MAX_ENCODER){		//限制电机最大速度
		encoderVal = MAX_ENCODER;
	}
	next_err = cur_err;
	cur_err = carOffset;
	feedback = Kp*carOffset + Kd*(cur_err-next_err);
	fl_vpid.target_val = bl_vpid.target_val = encoderVal + feedback;
	fr_vpid.target_val = br_vpid.target_val = encoderVal - feedback;
}

//设置电机PID参数值
void Pid_SetMotorPara(uint8_t num, float Kp, float Ki, float Kd)
{
	switch(num){
		case FL_MOTOR: fl_vpid.Kp = Kp; fl_vpid.Ki = Ki; fl_vpid.Kd = Kd; break;
		case FR_MOTOR: fr_vpid.Kp = Kp; fr_vpid.Ki = Ki; fr_vpid.Kd = Kd; break;
		case BL_MOTOR: bl_vpid.Kp = Kp; bl_vpid.Ki = Ki; bl_vpid.Kd = Kd; break;
		case BR_MOTOR: br_vpid.Kp = Kp; br_vpid.Ki = Ki; br_vpid.Kd = Kd; break;
	}
}

//得到电机PID实际值 - 一个周期的编码值
int Pid_GetMotorActualSpeed(uint8_t num)
{
	if(num==FL_MOTOR){
		return fl_vpid.actual_val;
	}else if(num==FR_MOTOR){
		return fr_vpid.actual_val;
	}else if(num==BL_MOTOR){
		return bl_vpid.actual_val;
	}else if(num==BR_MOTOR){
		return br_vpid.actual_val;
	}
	return 0;
}

//电机PID计算
//输入 - 本周期的编码值
//输出 - pwm的增量
int16_t Pid_GetIncPwm(uint8_t num, int encoder)
{
	float increment_val;	//调整增量
	sMotorPid* vpid;
	switch(num){
		case FL_MOTOR: vpid = &fl_vpid; break;
		case FR_MOTOR: vpid = &fr_vpid; break;
		case BL_MOTOR: vpid = &bl_vpid; break;
		case BR_MOTOR: vpid = &br_vpid; break;
	}
	/*获得实际值*/
	vpid->actual_val = encoder;
	/*计算目标值与实际值的误差*/
	vpid->err = vpid->target_val - vpid->actual_val;
	/*--------------------------PID算法实现-------------------------*/
	
	increment_val = vpid->Kp*(vpid->err-vpid->err_next) + vpid->Ki*vpid->err + vpid->Kd*(vpid->err-2*vpid->err_next+vpid->err_last);
	
	/*--------------------------------------------------------------*/	
	/*传递误差*/
	vpid->err_last = vpid->err_next;
	vpid->err_next = vpid->err;
	if(num==BL_MOTOR){
//		printf("  #");printf(" %d \r\n", (int)increment_val);
	}
	/*返回增量*/
	return (int)increment_val;
}
