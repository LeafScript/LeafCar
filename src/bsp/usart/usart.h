#ifndef __USART_H
#define __USART_H

#include "base_type.h"

#define USART_n		USART1  //定义使用printf函数的串口，其他串口要使用USART_printf专用函数发送

#define USART1_REC_LEN  			200  	//定义USART1最大接收字节数
#define USART2_REC_LEN  			200  	//定义USART2最大接收字节数
#define USART3_REC_LEN  			200  	//定义USART3最大接收字节数
#define USART4_REC_LEN  			200  	//定义USART4最大接收字节数
#define USART5_REC_LEN  			200  	//定义USART4最大接收字节数

//不使用某个串口时要禁止此串口，以减少编译量
#define EN_USART1 			1		//使能（1）/禁止（0）串口1
#define EN_USART2 			0		//使能（1）/禁止（0）串口2
#define EN_USART3 			1		//使能（1）/禁止（0）串口3
#define EN_USART4 			1		//使能（1）/禁止（0）串口4  
#define EN_USART5 			1		//使能（1）/禁止（0）串口5  

extern uint8_t USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern uint8_t  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern uint8_t  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
 
extern uint16_t USART1_RX_STA;         		//接收状态标记
extern uint16_t USART2_RX_STA;         		//接收状态标记
extern uint16_t USART3_RX_STA;         		//接收状态标记

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
void USART4_printf(char* fmt,...); //串口4的专用printf函数
void USART5_printf(char* fmt,...); //串口5的专用printf函数

#endif