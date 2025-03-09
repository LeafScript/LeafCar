#include "sys.h"
#include "taskconf.h"
#include "carmode.h"

extern sCar MyCar;
static uint8_t action = 1;
static uint8_t lock = 0;

//任务一扫描
void TestTask_Scan(void)
{
	if(MyCar.mode == STOP_MOVE){
		lock = 0;
	}
	if(lock){
		return;
	}
	
	switch(action){
		case 1: Car_Track(2000,400,1); action++; lock = 1; break;
		default: break;
	}
}