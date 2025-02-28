#include "task2.h"
#include "taskconf.h"
#include "redwire.h"
#include "carmode.h"
#include "k210.h"
#include "led.h"
#include "carled.h"
#include "delay.h"		//在不稳定的地方延时一下
#include <stdio.h>

extern sCar MyCar;
static uint8_t action = 1;
static uint8_t lock = 0;

//任务一扫描
void Task2_Scan()
{
	if(MyCar.mode == STOP_MOVE){
		lock = 0;
	}
	if(lock){
		return;
	}
	
	switch(action){
		case 1: if(Redwire_isFull()){action++;} break;
		case 2: Car_Track(200000,400,1); action++; lock = 1; break;
//		case 2: Car_Forward(1000,400); action++; lock = 1; break;
//		case 2: Car_Forward_Muti(1000,400,MUTI_LENGHTH_TEST); action++; lock = 1; break;
//		case 2: Car_Back(1000,400); action++; lock = 1; break;
//		case 2: Car_Back_Muti(1000,400,MUTI_LENGHTH_TEST); action++; lock = 1; break;
//		case 2: Car_Track(1000,400,1); action++; lock = 1; break;
//		case 2: Car_Track_Muti(1000,400,1,MUTI_LENGHTH_TEST); action++; lock = 1; break;
//		case 2: Car_Round(1000,400,1); action++; lock = 1; break;
//		case 2: Car_Round_Muti(1000,400,1,MUTI_LENGHTH_TEST); action++; lock = 1; break;
		case 3: if(Redwire_isFull()){} break;
		default: break;
	}
}