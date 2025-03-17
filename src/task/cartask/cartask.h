#ifndef __CARTASK_H
#define __CARTASK_H

#include "base_type.h"

enum car_task_id_e {
	CAR_TASK_0,
	CAR_TASK_1,
	CAR_TASK_2,
	CAR_TASK_MAX
};

void car_task_init(uint8_t task_id);
void car_task_start(uint8_t task_id);
void car_task_scan(void);

#endif