#ifndef _CAR_TASK_SCHEDULE_H_
#define _CAR_TASK_SCHEDULE_H_

#include "base_type.h"
#include "car_task_op.h"

int car_task_init(uint8_t task_id);
int car_task_info_init(car_op_s *op_list, uint16_t op_list_size);
void car_task_start(void);
void car_task_scan(void);

#endif