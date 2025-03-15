#include "car_ctrl.h"
#include "motor.h"
#include "pid.h"
#include "usart.h"

static car_ctrl_s g_car_ctrl;

void car_stop(void)
{
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		motor_set_stop(id);
	}
	vpid_ctrl_deinit();
}

void car_start(void)
{
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		motor_cancel_stop(id);
	}
}

static float car_get_distance(bool is_turn)
{
	if (is_turn) {
		//小车路程 = (左前轮-右前轮)/2
		return (motor_get_distance(FL_MOTOR) - motor_get_distance(FR_MOTOR)) / 2.0;
	} else {
		//小车路程 = (左前轮+右前轮)/2
		return (motor_get_distance(FL_MOTOR) + motor_get_distance(FR_MOTOR)) / 2.0;
	}
}

static float car_get_speed(void)
{
	int16_t fl_enc_val, fr_enc_val;
	//小车速度 = PERIOD_TIMES * (左前轮+右前轮)/2
	fl_enc_val = vpid_get_motor_actual_speed(FL_MOTOR);
	fr_enc_val = vpid_get_motor_actual_speed(FR_MOTOR);
	return CAR_PERIOD_TIMES * (fl_enc_val + fr_enc_val) / 2.0;
}

static void car_update_pwm_val(void)
{
	motor_inc_pwm_val(FL_MOTOR, vpid_get_inc_pwm(FL_MOTOR, motor_get_encoder_val(FL_MOTOR)));
	motor_inc_pwm_val(FR_MOTOR, vpid_get_inc_pwm(FR_MOTOR, -motor_get_encoder_val(FR_MOTOR)));
	motor_inc_pwm_val(BL_MOTOR, vpid_get_inc_pwm(BL_MOTOR, motor_get_encoder_val(BL_MOTOR)));
	motor_inc_pwm_val(BR_MOTOR, vpid_get_inc_pwm(BR_MOTOR, -motor_get_encoder_val(BR_MOTOR)));
}

static void car_set_stop_mode(void)
{
	g_car_ctrl.mode = STOP_MOVE;
	car_stop();
}

static void car_update_pwm_stop_mode(void)
{
	return;
}

static void car_set_free_mode(void)
{
	g_car_ctrl.mode = FREE_MOVE;
	g_car_ctrl.dir = CAR_STOP;
	g_car_ctrl.speed = 0;
	g_car_ctrl.distance = 0;
	car_start();
}

static void car_update_pwm_free_mode(void)
{
	static uint8_t dir_sw_map[] = { CAR_FORWARD, CAR_BACK, CAR_STOP };
	static int16_t target_enc_val[] = { 100.0 / CAR_PERIOD_TIMES, -100.0 / CAR_PERIOD_TIMES, 0 };
	static uint8_t step = 0;
	static uint16_t timer = 0;

	// 3s改变一次方向
	if (timer < (uint16_t)CAR_PERIOD_TIMES * 30) {
		timer++;
	} else {
		timer = 0;
		if (step == ARRAY_SIZE(dir_sw_map)) {
			step = 0;
		}
		g_car_ctrl.dir = dir_sw_map[step];
		vpid_set_car_speed(target_enc_val[step]);
		step++;
	}
	g_car_ctrl.speed = car_get_speed();
	g_car_ctrl.distance = car_get_distance(false);
	car_update_pwm_val();
}

static void car_set_target_mode(void)
{
	g_car_ctrl.mode = TARGET_MOVE;
	g_car_ctrl.dir = CAR_STOP;
	g_car_ctrl.speed = 0;
	g_car_ctrl.distance = 0;
	g_car_ctrl.target_dist = 0;
}

