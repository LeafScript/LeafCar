#ifndef __TASKCONF_H
#define __TASKCONF_H

#include "sys.h"

//-------调试步骤
//1. 1m距离补偿                先不补偿，再补偿，算出     走中端    MUTI_PER_METER     正
//2. 转弯前补偿前进的距离      先不补偿，再补偿，算出     走近端    ROUND_MUTI         正
//3. 转弯后补偿前进的距离      先不补偿，再补偿，算出     走近端    ROUND_MUTI_NEXT	   负


//默认距离补偿 编号
//#define DEFAULT_MUTI		10000
#define DEFAULT_MUTI		0
//1M 距离补偿      1m  -mm
//小车2:                            12---------------------------------------------
#define MUTI_PER_METER		0

//有重物和没重物的距离补偿
#define MUTI_LENGHTH		0
//有重物转弯的前进距离补偿 -mm      30---------------------------------------------
#define ROUND_MUTI			0
//有重物转弯的前进距离补偿 -mm      20---------------------------------------------
#define ROUND_MUTI_NEXT		0


#define MUTI_LENGHTH_TEST  -15
//#define MUTI_LENGHTH_TEST  0
#define ROUND_MUTI_TEST		30
//#define ROUND_MUTI_TEST		0

//测试模式
#define TASK_TEST		0

//行进速度
#define SPEED		400
//转弯速度
#define TURN_SPEED	700
//直角转弯路程
#define VERTICAL_L	190
#define VERTICAL_R	190


#endif