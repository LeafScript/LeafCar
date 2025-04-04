#ifndef _USART_H_
#define _USART_H_

#include "base_type.h"

// the usart which printf is used
#define USART_n		USART1

// usart max transmit length
#define USART1_MAX_TX_LEN  			200
#define USART2_MAX_TX_LEN  			200
#define USART3_MAX_TX_LEN  			200
#define USART4_MAX_TX_LEN  			200
#define USART5_MAX_TX_LEN  			200

enum usart_serv_usartx {
    USART_SERV_USART1,
    USART_SERV_USART2,
    USART_SERV_USART3,
    USART_SERV_UART4,
    USART_SERV_UART5,
    USART_SERV_MAX
};
typedef void (*usart_serv_func)(uint8_t ch);

#define printf USART1_printf

void USART1_Init(uint32_t baudrate);
void USART2_Init(uint32_t baudrate);
void USART3_Init(uint32_t baudrate);
void UART4_Init(uint32_t baudrate);
void UART5_Init(uint32_t baudrate);
void USART1_printf(char *fmt,...);
void USART2_printf(char *fmt,...);
void USART3_printf(char *fmt,...);
void UART4_printf(char *fmt, ...);
void UART5_printf(char *fmt, ...);
void usart_service_register(uint8_t usartx, usart_serv_func func);

#endif