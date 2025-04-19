#ifndef _DEBUG_TASK_H_
#define _DEBUG_TASK_H_

#include "base_type.h"

int cli_task_init(uint8_t task_id);
void cli_recv_data(uint8_t ch);
void cli_task_start(void);
void cli_task_scan(void);

#endif