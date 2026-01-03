#ifndef _ACTIVE_FW_TASK_H_
#define _ACTIVE_FW_TASK_H_

#include "base_type.h"

int active_fw_task_init(uint8_t task_id);
void active_fw_task_start(void);
void active_fw_task_scan(void);

#endif