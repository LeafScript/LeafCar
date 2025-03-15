#include "sys.h"
#include "taskconf.h"
#include "car_ctrl.h"
#include "car_option.h"

static uint8_t action = 1;
static uint8_t lock = 0;

//任务一扫描
void TestTask_Scan(void)
{
	if(car_ctrl_get_mode() == STOP_MOVE){
		lock = 0;
	}
	if(lock){
		return;
	}
	
	switch(action){
		case 1: car_track(2000,400,1); action++; lock = 1; break;
		default: break;
	}
}