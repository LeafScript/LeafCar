#include "car_op.h"
#include "car_ctrl.h"
#include "pid.h"

//小车向前
//dist - 距离 mm
//speed - 100ms编码器计数
void car_forward(uint16_t dist, uint16_t speed)
{
	int16_t enc_val = (float)speed / CAR_PERIOD_TIMES;

	car_ctrl_set_mode(TARGET_MOVE);
	car_ctrl_set_dir(CAR_FORWARD);
	car_ctrl_set_target_dist(car_ctrl_get_dist() + dist);
	vpid_set_car_speed(enc_val);
	car_start();
}

//小车向后
//dist - 距离 mm
//speed - 100ms编码器计数
void car_back(uint16_t dist, uint16_t speed)
{
	int16_t enc_val = (float)-speed / CAR_PERIOD_TIMES;

	car_ctrl_set_mode(TARGET_MOVE);
	car_ctrl_set_dir(CAR_BACK);
	car_ctrl_set_target_dist(car_ctrl_get_dist() - dist);
	vpid_set_car_speed(enc_val);
	car_start();
}

//小车循迹
//dist - 距离 mm
//speed - 100ms编码器计数
//dir - 1:前  0:后
void car_track(uint32_t dist, uint16_t speed, bool is_forward)
{
	int16_t enc_val;

	car_ctrl_set_mode(TRACK_MOVE);
	if (is_forward) {
		car_ctrl_set_dir(CAR_FORWARD);
		car_ctrl_set_target_dist(car_ctrl_get_dist() + dist);
		enc_val = (float)speed / CAR_PERIOD_TIMES;
	} else {
		car_ctrl_set_dir(CAR_BACK);
		car_ctrl_set_target_dist(car_ctrl_get_dist() - dist);
		enc_val = (float)-speed / CAR_PERIOD_TIMES;
	}
	vpid_set_car_speed(enc_val);
	car_start();
}

//小车旋转
//dist - 距离 mm
//speed - 100ms编码器计数
//dir - 1:顺  0:逆
void car_turn(uint16_t dist, uint16_t speed, bool is_right)
{
	int16_t enc_val;

	car_ctrl_set_mode(TURN_MOVE);
	if (is_right) {
		car_ctrl_set_dir(CAR_RIGHT_TURN);
		car_ctrl_set_target_dist(car_ctrl_get_dist() + dist);
		enc_val = (float)speed / CAR_PERIOD_TIMES;
	} else {
		car_ctrl_set_dir(CAR_LEFT_TURN);
		car_ctrl_set_target_dist(car_ctrl_get_dist() - dist);
		enc_val = (float)-speed / CAR_PERIOD_TIMES;
	}
	vpid_set_round_speed(enc_val);
	car_start();
}
