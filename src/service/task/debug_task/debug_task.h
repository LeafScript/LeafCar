#ifndef _DEBUG_TASK_H_
#define _DEBUG_TASK_H_

#include "base_type.h"

int debug_task_init(uint8_t task_id);
void debug_task_start(void);
void debug_task_scan(void);

#endif