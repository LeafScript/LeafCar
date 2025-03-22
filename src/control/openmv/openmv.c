#include "openmv.h"
#include "base_type.h"

//接收偏移程度
//       左          右
//	强度: -40 ~ +40
int8_t carOffset = 0;
//接收线的"斜率"
int8_t line_k = 0;
