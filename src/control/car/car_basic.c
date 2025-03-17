#include "car_basic.h"
#include "car_ctrl.h"
#include "motor.h"
#include "usart.h"

void car_init(void)
{
	motor_init();
	car_ctrl_init();
}

//小车扫描
void car_scan(void)
{
	uint8_t id;
	motor_update_encoder();
	car_ctrl_update_pwm();
	for (id = 0; id < MOTOR_NUM; id++) {
		motor_update_dir_val(id);
		motor_set_pwm(id, motor_get_pwm_val(id));
		motor_set_dir(id, motor_get_dir_val(id));
	}
}

//小车状态打印
void car_print(void)
{
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		motor_print(id);
	}
}

//设置小车4个电机PWM以及方向
void car_set_pwm(uint8_t dir, int16_t pwm)
{
	uint8_t id;
	for (id = 0; id < MOTOR_NUM; id++) {
		motor_set_pwm_val(id, pwm);
	}
}
