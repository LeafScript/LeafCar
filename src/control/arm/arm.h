#ifndef _ARM_H_
#define _ARM_H_

#include "base_type.h"

typedef enum {
    ARM_STATUS_FOLD,
    ARM_STATUS_MIDDLE,
    ARM_STATUS_LEFT,
    ARM_STATUS_MAX
} arm_status_e;

void Arm_Init(void);
void Arm_SetStatus(arm_status_e status);
	
#endif