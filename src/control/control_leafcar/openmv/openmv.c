#include "openmv.h"
#include "base_type.h"

//接收偏移程度
//       左          右
//	强度: -40 ~ +40
int8_t carOffset = 0;
//接收线的"斜率"
int8_t line_k = 0;

void openmv_update_data(uint8_t ch)
{
	static uint8_t ok_flag = 0;
    if (ok_flag == 1) {
        if (ch == 'd') {
            ok_flag = 0;
        } else {
            ok_flag = 2;
            carOffset = ch;
        }
    } else if (ok_flag == 2) {
        if ( ch == 'd') {
            ok_flag = 0;
        } else {
            ok_flag = 0;
            line_k = ch;
        }
    } else if (ok_flag == 0 && ch == 'd') {
        ok_flag = 1;
    }
}