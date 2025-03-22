#include "k210.h"
#include "base_type.h"

//接收数字0x01~0x08
uint8_t recNum1 = 0;
uint8_t recNum2 = 0;
//接收完成一个数字标志
uint8_t ok_flag1 = 0;	//左边数字
uint8_t ok_flag2 = 0;	//右边数字

void k210_update_data(uint8_t ch)
{
    if (ok_flag1 == 1) {
        ok_flag1 = 0;
        recNum1 = ch - 48;
    } else if(ok_flag1 == 0 && ch == 'J') {
        ok_flag1 = 1;
    }
    if (ok_flag2 == 1) {
        ok_flag2 = 0;
        recNum2 = ch - 48;
    } else if(ok_flag2 == 0 && ch == 'K') {
        ok_flag2 = 1;
    }
}