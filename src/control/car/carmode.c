#include "carmode.h"
#include "motor.h"
#include "pid.h"
#include "usart.h"

#include "taskconf.h"

extern void Car_Stop(void);
extern void Car_Start(void);

sCar MyCar;

//小车模式初始化
void CarMode_Init()
{
	//Pid初始化
	Pid_Init();
	//小车结构体初始化
//	Config_TrackMove();
//	Car_Track(2000,400);
//	Config_FreeMove();
//	Car_Round(100,100,1);
//	CarMode_Choose(ROUND_MOVE);
	Config_StopMove();
//	Car_Forward(2500,1800);
}

//小车模式选择
void CarMode_Choose(uint8_t mode)
{
	switch(mode){
		case STOP_MOVE: Config_StopMove(); break;
		case FREE_MOVE: Config_FreeMove(); break;
		case TARGET_MOVE: Config_TargetMove(); break;
		case TRACK_MOVE: Config_TrackMove(); break;
		case ROUND_MOVE: Config_RoundMove(); break;
	}
}

//小车模式扫描
void CarMode_Scan()
{
	CarMode_FreeMove();
	CarMode_TargetMove();
	CarMode_TrackMove();
	CarMode_RoundMove();
}

/*--------------------------------------FREE_MOVE--------------------------------------------*/

//FREE_MOVE 更新电机PWM
void CarMode_FreeMove()
{
	static uint16_t timer = 0;
	static uint8_t dir_switch = 0;
	
	if(MyCar.mode!=FREE_MOVE){
		return;
	}
	// 3s改变一次方向
	if(timer==(uint16_t)CAR_PERIOD_TIMES*30){		
		if(dir_switch==0){
			MyCar.config.freeMove.dir = MOTOR_FORWARD;
			int16_t encVal = 100.0/CAR_PERIOD_TIMES;
			Pid_SetCarSpeed(encVal);
			Car_Start();
			dir_switch = 1;
		}else if(dir_switch==1){
			MyCar.config.freeMove.dir = MOTOR_BACK;
			int16_t encVal = -100.0/CAR_PERIOD_TIMES;
			Pid_SetCarSpeed(encVal);
			Car_Start();
			dir_switch = 2;
		}else if(dir_switch==2){
			MyCar.config.freeMove.dir = MOTOR_STOP;
			Car_Stop();
			dir_switch = 0;
		}
		timer = 0;
	}else{
		timer++;
	}
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	MyCar.config.freeMove.speed = CAR_PERIOD_TIMES * (Pid_GetMotorActualSpeed(FL_MOTOR)+Pid_GetMotorActualSpeed(FR_MOTOR)) / 2.0;
	//小车路程 = (左前轮+右前轮)/2
	MyCar.config.freeMove.distance = (motor_get_distance(FL_MOTOR)+motor_get_distance(FR_MOTOR)) / 2.0;
	//更新电机PWM
	motor_inc_pwm_val(FL_MOTOR, Pid_GetIncPwm(FL_MOTOR, motor_get_encoder_val(FL_MOTOR)));
	motor_inc_pwm_val(FR_MOTOR, Pid_GetIncPwm(FR_MOTOR, -motor_get_encoder_val(FR_MOTOR)));
	motor_inc_pwm_val(BL_MOTOR, Pid_GetIncPwm(BL_MOTOR, motor_get_encoder_val(BL_MOTOR)));
	motor_inc_pwm_val(BR_MOTOR, Pid_GetIncPwm(BR_MOTOR, -motor_get_encoder_val(BR_MOTOR)));
}

/*--------------------------------------TARGET_MOVE--------------------------------------------*/

