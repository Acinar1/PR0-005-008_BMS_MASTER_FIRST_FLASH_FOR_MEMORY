/**
  ******************************************************************************
  * File Name          : usart_ex.h
  * Version            : 1.0
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#pragma once

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"

/* USER CODE BEGIN Includes */

#include <stdbool.h>
#include <stdint.h>

/* USER CODE END Includes */

typedef enum{
	Uart_Classic = 0,
	Uart_HalfDuplex,
	Uart_485,
}eUartType;

/* USER CODE BEGIN Private defines */

#define ENABLE_SOFT_RE_DE_4_RS485	0

/* USER CODE END Private defines */

// UART5 functions
#define UART5_RxInit()				s_uart5.RX_init(&s_uart5)
#define UART5_RxBuffClr()			s_uart5.RX_buffer_clear(&s_uart5)
#define	UART5_RxBuffUsage()			s_uart5.RX_get_buffer_usage(&s_uart5)
#define	UART5_RxPop(data)			s_uart5.RX_get(&s_uart5, data)
#define	UART5_TxBuffUsage()			s_uart5.TX_get_buffer_usage(&s_uart5)
#define	UART5_TxPut(data, size)		s_uart5.TX_put(&s_uart5, data, size)

// USART1 functions
#define USART1_RxInit()				s_usart1.RX_init(&s_usart1)
#define USART1_RxBuffClr()			s_usart1.RX_buffer_clear(&s_usart1)
#define	USART1_RxBuffUsage()			s_usart1.RX_get_buffer_usage(&s_usart1)
#define	USART1_RxPop(data)			s_usart1.RX_get(&s_usart1, data)
#define	USART1_TxBuffUsage()			s_usart1.TX_get_buffer_usage(&s_usart1)
#define	USART1_TxPut(data, size)		s_usart1.TX_put(&s_usart1, data, size)

/* USER CODE BEGIN Macros */

/* USER CODE END Macros */

// Uart Struct
typedef struct __HandlerUart_s {
	// handle
	UART_HandleTypeDef* phuart;					//! pointer to the uart handle
	uint16_t u16_TxRx_Mask;						//! mask for the Tx or Rx data
	eUartType  oUartType;
	uint8_t u8_485_DeassertTime;
	uint8_t u8_485_AssertTime;
	uint8_t u32_485_Polarity;
#if (ENABLE_SOFT_RE_DE_4_RS485 != 0)
	bool  bREDE_Soft_Enable;
	GPIO_TypeDef* GPIO_REDE_Port;
	uint16_t GPIO_REDE_Pin;
#endif
	// reception
	bool  bRxIT_DMA;
	bool  bRTS_Enable;
	GPIO_TypeDef* GPIO_RTS_Port;
	uint16_t GPIO_RTS_Pin;
	uint16_t u16_uart_rx_usage_4_rts;
	uint8_t* pu8_uart_rx_buffer;				//! pointer to the uart reception buffer
	uint16_t u16_uart_rx_buffer_size;			//! size of transmission buffer
	uint16_t u16_uart_rx_read_ptr;				//! pointer to the read index of reception buffer
	uint16_t u16_uart_rx_write_ptr;				//! pointer to the write index of RX buffer
	DMA_HandleTypeDef *hdmarx;                  /*!< UART Rx DMA Handle parameters      */
	void (*RX_init) (struct __HandlerUart_s* s_uart_info);
	void (*RX_buffer_clear) (struct __HandlerUart_s* s_uart_info);
	uint16_t (*RX_get_buffer_usage) (struct __HandlerUart_s* s_uart_info);
	uint8_t (*RX_get) (struct __HandlerUart_s* s_uart_info, uint8_t* pu8_data);
	// transmission
	bool	bTxIT_DMA;
	uint8_t* pu8_uart_tx_buffer;				//! pointer to the uart transmission buffer
	uint16_t u16_uart_tx_buffer_size;			//! size of transmission buffer
	uint16_t u16_uart_tx_write_ptr;				//! pointer to the write index of transmission buffer
	uint16_t u16_uart_tx_read_ptr;				//! pointer to the read index of TX buffer
	DMA_HandleTypeDef *hdmatx;                  /*!< UART Tx DMA Handle parameters      */
	uint16_t (*TX_get_buffer_usage) (struct __HandlerUart_s* s_uart_info);
	uint8_t (*TX_put) (struct __HandlerUart_s* s_uart_info, uint8_t* pData, uint16_t u16_size);
}uart_s;

extern uart_s s_uart5;
extern uart_s s_usart1;

/* USER CODE BEGIN Private Exported variables */

/* USER CODE END Private Exported variables */

/************************ COMMON ********************************/

void UART_Ex_Init(void);

//it call back
void UART_IrqHandler(uart_s* s_uart_info);

void UART_SetBaudrate(uart_s* s_uart_info, uint32_t u32_BaudRate);

/************************ POOLING *******************************/

uint8_t _UART_put(struct __HandlerUart_s* s_uart_info, uint8_t* pData, uint16_t u16_size);
uint8_t _HalfDuplex_UART_put(struct __HandlerUart_s* s_uart_info, uint8_t* pData, uint16_t u16_size);
uint8_t _UART_get(struct __HandlerUart_s* s_uart_info, uint8_t* pu8_data);

/*************************** IT *********************************/

// rx over it
void UART_RX_IT_init(struct __HandlerUart_s* s_uart_info);
uint8_t UART_IT_pop(struct __HandlerUart_s* s_uart_info, uint8_t* pu8_data);
uint16_t UART_RX_IT_get_buffer_usage(struct __HandlerUart_s* s_uart_info);
void UART_RX_IT_buffer_clear(struct __HandlerUart_s* s_uart_info);

// tx over it
uint8_t UART_IT_put(struct __HandlerUart_s* s_uart_info, uint8_t *pData, uint16_t u16_size );
uint8_t HalfDuplex_UART_IT_put(struct __HandlerUart_s* s_uart_info, uint8_t *pData, uint16_t u16_size );
void UART_TX_IT_send_next_data(uart_s* s_uart_info);
uint16_t UART_TX_IT_get_buffer_usage(struct __HandlerUart_s* s_uart_info);

/************************** DMA *********************************/

// rx over dma
void UART_RX_DMA_init(struct __HandlerUart_s* s_uart_info);
uint8_t UART_RX_DMA_stop(uart_s* s_uart_info);
uint8_t UART_DMA_pop(struct __HandlerUart_s* s_uart_info, uint8_t* pu8_data);
uint16_t UART_RX_DMA_get_buffer_usage(struct __HandlerUart_s* s_uart_info);
void UART_RX_DMA_buffer_clear(struct __HandlerUart_s* s_uart_info);
void UART_DMA_RxCpltCallback(DMA_HandleTypeDef *hdma);

// tx over dma
uint8_t UART_DMA_put(struct __HandlerUart_s* s_uart_info, uint8_t* pData, uint16_t u16_size);
uint8_t HalfDuplex_UART_DMA_put(struct __HandlerUart_s* s_uart_info, uint8_t* pData, uint16_t u16_size);
uint8_t UART_TX_DMA_stop(uart_s* s_uart_info);
uint16_t UART_TX_DMA_get_buffer_usage(struct __HandlerUart_s* s_uart_info);
void UART_DMA_TxCpltCallback(DMA_HandleTypeDef *hdma);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

