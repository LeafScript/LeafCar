#ifndef _CAR_CTRL_H_
#define _CAR_CTRL_H_

#include "base_type.h"

enum car_mode_e {
	STOP_MOVE,		// 停止模式     - 小车停止
	FREE_MOVE,		// 自由移动模式 - 前后以一定速度循环移动
	TARGET_MOVE,	// 移动目标距离 - 前后移动，可以控制移动距离和速度
	TRACK_MOVE,		// 循迹模式     - 循迹，可以控制循迹移动距离和速度
	TURN_MOVE,		// 转动目标距离 - 转动，可以控制转动距离和速度
	CAR_MODE_MAX
};

enum car_dir_e {
	CAR_STOP,
	CAR_FORWARD,
	CAR_BACK,
	CAR_LEFT_TURN,
	CAR_RIGHT_TURN,
	CAR_DIR_MAX
};

//小车扫描周期-ms，计算速度用
#define CAR_SCAN_PERIOD     10
//100ms/小车扫描周期-ms
#define CAR_PERIOD_TIMES    (100.0/CAR_SCAN_PERIOD)
//#define CAR_PERIOD_TIMES    10

//小车模式记录结构体
typedef struct{
	uint8_t mode;		//小车模式
	uint8_t dir;        //小车运动方向
	float speed;        //小车速度，100ms内编码值
	float distance;		//小车运动距离
	float targetDist;	//目标位移
	int16_t encVal;		//设定速度对应编码值
} car_ctrl_s;

void car_ctrl_init(void);
void car_ctrl_update_pwm(void);
uint8_t car_ctrl_get_mode(void);
void car_ctrl_set_mode(uint8_t mode);
void car_ctrl_set_dir(uint8_t dir);
void car_ctrl_set_speed(float speed);
float car_ctrl_get_dist(void);
void car_ctrl_set_target_dist(float dist);

void car_stop(void);
void car_start(void);

#endif