//小车向前
//dis - 距离 mm
//speed - 100ms编码器计数
void Car_Forward(uint16_t dis, uint16_t speed)
{
	MyCar.mode = TARGET_MOVE;
	MyCar.config.targetMove.dir = MOTOR_FORWARD;
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	MyCar.config.targetMove.speed = CAR_PERIOD_TIMES * (Pid_GetMotorActualSpeed(FL_MOTOR)+Pid_GetMotorActualSpeed(FL_MOTOR)) / 2.0;
	//小车路程 = (左前轮+右前轮)/2
	MyCar.config.targetMove.distance = (motor_get_distance(FL_MOTOR)+motor_get_distance(FR_MOTOR)) / 2.0;
	MyCar.config.targetMove.targetDist = MyCar.config.targetMove.distance;
	MyCar.config.targetMove.targetDist += dis;
	int16_t encVal = (float)speed/CAR_PERIOD_TIMES;
	Pid_SetCarSpeed(encVal);
	Car_Start();
}

//小车向前   加补偿
//dis - 距离 mm
//speed - 100ms编码器计数
void Car_Forward_Muti(uint16_t dis, uint16_t speed, int16_t dismuti)
{
	MyCar.mode = TARGET_MOVE;
	MyCar.config.targetMove.dir = MOTOR_FORWARD;
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	MyCar.config.targetMove.speed = CAR_PERIOD_TIMES * (Pid_GetMotorActualSpeed(FL_MOTOR)+Pid_GetMotorActualSpeed(FL_MOTOR)) / 2.0;
	//小车路程 = (左前轮+右前轮)/2
	MyCar.config.targetMove.distance = (motor_get_distance(FL_MOTOR)+motor_get_distance(FR_MOTOR)) / 2.0;
	MyCar.config.targetMove.targetDist = MyCar.config.targetMove.distance;
	DISMUTI_JUDGE;
	MyCar.config.targetMove.targetDist += dis+dismuti;
	int16_t encVal = (float)speed/CAR_PERIOD_TIMES;
	Pid_SetCarSpeed(encVal);
	Car_Start();
}

//小车向后
//dis - 距离 mm
//speed - 100ms编码器计数
void Car_Back(uint16_t dis, uint16_t speed)
{
	MyCar.mode = TARGET_MOVE;
	MyCar.config.targetMove.dir = MOTOR_BACK;
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	MyCar.config.targetMove.speed = CAR_PERIOD_TIMES * (Pid_GetMotorActualSpeed(FL_MOTOR)+Pid_GetMotorActualSpeed(FL_MOTOR)) / 2.0;
	//小车路程 = (左前轮+右前轮)/2
	MyCar.config.targetMove.distance = (motor_get_distance(FL_MOTOR)+motor_get_distance(FR_MOTOR)) / 2.0;
	MyCar.config.targetMove.targetDist = MyCar.config.targetMove.distance;
	MyCar.config.targetMove.targetDist -= dis;
	int16_t encVal = (float)-speed/CAR_PERIOD_TIMES;
	Pid_SetCarSpeed(encVal);
	Car_Start();
}

//小车向后   加补偿
//dis - 距离 mm
//speed - 100ms编码器计数
void Car_Back_Muti(uint16_t dis, uint16_t speed, int16_t dismuti)
{
	MyCar.mode = TARGET_MOVE;
	MyCar.config.targetMove.dir = MOTOR_BACK;
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	MyCar.config.targetMove.speed = CAR_PERIOD_TIMES * (Pid_GetMotorActualSpeed(FL_MOTOR)+Pid_GetMotorActualSpeed(FL_MOTOR)) / 2.0;
	//小车路程 = (左前轮+右前轮)/2
	MyCar.config.targetMove.distance = (motor_get_distance(FL_MOTOR)+motor_get_distance(FR_MOTOR)) / 2.0;
	MyCar.config.targetMove.targetDist = MyCar.config.targetMove.distance;
	DISMUTI_JUDGE;
	MyCar.config.targetMove.targetDist -= dis+dismuti;
	int16_t encVal = (float)-speed/CAR_PERIOD_TIMES;
	Pid_SetCarSpeed(encVal);
	Car_Start();
}

