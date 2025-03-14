#include "motor.h"
#include "tb6612.h"
#include "usart.h"
#include <stdlib.h>
#include <math.h>

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

static sMotor g_motor_ctrl[MOTOR_NUM];
static float g_motor_dist_1mm[MOTOR_NUM] = {
	FL_DISRANCE, FR_DISRANCE, BL_DISRANCE, BR_DISRANCE
};
static uint8_t g_motor_tb6612_map[] = {
	[FL_MOTOR] = TB6612_ID_0,
	[FR_MOTOR] = TB6612_ID_1,
	[BL_MOTOR] = TB6612_ID_2,
	[BR_MOTOR] = TB6612_ID_3
};

void motor_set_dir(uint8_t id, uint8_t dir)
{
	if (g_motor_ctrl[id].is_stop) {
		return;
	}
	tb6612_set_dir(g_motor_tb6612_map[id], dir);
}

uint8_t motor_get_dir_val(uint8_t id)
{
	return g_motor_ctrl[id].dir;
}

void motor_update_dir_val(uint8_t id)
{
	int16_t pwm = g_motor_ctrl[id].pwm;
	if (pwm > 0) {
		g_motor_ctrl[id].dir = TB6612_FORWARD;
	} else if (pwm < 0) {
		g_motor_ctrl[id].dir = TB6612_BACK;
	} else {
		g_motor_ctrl[id].dir = TB6612_STOP;
	}
}

int16_t motor_get_pwm_val(uint8_t id)
{
	return g_motor_ctrl[id].pwm;
}

void motor_set_pwm(uint8_t id, int16_t pwm)
{
	if (g_motor_ctrl[id].is_stop) {
		return;
	}
	// 限幅
	if (pwm > MAX_PWM) {	
		pwm = MAX_PWM;
	} else if(pwm < -MAX_PWM) {
		pwm = -MAX_PWM;
	}
	tb6612_set_pwm(g_motor_tb6612_map[id], abs(pwm));
}

void motor_set_pwm_val(uint8_t id, int16_t pwm)
{
	if (g_motor_ctrl[id].is_stop) {
		return;
	}
	g_motor_ctrl[id].pwm = pwm;
}

void motor_inc_pwm_val(uint8_t id, int16_t pwm)
{
	if (g_motor_ctrl[id].is_stop) {
		return;
	}
	g_motor_ctrl[id].pwm += pwm;
}

void motor_update_encoder(void)
{
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		g_motor_ctrl[id].encoder = tb6612_encoder_read_and_reset(id) - ENCODER_TIM_INIT_VAL;
	}
}

int16_t motor_get_encoder_val(uint8_t id)
{
	return g_motor_ctrl[id].encoder;
}

//计算电机带动轮子的路程
float motor_get_distance(uint8_t id)
{
	g_motor_ctrl[id].distance = g_motor_ctrl[id].encCnter / g_motor_ctrl[id].dist_1mm;
	return g_motor_ctrl[id].distance;
}

void motor_set_stop(uint8_t id)
{
	motor_set_dir(id, TB6612_STOP);
	motor_set_pwm(id, 0);
	g_motor_ctrl[id].is_stop = true;
}

void motor_cancel_stop(uint8_t id)
{
	g_motor_ctrl[id].is_stop = false;
}

static void motor_ctrl_init_one(uint8_t id)
{
	sMotor *motor = &g_motor_ctrl[id];
	motor->id = id;
	motor->dir = TB6612_STOP;
	motor->pwm = 0;
	motor->encoder = 0;
	motor->encCnter = 0;
	motor->is_stop = false;
	motor->distance = 0;
	motor->dist_1mm = g_motor_dist_1mm[id];
}

static void motor_ctrl_init(void)
{
	sMotor *motor = NULL;
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		motor_ctrl_init_one(id);
	}
}

static void motor_encoder_init(void)
{
	uint8_t id;
	for (id = 0; id < TB6612_ID_MAX; id++) {
		tb6612_encoder_read_and_reset(id);
	}
}

void motor_init(void)
{
	tb6612_init();
	motor_encoder_init();
	motor_ctrl_init();
}

void motor_start(void)
{
	uint8_t id;

	tb6612_start();
	for (id = 0; id < MOTOR_NUM; id++) {
		motor_set_dir(id, TB6612_STOP);
		motor_set_pwm(id, 0);
	}
}

//打印电机参数
void motor_print(uint8_t id)
{
	printf("\r\n");
	switch(id){
		case FL_MOTOR: printf("FLMotor - "); break;
		case FR_MOTOR: printf("FRMotor - "); break;
		case BL_MOTOR: printf("BLMotor - "); break;
		case BR_MOTOR: printf("BRMotor - "); break;
	}
	printf("pwm:%d - ", g_motor_ctrl[id].pwm);
	if(id == FL_MOTOR || id == BL_MOTOR) {
		printf("encoder:%d - ", g_motor_ctrl[id].encoder);
	} else if(id == FR_MOTOR || id == BR_MOTOR) {
		printf("encoder:%d - ", -g_motor_ctrl[id].encoder);
	}
	printf("encCnter:%d - ", g_motor_ctrl[id].encCnter);
	printf("distance:%.2f - ", motor_get_distance(id));
	switch(g_motor_ctrl[id].dir){
		case TB6612_FORWARD:	printf("Forward"); break;
        case TB6612_BACK:      printf("Back");    break;
        case TB6612_STOP:      printf("Stop");    break;
	}
	printf("\r\n---------------------------------------------------------");
}