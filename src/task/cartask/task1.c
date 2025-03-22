#include "task1.h"
#include "taskconf.h"
#include "redwire.h"
#include "car_ctrl.h"
#include "car_op.h"
#include "carled.h"
#include "log.h"
#include "car_task_op.h"
#include "car_task_schedule.h"

extern uint8_t recNum1;
extern uint8_t recNum2;

static uint8_t room_num = 2;	//识别到的房间号数字
static uint8_t nowNum1 = 0;		//识别到的当前数字1
static uint8_t nowNum2 = 0;		//识别到的当前数字2
static uint16_t jmp_next = 0;
static uint16_t car_stop_jump_next(void)
{
	return (car_ctrl_get_mode() == STOP_MOVE) ? 1 : 0;
}

static uint16_t room_1_2_judge(void)
{
	if (recNum1 == 0 && recNum2 == 0){		//未识别到数字
		return 0;
	}
	room_num = (recNum1 != 0) ? recNum1 : recNum2;
	// 若为1,2号病房，进入近端病房; 否则运动到中部病房识别
	jmp_next = (room_num == 1 || room_num == 2) ? 1 : 21;
	recNum1 = 0;
	recNum2 = 0;
	return 1;
}

static uint16_t room_1_2_jump(void)
{
	if (!Redwire_isFull()) {
		return 0;
	}
	return jmp_next;
}

static uint16_t room_1_2_car_turn(void)
{
	car_turn(VERTICAL_L, TURN_SPEED, (room_num == 1) ? 0 : 1);
	return 1;
}

static uint16_t room_3_4_judge(void)
{
	if (recNum1 == 0 || recNum2 == 0){		// 未同时识别到2个数字
		return 0;
	}
	// 若为3,4号病房则进入中部病房; 否则运动到远端交叉路口识别
	jmp_next = (recNum1 == room_num || recNum2 == room_num) ? 1 : 21;
	nowNum1 = recNum1;
	nowNum2 = recNum2;
	recNum1 = 0;
	recNum2 = 0;
	return 1;
}

static uint16_t room_3_4_car_turn(void)
{
	car_turn(VERTICAL_L, TURN_SPEED, (nowNum1 == room_num) ? 0 : 1);
	return 1;
}

static uint16_t room_remote_1_judge(void)
{
	if (recNum1 <= 2 && recNum2 <= 2){		//未识别到数字
		return 0;
	}
	nowNum1 = recNum1;
	nowNum2 = recNum2;
	recNum1 = 0;
	recNum2 = 0;
	return 1;
}

static uint16_t room_remote_1_car_turn(void)
{
	car_turn(VERTICAL_L, TURN_SPEED, (nowNum1 == room_num || nowNum2 == room_num) ? 0 : 1);
	return 1;
}

static uint16_t room_remote_2_judge(void)
{
	if (recNum1 <= 2 && recNum2 <= 2){		//未识别到数字
		return 0;
	}
	nowNum1 = recNum1;
	nowNum2 = recNum2;
	recNum1 = 0;
	recNum2 = 0;
	return 1;
}

static uint16_t room_remote_2_car_turn(void)
{
	car_turn(VERTICAL_L, TURN_SPEED, (nowNum1 == room_num || nowNum2 == room_num) ? 0 : 1);
	return 1;
}