//TARGET_MOVE 更新电机PWM
void CarMode_TargetMove()
{
	if(MyCar.mode!=TARGET_MOVE){
		return;
	}
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	MyCar.config.targetMove.speed = CAR_PERIOD_TIMES * (Pid_GetMotorActualSpeed(FL_MOTOR)+Pid_GetMotorActualSpeed(FL_MOTOR)) / 2.0;
	//小车路程 = (左前轮+右前轮)/2
	MyCar.config.targetMove.distance = (motor_get_distance(FL_MOTOR)+motor_get_distance(FR_MOTOR)) / 2.0;
	//达到目标路程，停止
	if(MyCar.config.targetMove.dir==MOTOR_FORWARD && MyCar.config.targetMove.distance>MyCar.config.targetMove.targetDist){	//向前
		MyCar.mode = STOP_MOVE; Car_Stop();
	}
	if(MyCar.config.targetMove.dir==MOTOR_BACK && MyCar.config.targetMove.distance<MyCar.config.targetMove.targetDist){		//向后
		MyCar.mode = STOP_MOVE; Car_Stop();
	}
	//更新电机PWM
	motor_inc_pwm_val(FL_MOTOR, Pid_GetIncPwm(FL_MOTOR,motor_get_encoder_val(FL_MOTOR)));
	motor_inc_pwm_val(FR_MOTOR, Pid_GetIncPwm(FR_MOTOR,-motor_get_encoder_val(FR_MOTOR)));
	motor_inc_pwm_val(BL_MOTOR, Pid_GetIncPwm(BL_MOTOR,motor_get_encoder_val(BL_MOTOR)));
	motor_inc_pwm_val(BR_MOTOR, Pid_GetIncPwm(BR_MOTOR,-motor_get_encoder_val(BR_MOTOR)));
}

/*--------------------------------------ROUND_MOVE--------------------------------------------*/

//小车旋转
//dis - 距离 mm
//speed - 100ms编码器计数
//dir - 1:顺  0:逆
void Car_Round(uint16_t dis, uint16_t speed, uint8_t dir)
{
	MyCar.mode = ROUND_MOVE;
	if(dir==1){
		MyCar.config.roundMove.dir = RIGHT_ROUND;
	}else{
		MyCar.config.roundMove.dir = LEFT_ROUND;
	}
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	MyCar.config.roundMove.speed = CAR_PERIOD_TIMES * (Pid_GetMotorActualSpeed(FL_MOTOR)+Pid_GetMotorActualSpeed(FL_MOTOR)) / 2.0;
	//小车路程 = (左前轮+右前轮)/2
	MyCar.config.roundMove.distance = (motor_get_distance(FL_MOTOR)-motor_get_distance(FR_MOTOR)) / 2.0;
	MyCar.config.roundMove.targetDist = MyCar.config.roundMove.distance;
	int16_t encVal;
	if(dir==1){
		MyCar.config.roundMove.targetDist += dis;
		encVal = (float)speed/CAR_PERIOD_TIMES;
	}else{
		MyCar.config.roundMove.targetDist -= dis;
		encVal = (float)-speed/CAR_PERIOD_TIMES;
	}
	Pid_SetRoundSpeed(encVal);
	Car_Start();
}

//小车旋转
//dis - 距离 mm
//speed - 100ms编码器计数
//dir - 1:顺  0:逆
void Car_Round_Muti(uint16_t dis, uint16_t speed, uint8_t dir, int16_t dismuti)
{
	MyCar.mode = ROUND_MOVE;
	if(dir==1){
		MyCar.config.roundMove.dir = RIGHT_ROUND;
	}else{
		MyCar.config.roundMove.dir = LEFT_ROUND;
	}
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	MyCar.config.roundMove.speed = CAR_PERIOD_TIMES * (Pid_GetMotorActualSpeed(FL_MOTOR)+Pid_GetMotorActualSpeed(FL_MOTOR)) / 2.0;
	//小车路程 = (左前轮+右前轮)/2
	MyCar.config.roundMove.distance = (motor_get_distance(FL_MOTOR)-motor_get_distance(FR_MOTOR)) / 2.0;
	MyCar.config.roundMove.targetDist = MyCar.config.roundMove.distance;
	int16_t encVal;
	if(dir==1){
		DISMUTI_JUDGE;
		MyCar.config.roundMove.targetDist += dis + dismuti;
		encVal = (float)speed/CAR_PERIOD_TIMES;
	}else{
		DISMUTI_JUDGE;
		MyCar.config.roundMove.targetDist -= dis + dismuti;
		encVal = (float)-speed/CAR_PERIOD_TIMES;
	}
	Pid_SetRoundSpeed(encVal);
	Car_Start();
}

