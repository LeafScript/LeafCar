#include "k210.h"
#include "base_type.h"

//接收数字0x01~0x08
uint8_t recNum1 = 0;
uint8_t recNum2 = 0;
//接收完成一个数字标志
uint8_t ok_flag1 = 0;	//左边数字
uint8_t ok_flag2 = 0;	//右边数字
