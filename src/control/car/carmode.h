#ifndef __CARMODE_H
#define __CARMODE_H

#include "sys.h"

//小车补偿距离判断
#define DISMUTI_JUDGE		do{\
													if(dismuti >= 10000-1000){\
														dismuti -= 10000;\
														dismuti += (float)(dis/1000)*MUTI_PER_METER;\
													}\
												}while(0);

//小车模式配置初始化
#define Config_StopMove()    MyCar.mode = STOP_MOVE;\
							 Car_Stop()
							 
#define Config_FreeMove()    MyCar.mode = FREE_MOVE;\
                             MyCar.config.freeMove.dir = STOP;\
							 MyCar.config.freeMove.distance = 0;\
							 MyCar.config.freeMove.speed = 0;\
							 Car_Start()
							 
#define Config_TargetMove()    MyCar.mode = TARGET_MOVE;\
                               MyCar.config.targetMove.dir = STOP;\
	                           MyCar.config.targetMove.distance = 0;\
							   MyCar.config.targetMove.speed = 0;\
							   MyCar.config.targetMove.targetDist = 0
							 
#define Config_TrackMove()    MyCar.mode = TRACK_MOVE;\
                              MyCar.config.trackMove.dir = STOP;\
							  MyCar.config.trackMove.distance = 0;\
                              MyCar.config.trackMove.encVal = 0;\
							  MyCar.config.trackMove.speed = 0;\
							  MyCar.config.trackMove.turnDist = 0
							 
#define Config_RoundMove()     MyCar.mode = ROUND_MOVE;\
                               MyCar.config.roundMove.dir = STOP;\
	                           MyCar.config.roundMove.distance = 0;\
							   MyCar.config.roundMove.speed = 0;\
							   MyCar.config.roundMove.targetDist = 0

//小车模式
#define STOP_MOVE           0       // 停止模式     - 小车停止
#define FREE_MOVE			1		// 自由移动模式 - 前后以一定速度循环移动
#define TARGET_MOVE  		2		// 移动目标距离 - 前后移动，可以控制移动距离和速度
#define TRACK_MOVE			3		// 循迹模式     - 循迹，可以控制循迹移动距离和速度
#define ROUND_MOVE			4		// 转动目标距离 - 转动，可以控制转动距离和速度

//小车运动方向
#define RIGHT_ROUND			3
#define LEFT_ROUND			4

//小车扫描周期-ms，计算速度用
#define CAR_SCAN_PERIOD     10
//100ms/小车扫描周期-ms
#define CAR_PERIOD_TIMES    (100.0/CAR_SCAN_PERIOD)
//#define CAR_PERIOD_TIMES    10

//小车模式记录结构体
typedef struct{
	uint8_t mode;		//小车模式
	union{
		// 自由移动模式 - 前后以一定速度循环移动
		struct{
			uint8_t dir;        //小车运动方向
			float speed;        //小车速度，100ms内编码值
			float distance;		//小车运动距离
		}freeMove;
		// 移动目标距离 - 前后移动，可以控制移动距离和速度
		struct{
			uint8_t dir;        //小车运动方向
			float speed;        //小车速度，100ms内编码值
			float distance;		//小车运动距离
			float targetDist;	//目标位移
		}targetMove;
		// 循迹模式     - 循迹，可以控制循迹移动距离和速度
		struct{
			uint8_t dir;        //小车运动方向
			float speed;        //小车速度，100ms内编码值
			float distance;		//小车运动距离
			float turnDist;		//目标转弯路程
			int16_t encVal;		//设定速度对应编码值
		}trackMove;
		// 转动目标距离 - 转动，可以控制转动距离和速度
		struct{
			uint8_t dir;        //小车运动方向		RIGHT_ROUND  LEFT_ROUND
			float speed;        //小车速度，100ms内编码值
			float distance;		//小车运动距离
			float targetDist;	//目标位移
		}roundMove;
	}config;
}sCar;

void CarMode_Init(void);
void CarMode_Scan(void);
void CarMode_FreeMove(void);
void CarMode_TargetMove(void);
void CarMode_TrackMove(void);
void CarMode_RoundMove(void);
void CarMode_Choose(uint8_t mode);
void Car_Forward(uint16_t dis, uint16_t speed);
void Car_Back(uint16_t dis, uint16_t speed);
void Car_Round(uint16_t dis, uint16_t speed, uint8_t dir);
void Car_Track(uint32_t dis, uint16_t speed, uint8_t dir);
void Car_Forward_Muti(uint16_t dis, uint16_t speed, int16_t dismuti);
void Car_Back_Muti(uint16_t dis, uint16_t speed, int16_t dismuti);
void Car_Track_Muti(uint32_t dis, uint16_t speed, uint8_t dir, int16_t dismuti);
void Car_Round_Muti(uint16_t dis, uint16_t speed, uint8_t dir, int16_t dismuti);

#endif