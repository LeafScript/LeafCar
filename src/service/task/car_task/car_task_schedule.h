#ifndef _CAR_TASK_SCHEDULE_H_
#define _CAR_TASK_SCHEDULE_H_

#include "base_type.h"
#include "car_task_op.h"

enum car_task_id_e {
    CAR_TASK_TEST,
    CAR_TASK_F,
    CAR_TASK_ID_MAX
};

int car_task_init(uint8_t task_id);
int car_task_info_init(car_op_s *op_list, uint16_t op_list_size);
int car_task_register(uint8_t car_task_id);
void car_task_start(void);
void car_task_scan(void);
void car_task_async_step_finish(void);
int car_task_set_op_next(uint16_t op_index);
uint16_t car_task_get_op_cur(void);

#endif