//ROUND_MOVE 更新电机PWM
void CarMode_RoundMove()
{
	if(MyCar.mode!=ROUND_MOVE){
		return;
	}
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	MyCar.config.roundMove.speed = CAR_PERIOD_TIMES * (Pid_GetMotorActualSpeed(FL_MOTOR)+Pid_GetMotorActualSpeed(FL_MOTOR)) / 2.0;
	//小车路程 = (左前轮+右前轮)/2
	MyCar.config.roundMove.distance = (motor_get_distance(FL_MOTOR)-motor_get_distance(FR_MOTOR)) / 2.0;
//	USART1_printf("distance=%.2f  %.2f  %.2f\r\n",MyCar.config.roundMove.distance, motor_get_distance(FL_MOTOR), motor_get_distance(FR_MOTOR));
	//达到目标路程，停止
	if(MyCar.config.roundMove.dir==RIGHT_ROUND && MyCar.config.roundMove.distance>MyCar.config.roundMove.targetDist){	//顺时针
		MyCar.mode = STOP_MOVE; Car_Stop();
	}
	if(MyCar.config.roundMove.dir==LEFT_ROUND && MyCar.config.roundMove.distance<MyCar.config.roundMove.targetDist){		//逆时针
		MyCar.mode = STOP_MOVE; Car_Stop();
	}
	//更新电机PWM
	motor_inc_pwm_val(FL_MOTOR, Pid_GetIncPwm(FL_MOTOR,motor_get_encoder_val(FL_MOTOR)));
	motor_inc_pwm_val(FR_MOTOR, Pid_GetIncPwm(FR_MOTOR,-motor_get_encoder_val(FR_MOTOR)));
	motor_inc_pwm_val(BL_MOTOR, Pid_GetIncPwm(BL_MOTOR,motor_get_encoder_val(BL_MOTOR)));
	motor_inc_pwm_val(BR_MOTOR, Pid_GetIncPwm(BR_MOTOR,-motor_get_encoder_val(BR_MOTOR)));
}

/*--------------------------------------TRACK_MOVE--------------------------------------------*/

//小车循迹
//dis - 距离 mm
//speed - 100ms编码器计数
//dir - 1:前  0:后
void Car_Track(uint32_t dis, uint16_t speed, uint8_t dir)
{
	MyCar.mode = TRACK_MOVE;
	if(dir==1){
		MyCar.config.trackMove.dir = MOTOR_FORWARD;
	}else{
		MyCar.config.trackMove.dir = MOTOR_BACK;
	}	
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	MyCar.config.trackMove.speed = CAR_PERIOD_TIMES * (Pid_GetMotorActualSpeed(FL_MOTOR)+Pid_GetMotorActualSpeed(FL_MOTOR)) / 2.0;
	//小车路程 = (左前轮+右前轮)/2
	MyCar.config.trackMove.distance = (motor_get_distance(FL_MOTOR)+motor_get_distance(FR_MOTOR)) / 2.0;
	MyCar.config.trackMove.turnDist = MyCar.config.trackMove.distance;
	int16_t encVal;
	if(dir==1){
		MyCar.config.trackMove.turnDist += dis;
		encVal = (float)speed/CAR_PERIOD_TIMES;
	}else{
		MyCar.config.trackMove.turnDist -= dis;
		encVal = (float)-speed/CAR_PERIOD_TIMES;
	}
	MyCar.config.trackMove.encVal = encVal;
	Pid_SetCarSpeed(encVal);
	Car_Start();
}

