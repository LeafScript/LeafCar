#ifndef _UPDATE_FW_TASK_H_
#define _UPDATE_FW_TASK_H_

#include "base_type.h"

int update_fw_task_init(uint8_t task_id);
void update_fw_task_start(void);
void update_fw_task_scan(void);

#endif