#include "cartask.h"
#include "taskconf.h"
#include "task1.h"
#include "work1.h"
#include "test_task.h"

//小车执行任务
uint8_t carTask = 3;

void CarTask_Scan(void)
{
	switch(carTask){
		case 1: Task1_Scan(); break;
		case 2: Work1_Scan(); break;
		case 3: TestTask_Scan(); break;
		default: break;
	}
}