static void car_update_pwm_target_mode(void)
{
	if((g_car_ctrl.dir == CAR_FORWARD && g_car_ctrl.distance >= g_car_ctrl.target_dist) ||
	   (g_car_ctrl.dir == CAR_BACK && g_car_ctrl.distance <= g_car_ctrl.target_dist)){
		car_set_stop_mode();
	}
	g_car_ctrl.speed = car_get_speed();
	g_car_ctrl.distance = car_get_distance(false);
	car_update_pwm_val();
}

static void car_set_track_mode(void)
{
	g_car_ctrl.mode = TRACK_MOVE;
	g_car_ctrl.dir = CAR_STOP;
	g_car_ctrl.speed = 0;
	g_car_ctrl.distance = 0;
	g_car_ctrl.target_dist = 0;
	g_car_ctrl.enc_val = 0;
}

static void car_update_pwm_track_mode(void)
{
	if ((g_car_ctrl.dir == CAR_FORWARD && g_car_ctrl.distance >= g_car_ctrl.target_dist) ||
	    (g_car_ctrl.dir == CAR_BACK && g_car_ctrl.distance <= g_car_ctrl.target_dist)){
		car_set_stop_mode();
	}
	g_car_ctrl.speed = car_get_speed();
	g_car_ctrl.distance = car_get_distance(false);
	// 寻迹
	vpid_set_car_speed_by_offset(g_car_ctrl.enc_val);
	car_update_pwm_val();
}

static void car_set_turn_mode(void)
{
	g_car_ctrl.mode = TURN_MOVE;
	g_car_ctrl.dir = MOTOR_STOP;
	g_car_ctrl.speed = 0;
	g_car_ctrl.distance = 0;
	g_car_ctrl.target_dist = 0;
}

static void car_update_pwm_turn_mode(void)
{
	g_car_ctrl.speed = car_get_speed();
	g_car_ctrl.distance = car_get_distance(true);
	if ((g_car_ctrl.dir == CAR_RIGHT_TURN && g_car_ctrl.distance >= g_car_ctrl.target_dist) ||
	    (g_car_ctrl.dir == CAR_LEFT_TURN && g_car_ctrl.distance <= g_car_ctrl.target_dist)){
		car_set_stop_mode();
	}
	car_update_pwm_val();
}

struct {
	uint8_t mode;
	void (*set_mode)(void);
	void (*update_pwm)(void);
} g_car_mode_list[] = {
	{ STOP_MOVE,   car_set_stop_mode, car_update_pwm_stop_mode },
	{ FREE_MOVE,   car_set_free_mode, car_update_pwm_free_mode },
	{ TARGET_MOVE, car_set_target_mode, car_update_pwm_target_mode },
	{ TRACK_MOVE,  car_set_track_mode, car_update_pwm_track_mode },
	{ TURN_MOVE,   car_set_turn_mode, car_update_pwm_turn_mode },
};

void car_ctrl_set_mode(uint8_t mode)
{
	if (mode >= ARRAY_SIZE(g_car_mode_list)) {
		return;
	}
	g_car_mode_list[mode].set_mode();
}

void car_ctrl_update_pwm(void)
{
	uint8_t mode;
	for (mode = 0; mode < ARRAY_SIZE(g_car_mode_list); mode++) {
		if (mode == g_car_ctrl.mode) {
			g_car_mode_list[mode].update_pwm();
			return;
		}
	}
}

void car_ctrl_init(void)
{
	vpid_ctrl_init();
	car_ctrl_set_mode(STOP_MOVE);
}

uint8_t car_ctrl_get_mode(void)
{
	return g_car_ctrl.mode;
}

void car_ctrl_set_dir(uint8_t dir)
{
	if (dir >= CAR_DIR_MAX) {
		return;
	}
	g_car_ctrl.dir = dir;
}

void car_ctrl_set_speed(float speed)
{
	g_car_ctrl.speed = speed;
}

float car_ctrl_get_dist(void)
{
	return g_car_ctrl.distance;
}

void car_ctrl_set_target_dist(float dist)
{
	g_car_ctrl.target_dist = dist;
}