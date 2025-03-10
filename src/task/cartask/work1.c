#include "work1.h"
#include "taskconf.h"
#include "redwire.h"
#include "carmode.h"
#include "k210.h"
#include "led.h"
#include "carled.h"
#include "delay.h"		//在不稳定的地方延时一下
#include "usart.h"
#include "arm.h"

extern sCar MyCar;
extern uint8_t recNum1;
extern uint8_t recNum2;

static uint8_t action = 1;
static uint8_t lock = 0;
static uint8_t storeNum = 2;	//识别到的房间号数字
static uint8_t nowNum1 = 0;		//识别到的当前数字1
static uint8_t nowNum2 = 0;		//识别到的当前数字2
static uint8_t jmpAction = 0;
static uint8_t joint = 0;		//小车到达节点  1~5
static uint8_t joint2 = 0;		//小车到达节点  6，7

static uint8_t recNum2_backup;

//调试模式打印小车步骤
void Work1_PrintAction()
{
	printf("action=%d\r\n",action);
}

//任务一扫描
void Work1_Scan()
{
	if(MyCar.mode == STOP_MOVE){
		lock = 0;
	}
	if(lock){
		return;
	}
	
	if(action>=6 && recNum1<=2){
		recNum1 = 0;
	}
	if(action>=6 && recNum2<=2){
		recNum2 = 0;
	}
	
	switch(action){
			/*&& 起始状态 &&*/
		case 1: Arm_SetStatus(1); delay_ms(500); action++; break;
			/*########### K210识别得到病房号 ##########*/
		case 2: Arm_SetStatus(2); action++; break;
		case 3: delay_ms(500); action++; break;		//延时等待识别稳定
		case 4:
			#if (TASK_TEST==0)
			if(recNum1==0 && recNum2==0){		//未识别到数字
				return;
			}else{
				if(recNum1!=0){ storeNum =recNum1; }
				else		  { storeNum =recNum2; }
				recNum1=0; recNum2=0; action++;
				if(storeNum==1 || storeNum==2){		// 1,2号病房，不需要K210识别
					jmpAction = 6; joint = 1;
				}else{								// 运动到中部病房识别
					jmpAction = 20; joint = 2;
				}
			}
			#else
			action++;
			if(storeNum==1 || storeNum==2){		// 1,2号病房，不需要K210识别
				jmpAction = 6; joint = 1;
			}else{								// 运动到中部病房识别
				jmpAction = 20; joint = 2;
			}
			#endif
		break;
		case 5: if(Redwire_isFull()){action = jmpAction;} break;
		/*--------------------------1,2号病房------------------------------*/
		case 6: Arm_SetStatus(1); Car_Track(570,SPEED,1); action++; lock = 1; break;
		case 7: Car_Forward(180,SPEED); action++; lock = 1; break;
		case 8:
			if(storeNum == 1) { Car_Round(VERTICAL_L,TURN_SPEED,0); }		//左转
			else              { Car_Round(VERTICAL_R,TURN_SPEED,1); }		//右转
			action++; lock = 1; 
		break;
		case 9: Car_Forward(100,SPEED); action++; lock = 1; break;
		case 10: Car_Track(300,SPEED,1); action++; lock = 1; break;
		case 11: RedLed = 1; if(!Redwire_isFull()){action++;} break;
		case 12: RedLed = 0; Car_Back(400,SPEED); action++; lock = 1; break;	//倒车
		case 13:
			if(storeNum == 1) { Car_Round(VERTICAL_L,TURN_SPEED,0); }		//左转
			else              { Car_Round(VERTICAL_R,TURN_SPEED,1); }		//右转
			action++; lock = 1;
		break;
		case 14: Car_Forward(100,SPEED); action++; lock = 1; break;
		case 15: Car_Track(550,SPEED,1); action++; lock = 1; break;
		case 16: GreLed = 1; break;		/*@@@@@@@@@@@@ 结束 @@@@@@@@@@@@@*/
		
			/*########### K210识别判断病房在中部&远端（左右都要识别） ##########*/
		case 20: Car_Track(1350,SPEED,1); Arm_SetStatus(1); action++; lock = 1; break;	//机械臂收回
		case 21: Arm_SetStatus(2); action++; break;	//机械臂中间伸出
		case 22: delay_ms(1000); delay_ms(500); action++; break;		//延时等待识别稳定
		case 23:
			#if (TASK_TEST==0)
			if(recNum1==0 || recNum2==0){		//未同时识别到2个数字
//			if(!(recNum1 || recNum2)){				//识别到其中1个数字
				return;
			}else{
				nowNum1 = recNum1; recNum1 = 0;
				nowNum2 = recNum2; recNum2 = 0;
				action++;
				if(nowNum1==storeNum || nowNum2==storeNum){		// 中部病房
					jmpAction = 30; joint = 2;
				}else{								// 运动到远端交叉路口识别
					jmpAction = 50; joint = 3;
					//jmpAction = 30; joint = 2;
				}
			}
			#else
			action++;
			if(1 || (nowNum1==storeNum && nowNum2==storeNum)){		// 中部病房
				jmpAction = 30; joint = 2; nowNum1 = storeNum;
			}else{								// 运动到远端交叉路口识别
				jmpAction = 40; joint = 3; nowNum2 = storeNum;
			}
			#endif
		break;
		case 24: action = jmpAction; break;
		/*--------------------------中部病房------------------------------*/
		case 30: Arm_SetStatus(1); delay_ms(1000); action++; break;	//收回手臂
		case 31: Car_Forward(300,SPEED); action++; lock = 1; break;	//前进一些
		case 32:
			if(nowNum1 == storeNum) { Car_Round(VERTICAL_L,TURN_SPEED,0); }		//左转
			else              		{ Car_Round(VERTICAL_R,TURN_SPEED,1); }		//右转
			action++; lock = 1;
		break;
		case 33: Car_Forward(50,SPEED); action++; lock = 1; break;	//前进一些
		case 34: Car_Track(400,SPEED,1); action++; lock = 1; break;
		case 35: RedLed = 1; if(!Redwire_isFull()){action++;} break;
		case 36: RedLed = 0; Car_Back(450,SPEED); action++; lock = 1; break;
		case 37:
			if(nowNum1 == storeNum) { Car_Round(VERTICAL_L,TURN_SPEED,0); }		//左转
			else              		{ Car_Round(VERTICAL_R,TURN_SPEED,1); }		//右转
			action++; lock = 1;
		break;
		case 38: Car_Forward(50,SPEED); action++; lock = 1; break;	//前进一些
		case 39: Car_Track(1500,SPEED,1); action++; lock = 1; break;
		case 40: GreLed = 1; break;		/*@@@@@@@@@@@@ 结束 @@@@@@@@@@@@@*/
		
			
			
			
			/*########### K210识别判断病房在 远端第一交叉路口 左右（左，右边通过左边推断出） ##########*/
		case 50: Arm_SetStatus(1); delay_ms(1000); action++; break;	//收回手臂
		case 51: Car_Forward(150,SPEED); action++; lock = 1; break;	//前进一些
		case 52: Car_Track(830,SPEED,1); action++; lock = 1; break;
		case 53: Arm_SetStatus(3); delay_ms(1000); delay_ms(500); action++; break;	//机械臂中间伸出
		case 54:
			#if (TASK_TEST==0)
			if(recNum1==storeNum || recNum2==storeNum){
				jmpAction = 60; joint = 4;		// 远端第一交叉路口左侧
				 recNum1 = 0;  recNum2 = 0;
			}else{
				jmpAction = 60; joint = 5;
				 recNum1 = 0;  recNum2 = 0;
			}
			action++;
			#else
			action++;
			if(1 || (nowNum1==storeNum && nowNum2==storeNum)){		// 远端第一交叉路口左侧
				 jmpAction = 60; joint = 4;
			}else{											// 远端第一交叉路口右侧
				 jmpAction = 60; joint = 5;
			}
			#endif
		break;
		case 55: action = jmpAction; break;
		/*--------------------------远端病房第一交叉路口-->远端病房第二交叉路口------------------------------*/
		case 60: Car_Forward(150,SPEED); action++; lock = 1; break;
		case 61:
			if(joint == 4) { Car_Round(VERTICAL_L,TURN_SPEED,0); }		//左转
			else              		{ Car_Round(VERTICAL_R,TURN_SPEED,1); }		//右转
			action++; lock = 1;
		break;
		case 62: Car_Forward(50,SPEED); action++; lock = 1; break;	//前进一些
		case 63: Car_Track(590,SPEED,1); action++; lock = 1; break;
			
			/*########### K210识别判断病房在远端第二交叉路口左右 ##########*/
		case 64: delay_ms(500); action++; break;	//延时识别
		case 65:
			#if (TASK_TEST==0)
			if(recNum1==0 && recNum2==0){		//识别到1个数字
				return;
			}else{
				action++;
				if(recNum1==storeNum || recNum2==storeNum){		// 远端第二交叉路口左侧
					 jmpAction = 70; joint2 = 6;		//左
					 recNum1 = 0;  recNum2 = 0;
				}else{											// 远端第二交叉路口右侧
					 jmpAction = 70; joint2 = 7;		//右
					recNum1 = 0;  recNum2 = 0;
				}
			}
			#else
			action++;
			if(1 || (nowNum1==storeNum && nowNum2==storeNum)){		// 远端第二交叉路口左侧
				 jmpAction = 70; joint2 = 6;		//左
			}else{											// 远端第二交叉路口右侧
				 jmpAction = 70; joint2 = 7;		//右
			}
			#endif
		break;
		case 66: action = jmpAction; break;
		/*--------------------------远端病房&退回------------------------------*/
		case 70: Car_Forward(200,SPEED); action++; lock = 1; break;	//前进一些
		case 71:
			if(joint2 == 6) { Car_Round(VERTICAL_L,TURN_SPEED,0); }		//左转
			else              		{ Car_Round(VERTICAL_R,TURN_SPEED,1); }		//右转
			action++; lock = 1;
		break;
		case 72: Car_Forward(50,SPEED); action++; lock = 1; break;	//前进一些
		case 73: Car_Track(400,SPEED,1); action++; lock = 1; break;
		case 74: RedLed = 1; if(!Redwire_isFull()){action++;} break;
		case 75: RedLed = 0; Car_Back(450,SPEED); action++; lock = 1; break;
		case 76:
			if(joint2 == 6) { Car_Round(VERTICAL_L,TURN_SPEED,0); }		//左转
			else              		{ Car_Round(VERTICAL_R,TURN_SPEED,1); }		//右转
			action++; lock = 1;
		break;
		case 77: Car_Track(750,SPEED,1); action++; lock = 1; break;
		case 78: Car_Forward(150,SPEED); action++; lock = 1; break;	//前进一些
		case 79:
			if(joint == 4) { Car_Round(VERTICAL_L,TURN_SPEED,1); }		//右转
			else           { Car_Round(VERTICAL_R,TURN_SPEED,0); }		//左转
			action++; lock = 1;
		break;
		case 80: Car_Forward(50,SPEED); action++; lock = 1; break;	//前进一些
		case 81: Car_Track(2450,SPEED,1); action++; lock = 1; break;
		case 82: GreLed = 1; break;		/*@@@@@@@@@@@@ 结束 @@@@@@@@@@@@@*/
		
		default: break;
	}
}