static car_op_s g_car_task1_op_list[] = {
	{ CAR_OP_OBJ_ARM, ARM_OP_FOLD },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = car_task_async_cb } },
	{ CAR_OP_OBJ_ARM, ARM_OP_MIDDLE },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = car_task_async_cb } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = room_1_2_judge } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = room_1_2_jump } },
	// 1, 2号病房
	{ CAR_OP_OBJ_ARM, ARM_OP_FOLD },
	{ CAR_OP_OBJ_CAR, CAR_OP_TRACK, .car_param = { .dist = 570, .speed = SPEED, .is_forward = 1 } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_CAR, CAR_OP_FORWARD, .car_param = { .dist = 180, .speed = SPEED} },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = room_1_2_car_turn } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_CAR, CAR_OP_TRACK, .car_param = { .dist = 300, .speed = SPEED, .is_forward = 1 } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_CAR, CAR_OP_FORWARD, .car_param = { .dist = 100, .speed = SPEED } },
	{ CAR_OP_OBJ_LED, LED_OP_ON, .led_param = { .color = LED_RED } },
	{ CAR_OP_OBJ_REDWIRE, REDWIRE_OP_IS_NOT_FULL, .redwire_param = { .cb = car_task_async_cb } },
	{ CAR_OP_OBJ_LED, LED_OP_OFF, .led_param = { .color = LED_RED } },
	{ CAR_OP_OBJ_CAR, CAR_OP_BACK, .car_param = { .dist = 400, .speed = SPEED } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = room_1_2_car_turn } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_CAR, CAR_OP_TRACK, .car_param = { .dist = 550, .speed = SPEED, .is_forward = 1 } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_CAR, CAR_OP_FORWARD, .car_param = { .dist = 100, .speed = SPEED } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_LED, LED_OP_ON, .led_param = { .color = LED_GREEN } },
	{ CAR_OP_OBJ_OP, OP_OP_END },
	// K210识别判断病房在中部&远端（左右都要识别）
	{ CAR_OP_OBJ_CAR, CAR_OP_TRACK, .car_param = { .dist = 1350, .speed = SPEED, .is_forward = 1 } },
	{ CAR_OP_OBJ_ARM, ARM_OP_FOLD },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_ARM, ARM_OP_MIDDLE },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 1500, .cb = car_task_async_cb } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = room_3_4_judge } },
	// 中部病房
	{ CAR_OP_OBJ_ARM, ARM_OP_FOLD },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 1000, .cb = car_task_async_cb } },
	{ CAR_OP_OBJ_CAR, CAR_OP_FORWARD, .car_param = { .dist = 300, .speed = SPEED} },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = room_3_4_car_turn } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_CAR, CAR_OP_TRACK, .car_param = { .dist = 400, .speed = SPEED, .is_forward = 1 } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_CAR, CAR_OP_FORWARD, .car_param = { .dist = 50, .speed = SPEED} },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_LED, LED_OP_ON, .led_param = { .color = LED_RED } },
	{ CAR_OP_OBJ_REDWIRE, REDWIRE_OP_IS_NOT_FULL, .redwire_param = { .cb = car_task_async_cb } },
	{ CAR_OP_OBJ_LED, LED_OP_OFF, .led_param = { .color = LED_RED } },
	{ CAR_OP_OBJ_CAR, CAR_OP_BACK, .car_param = { .dist = 450, .speed = SPEED } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = room_3_4_car_turn } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_CAR, CAR_OP_TRACK, .car_param = { .dist = 1500, .speed = SPEED, .is_forward = 1 } },
	{ CAR_OP_OBJ_CAR, CAR_OP_FORWARD, .car_param = { .dist = 50, .speed = SPEED} },
	{ CAR_OP_OBJ_LED, LED_OP_ON, .led_param = { .color = LED_GREEN } },
	{ CAR_OP_OBJ_OP, OP_OP_END },
	// K210识别判断病房在 远端第一交叉路口 左右（左，右边通过左边推断出）
	{ CAR_OP_OBJ_ARM, ARM_OP_FOLD },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 1000, .cb = car_task_async_cb } },
	{ CAR_OP_OBJ_CAR, CAR_OP_FORWARD, .car_param = { .dist = 150, .speed = SPEED} },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_CAR, CAR_OP_TRACK, .car_param = { .dist = 830, .speed = SPEED, .is_forward = 1 } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_ARM, ARM_OP_LEFT },
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 1500, .cb = car_task_async_cb } },
	// 远端病房第一交叉路口-->远端病房第二交叉路口
	{ CAR_OP_OBJ_CAR, CAR_OP_FORWARD, .car_param = { .dist = 150, .speed = SPEED} },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = room_remote_1_car_turn } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_CAR, CAR_OP_TRACK, .car_param = { .dist = 590, .speed = SPEED, .is_forward = 1 } },
	{ CAR_OP_OBJ_CAR, CAR_OP_FORWARD, .car_param = { .dist = 50, .speed = SPEED} },
	// K210识别判断病房在远端第二交叉路口左右
	{ CAR_OP_OBJ_TIMER, TIMER_OP_TRIGGER_ONCE, .timer_param = { .trigger_ms = 500, .cb = car_task_async_cb } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = room_remote_2_judge } },
	// 远端病房&退回
	{ CAR_OP_OBJ_CAR, CAR_OP_FORWARD, .car_param = { .dist = 200, .speed = SPEED} },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = room_remote_2_car_turn } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_CAR, CAR_OP_TRACK, .car_param = { .dist = 400, .speed = SPEED, .is_forward = 1 } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_CAR, CAR_OP_FORWARD, .car_param = { .dist = 50, .speed = SPEED} },
	{ CAR_OP_OBJ_LED, LED_OP_ON, .led_param = { .color = LED_RED } },
	{ CAR_OP_OBJ_REDWIRE, REDWIRE_OP_IS_NOT_FULL, .redwire_param = { .cb = car_task_async_cb } },
	{ CAR_OP_OBJ_LED, LED_OP_OFF, .led_param = { .color = LED_RED } },
	{ CAR_OP_OBJ_CAR, CAR_OP_BACK, .car_param = { .dist = 450, .speed = SPEED } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = car_stop_jump_next } },
	{ CAR_OP_OBJ_OP, OP_OP_FUNC_JUMP_NEXT, .op_param = { .get_step = room_remote_2_car_turn } },
	{ CAR_OP_OBJ_CAR, CAR_OP_TRACK, .car_param = { .dist = 2450, .speed = SPEED, .is_forward = 1 } },
	{ CAR_OP_OBJ_CAR, CAR_OP_FORWARD, .car_param = { .dist = 50, .speed = SPEED} },
	{ CAR_OP_OBJ_LED, LED_OP_ON, .led_param = { .color = LED_GREEN } },
	{ CAR_OP_OBJ_OP, OP_OP_END },
};

int car_task1_info_init(void)
{
	return car_task_info_init(g_car_task1_op_list, ARRAY_SIZE(g_car_task1_op_list));
}