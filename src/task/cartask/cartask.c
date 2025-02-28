#include "cartask.h"
#include "taskconf.h"
#include "redwire.h"
#include "led.h"
#include "task1.h"
#include "task2.h"
#include "work1.h"

//小车执行任务
uint8_t carTask = 1;

void Car_SetTask()
{
//	carTask = Button_GetTask();
}

void CarTask_Scan()
{
	switch(carTask){
		case 1: Task1_Scan(); break;
		case 2: Redwire_isFull(); break;
		case 3: Task2_Scan(); break;
		case 4: Work1_Scan(); break;
		case 5:  break;
		case 6:  break;
		case 7:  break;
		default: break;
	}
}
