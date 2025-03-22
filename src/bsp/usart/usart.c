#include "stm32f10x_conf.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "usart.h"

#pragma import(__use_no_semihosting)             

struct __FILE {
	int handle; 
}; 
FILE __stdout;       

void _sys_exit(int x)
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{      
	while((USART_n->SR & 0x40)==0);
    USART_n->DR = (uint8_t)ch;      
	return ch;
}

typedef struct {
	USART_TypeDef *USARTx;
	uint32_t RCC_APB2Periph;
	uint32_t baudrate;
	uint8_t nvic_irq;
	uint8_t nvic_pri;
	uint8_t nvic_sub_pri;
	GPIO_TypeDef *tx_gpio;
	uint16_t tx_pin;
	GPIO_TypeDef *rx_gpio;
	uint16_t rx_pin;
	uint16_t USART_IT;
} usart_init_info_s; 

#define USART_PRINTF(USARTn, recv_len) \
void USARTn##_printf(char *fmt, ...) \
{ \
	char buffer[(recv_len) + 1]; \
	uint32_t i = 0;	\
	va_list arg_ptr; \
	va_start(arg_ptr, fmt); \
	vsnprintf(buffer, (recv_len) + 1, fmt, arg_ptr); \
	while ((i < (recv_len)) && (i < strlen(buffer))) { \
        USART_SendData(USARTn, (uint16_t)buffer[i++]); \
        while (USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET); \
	} \
	va_end(arg_ptr); \
}

USART_PRINTF(USART1, USART1_REC_LEN)
USART_PRINTF(USART2, USART2_REC_LEN)
USART_PRINTF(USART3, USART3_REC_LEN)
USART_PRINTF(UART4, USART4_REC_LEN)
USART_PRINTF(UART5, USART5_REC_LEN)

static usart_serv_func g_serv_func[USART_SERV_MAX] = { 0 };

void usart_service_register(uint8_t usartx, usart_serv_func func)
{
	if (usartx >= USART_SERV_MAX) {
		return;
	}
	g_serv_func[usartx] = func;
}

static void usart_init(usart_init_info_s *info)
{
	USART_InitTypeDef usart_init = { info->baudrate, USART_WordLength_8b, USART_StopBits_1,
		USART_Parity_No, USART_Mode_Rx | USART_Mode_Tx, USART_HardwareFlowControl_None };
	NVIC_InitTypeDef nvic_init = { info->nvic_irq, info->nvic_pri, info->nvic_sub_pri, ENABLE };
	GPIO_InitTypeDef gpio_init;

	RCC_APB2PeriphClockCmd(info->RCC_APB2Periph, ENABLE);

	gpio_init.GPIO_Pin = info->tx_pin;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(info->tx_gpio, &gpio_init);

	gpio_init.GPIO_Pin = info->rx_pin;
	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(info->rx_gpio, &gpio_init);

	NVIC_Init(&nvic_init);
	USART_Init(info->USARTx, &usart_init);
	USART_ITConfig(info->USARTx, info->USART_IT, ENABLE);
	USART_ITConfig(info->USARTx, USART_IT_IDLE, ENABLE);
	USART_Cmd(info->USARTx, ENABLE);
}

void USART1_Init(uint32_t baudrate)
{
	usart_init_info_s info = {
		USART1, RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, baudrate, USART1_IRQn, 3, 3,
		GPIOA, GPIO_Pin_9, GPIOA, GPIO_Pin_10, USART_IT_RXNE
	};
	usart_init(&info);
}

uint8_t USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART1_RX_STA=0;       //接收状态标记	 

void USART1_IRQHandler(void)
{
	uint8_t Res;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((USART1_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART1_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
				else USART1_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART1_RX_STA|=0x4000;
				else
				{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
     }
}

void USART2_Init(uint32_t baudrate)
{
	usart_init_info_s info = {
		USART2, RCC_APB1Periph_USART2 | RCC_APB2Periph_GPIOA, baudrate, USART2_IRQn, 3, 1,
		GPIOA, GPIO_Pin_2, GPIOA, GPIO_Pin_3, USART_IT_RXNE
	};
	usart_init(&info);
}

void USART2_IRQHandler(void)
{
	uint8_t ch;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		ch = USART_ReceiveData(USART2);
		if (g_serv_func[USART_SERV_USART2] != NULL) {
			g_serv_func[USART_SERV_USART2](ch);
		}
	}
}

void USART3_Init(uint32_t baudrate)
{
	usart_init_info_s info = {
		USART3, RCC_APB1Periph_USART3 | RCC_APB2Periph_GPIOB, baudrate, USART3_IRQn, 2, 3,
		GPIOB, GPIO_Pin_10, GPIOB, GPIO_Pin_11, USART_IT_RXNE
	};
	usart_init(&info);
}

void USART3_IRQHandler(void)
{ 
	uint8_t ch;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		ch = USART_ReceiveData(USART3);
		if (g_serv_func[USART_SERV_USART3] != NULL) {
			g_serv_func[USART_SERV_USART3](ch);
		}
	}
}

void UART4_Init(uint32_t baudrate)
{
	usart_init_info_s info = {
		UART4, RCC_APB1Periph_UART4 | RCC_APB2Periph_GPIOC, baudrate, UART4_IRQn, 3, 2,
		GPIOC, GPIO_Pin_10, GPIOC, GPIO_Pin_11, USART_IT_RXNE
	};
	usart_init(&info);
}

void UART4_IRQHandler(void)
{
	uint8_t ch;
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		ch = USART_ReceiveData(UART4);
		if (g_serv_func[USART_SERV_UART4] != NULL) {
			g_serv_func[USART_SERV_UART4](ch);
		}
	}
}

void UART5_Init(uint32_t baudrate)
{
	usart_init_info_s info = {
		UART5, RCC_APB1Periph_UART5 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, baudrate, UART5_IRQn, 2, 0,
		GPIOC, GPIO_Pin_12, GPIOD, GPIO_Pin_2, USART_IT_RXNE
	};
	usart_init(&info);
}

void UART5_IRQHandler(void)
{
	uint8_t ch;
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		ch = USART_ReceiveData(UART5);
		if (g_serv_func[USART_SERV_UART5] != NULL) {
			g_serv_func[USART_SERV_UART5](ch);
		}
	}
}