//小车循迹
//dis - 距离 mm
//speed - 100ms编码器计数
//dir - 1:前  0:后
//dismuti - 1m（1000mm）距离小车补偿距离 - mm
void Car_Track_Muti(uint32_t dis, uint16_t speed, uint8_t dir, int16_t dismuti)
{
	MyCar.mode = TRACK_MOVE;
	if(dir==1){
		MyCar.config.trackMove.dir = MOTOR_FORWARD;
	}else{
		MyCar.config.trackMove.dir = MOTOR_BACK;
	}	
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	MyCar.config.trackMove.speed = CAR_PERIOD_TIMES * (Pid_GetMotorActualSpeed(FL_MOTOR)+Pid_GetMotorActualSpeed(FL_MOTOR)) / 2.0;
	//小车路程 = (左前轮+右前轮)/2
	MyCar.config.trackMove.distance = (motor_get_distance(FL_MOTOR)+motor_get_distance(FR_MOTOR)) / 2.0;
	MyCar.config.trackMove.turnDist = MyCar.config.trackMove.distance;
	int16_t encVal;
	if(dir==1){
		DISMUTI_JUDGE;
		MyCar.config.trackMove.turnDist += dis + dismuti;
		encVal = (float)speed/CAR_PERIOD_TIMES;
	}else{
		DISMUTI_JUDGE;
		MyCar.config.trackMove.turnDist -= dis + dismuti;
		encVal = (float)-speed/CAR_PERIOD_TIMES;
	}
	MyCar.config.trackMove.encVal = encVal;
	Pid_SetCarSpeed(encVal);
	Car_Start();
}

//TRACK_MOVE 更新电机PWM
void CarMode_TrackMove()
{
	if(MyCar.mode!=TRACK_MOVE){
		return;
	}
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	MyCar.config.trackMove.speed = CAR_PERIOD_TIMES * (Pid_GetMotorActualSpeed(FL_MOTOR)+Pid_GetMotorActualSpeed(FL_MOTOR)) / 2.0;
	//小车路程 = (左前轮+右前轮)/2
	MyCar.config.trackMove.distance = (motor_get_distance(FL_MOTOR)+motor_get_distance(FR_MOTOR)) / 2.0;
	//达到目标路程，停止
	if(MyCar.config.trackMove.dir==MOTOR_FORWARD && MyCar.config.trackMove.distance > MyCar.config.trackMove.turnDist){	//平均距离达到，停止
		MyCar.mode = STOP_MOVE; Car_Stop();
	}
	if(MyCar.config.trackMove.dir==MOTOR_BACK && MyCar.config.trackMove.distance < MyCar.config.trackMove.turnDist){	//平均距离达到，停止
		MyCar.mode = STOP_MOVE; Car_Stop();
	}
	/*---------------------------------循迹函数--------------------------------------*/
	
	Pid_SetCarSpeedByGray_Simple(MyCar.config.trackMove.encVal);
	
	/*-------------------------------------------------------------------------------*/
	//更新电机PWM
	motor_inc_pwm_val(FL_MOTOR, Pid_GetIncPwm(FL_MOTOR,motor_get_encoder_val(FL_MOTOR)));
	motor_inc_pwm_val(FR_MOTOR, Pid_GetIncPwm(FR_MOTOR,-motor_get_encoder_val(FR_MOTOR)));
	motor_inc_pwm_val(BL_MOTOR, Pid_GetIncPwm(BL_MOTOR,motor_get_encoder_val(BL_MOTOR)));
	motor_inc_pwm_val(BR_MOTOR, Pid_GetIncPwm(BR_MOTOR,-motor_get_encoder_val(BR_MOTOR)));
}
