#ifndef _CAR_TASK_OP_H_
#define _CAR_TASK_OP_H_

#include "base_type.h"

enum car_op_obj_e {
	CAR_OP_OBJ_CAR,
	CAR_OP_OBJ_ARM,
	CAR_OP_OBJ_REDWIRE,
	CAR_OP_OBJ_LED,
	CAR_OP_OBJ_TIMER,
	CAR_OP_OBJ_OP,
	CAR_OP_OBJ_MAX
};

enum car_op_e {
	CAR_OP_FORWARD,
	CAR_OP_BACK,
	CAR_OP_TRACK,
	CAR_OP_TURN,
	CAR_OP_MAX
};

enum arm_op_e {
	ARM_OP_FOLD,
	ARM_OP_MIDDLE,
	ARM_OP_LEFT,
	ARM_OP_MAX
};

enum redwire_op_e {
	REDWIRE_OP_IS_FULL,
	REDWIRE_OP_IS_NOT_FULL,
	REDWIRE_OP_MAX
};

enum led_op_e {
	LED_OP_ON,
	LED_OP_OFF,
	LED_OP_MAX
};

enum timer_op_e {
	TIMER_OP_TRIGGER_ONCE,
	TIMER_OP_TRIGGER_MUTI,
	TIMER_OP_MAX
};

enum op_op_e {
	OP_OP_JUMP_TO,
	OP_OP_JUMP_PREV,
	OP_OP_JUMP_NEXT,
	OP_OP_FUNC_JUMP_TO,
	OP_OP_FUNC_JUMP_PREV,
	OP_OP_FUNC_JUMP_NEXT,
	OP_OP_END,
	OP_OP_MAX
};

typedef struct {
	uint16_t dist;
	uint16_t speed;
	bool is_forward;
	bool is_right;
} car_op_param_s;

typedef struct {
    void (*cb)(void *priv);
	void *priv;
} redwire_op_param_s;

typedef struct {
	uint8_t color;
} led_op_param_s;

typedef struct {
	uint32_t trigger_ms;
	void (*cb)(void *priv);
	void *priv;
	uint32_t trigger_times;
} timer_op_param_s;

typedef struct {
	uint16_t step;
	uint16_t (*get_step)(void);
} op_op_param_s;

typedef struct {
	uint8_t op_obj;
	uint8_t op;
	union {
		car_op_param_s car_param;
        redwire_op_param_s redwire_param;
		led_op_param_s led_param;
		timer_op_param_s timer_param;
		op_op_param_s op_param;
	};
} car_op_s;

int car_op_handle(car_op_s *op);

#endif