#ifndef _CAR_TASK_OP_H_
#define _CAR_TASK_OP_H_

#include "base_type.h"

enum car_op_obj_e {
	CAR_OP_OBJ_CAR,
	CAR_OP_OBJ_ARM,
	CAR_OP_OBJ_REDWIRE,
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

typedef struct {
	uint16_t dist;
	uint16_t speed;
	bool is_forward;
	bool is_right;
} car_op_param_s;

typedef struct {
    void (*cb)(void);
} redwire_op_param_s;

typedef struct {
	uint8_t op_obj;
	uint8_t op;
	union {
		car_op_param_s car_param;
        redwire_op_param_s redwire_param;
	};
} car_op_s;

int car_op_handle(car_op_s *op);

#endif