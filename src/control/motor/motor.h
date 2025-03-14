#ifndef __MOTOR_H
#define __MOTOR_H

#include "base_type.h"
#include "tb6612.h"

enum motor_id_e {
	FL_MOTOR,
	FR_MOTOR,
	BL_MOTOR,
	BR_MOTOR,
	MOTOR_NUM
};

enum motor_dir_e {
	MOTOR_FORWARD = TB6612_FORWARD,
	MOTOR_BACK = TB6612_BACK,
	MOTOR_STOP = TB6612_STOP
};

//电机最大PWM限制
#define MAX_PWM			3200

//电机状态记录结构体
typedef struct{
	uint8_t id;			    //电机编号
	uint8_t dir;			//电机方向, enum tb6612_dir_e
	int16_t pwm;			//电机pwm    pwm为负代表反向
	int16_t encoder;		//电机编码器值
	int32_t encCnter;		//电机累计编码值
	float distance;			//电机路程（经典轮胎）- mm	DEBUG使用
	float dist_1mm;			//电机带动轮胎1mm路程编码值
	bool is_stop;			//电机停止标志  1-停止  0-正常
} sMotor;

void motor_init(void);
void motor_print(uint8_t id);
void motor_set_dir(uint8_t id, uint8_t dir);
uint8_t motor_get_dir_val(uint8_t id);
void motor_update_dir_val(uint8_t id);
void motor_set_pwm(uint8_t id, int16_t pwm);
int16_t motor_get_pwm_val(uint8_t id);
void motor_set_pwm_val(uint8_t id, int16_t pwm);
void motor_inc_pwm_val(uint8_t id, int16_t pwm);
void motor_update_encoder(void);
int16_t motor_get_encoder_val(uint8_t id);
float motor_get_distance(uint8_t id);
void motor_set_stop(uint8_t id);
void motor_cancel_stop(uint8_t id);

#endif