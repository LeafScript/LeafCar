#ifndef _CAR_TASK_H_
#define _CAR_TASK_H_

#include "base_type.h"

enum car_task_id_e {
	CAR_TASK_0,
	CAR_TASK_1,
	CAR_TASK_2,
	CAR_TASK_MAX
};

enum car_op_obj_e {
	CAR_OP_OBJ_CAR,
	CAR_OP_OBJ_ARM,
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

struct car_op_obj_car_s {
	uint8_t op;
	uint16_t dist;
	uint16_t speed;
	bool is_forward;
	bool is_right;
};

void car_task_init(uint8_t task_id);
void car_task_start(uint8_t task_id);
void car_task_scan(void);

#endif