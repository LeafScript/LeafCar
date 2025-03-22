#ifndef _USART_H_
#define _USART_H_

#include "base_type.h"

#define USART_n		USART1  //定义使用printf函数的串口，其他串口要使用USART_printf专用函数发送

#define USART1_REC_LEN  			200  	//定义USART1最大接收字节数
#define USART2_REC_LEN  			200  	//定义USART2最大接收字节数
#define USART3_REC_LEN  			200  	//定义USART3最大接收字节数
#define USART4_REC_LEN  			200  	//定义USART4最大接收字节数
#define USART5_REC_LEN  			200  	//定义USART4最大接收字节数

enum usart_serv_usartx {
    USART_SERV_USART1,
    USART_SERV_USART2,
    USART_SERV_USART3,
    USART_SERV_UART4,
    USART_SERV_UART5,
    USART_SERV_MAX
};
typedef void (*usart_serv_func)(uint8_t ch);

extern uint8_t USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
 
extern uint16_t USART1_RX_STA;         		//接收状态标记

#define printf USART1_printf

//函数声明
void USART1_Init(uint32_t bound);//串口1初始化并启动
void USART2_Init(uint32_t bound);//串口2初始化并启动
void USART3_Init(uint32_t bound);//串口3初始化并启动
void UART4_Init(uint32_t bound);//串口4初始化并启动
void UART5_Init(uint32_t bound);//串口5初始化并启动
void USART1_printf(char* fmt,...); //串口1的专用printf函数
void USART2_printf(char* fmt,...); //串口2的专用printf函数
void USART3_printf(char* fmt,...); //串口3的专用printf函数
void UART4_printf(char* fmt,...); //串口4的专用printf函数
void UART5_printf(char* fmt,...); //串口5的专用printf函数
void usart_service_register(uint8_t usartx, usart_serv_func func);
;
#endif