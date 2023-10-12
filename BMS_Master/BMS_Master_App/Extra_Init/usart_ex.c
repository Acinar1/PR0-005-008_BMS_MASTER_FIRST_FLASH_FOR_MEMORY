/**
  ******************************************************************************
  * File Name          : usart_ex.c
  * Version            : 1.1
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "usart_ex.h"

#if __has_include("ModbusSlave/modbus.h")
#include "ModbusSlave/modbus.h"
#else
bool bModBusInitialized = false;
#endif

#if __has_include("Debug/Test.h")
#include "Debug/Test.h"
#else
bool bRunTest = false;
#endif

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/**
  * Definir la taille des buffer de reception et emission pour chaque UART
  * Definir le port et le numero de pin du RTS de chaque UART si besoin
  */
/* USER CODE BEGIN Private defines */

#define UART5_TX_BUFFER_SIZE		512
#define UART5_RX_BUFFER_SIZE		4096

#define USART1_TX_BUFFER_SIZE		512
#define USART1_RX_BUFFER_SIZE		128

/* USER CODE END Private defines */

uart_s s_uart5;
uint8_t au8_uart5_tx_buffer[UART5_TX_BUFFER_SIZE];
uint8_t au8_uart5_rx_buffer[UART5_RX_BUFFER_SIZE];

uart_s s_usart1;
uint8_t au8_usart1_tx_buffer[USART1_TX_BUFFER_SIZE];
uint8_t au8_usart1_rx_buffer[USART1_RX_BUFFER_SIZE];
#define husart1		huart1

/* USER CODE BEGIN Private variables */

/* USER CODE END Private variables */

extern DMA_HandleTypeDef hdma_uart5_rx;
extern DMA_HandleTypeDef hdma_uart5_tx;
extern DMA_HandleTypeDef hdma_usart1_tx;

void Dummy_init(struct __HandlerUart_s* s_uart_info);
uint16_t Dummy_RX_get_buffer_usage(struct __HandlerUart_s* s_uart_info);
void Dummy_buffer_clear(struct __HandlerUart_s* s_uart_info);
uint16_t Dummy_TX_get_buffer_usage(struct __HandlerUart_s* s_uart_info);

/* USER CODE BEGIN Private function prototypes */

/* USER CODE END Private function prototypes */

#if defined(STM32F7)
typedef struct
{
  __IO uint32_t ISR; /*!< DMA interrupt status register */
  __IO uint32_t Reserved0;
  __IO uint32_t IFCR; /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;
#endif

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void UART_Ex_Init(void)
{

/*			UART5			*/
/* USER CODE BEGIN INIT_UART5 */

/* USER CODE END INIT_UART5 */
	// UART handle
	s_uart5.phuart = &huart5;
	s_uart5.u16_TxRx_Mask = 0xFFFF >> (16 - (8 - 0));
	s_uart5.oUartType = Uart_Classic;
	// RX buffer
	s_uart5.bRTS_Enable = false;
	s_uart5.pu8_uart_rx_buffer = au8_uart5_rx_buffer;
	s_uart5.u16_uart_rx_buffer_size = UART5_RX_BUFFER_SIZE;
	s_uart5.u16_uart_rx_read_ptr = 0;
	s_uart5.u16_uart_rx_write_ptr = 0;
	s_uart5.RX_init = UART_RX_DMA_init;
	s_uart5.RX_buffer_clear = UART_RX_DMA_buffer_clear;
	s_uart5.RX_get_buffer_usage = UART_RX_DMA_get_buffer_usage;
	s_uart5.RX_get = UART_DMA_pop;
	// TX buffer
	s_uart5.pu8_uart_tx_buffer = au8_uart5_tx_buffer;
	s_uart5.u16_uart_tx_buffer_size = UART5_TX_BUFFER_SIZE;
	s_uart5.u16_uart_tx_write_ptr = 0;
	s_uart5.u16_uart_tx_read_ptr = 0;
	s_uart5.TX_get_buffer_usage = UART_TX_DMA_get_buffer_usage;
	s_uart5.TX_put = UART_DMA_put;
	// DMA RX
	__HAL_LINKDMA(&s_uart5, hdmarx, hdma_uart5_rx);
	// DMA TX
	__HAL_LINKDMA(&s_uart5, hdmatx, hdma_uart5_tx);

	/* USER CODE BEGIN EXTRA_INIT_UART5 */
#if (ENABLE_SOFT_RE_DE_4_RS485 != 0)
	s_uart5.bREDE_Soft_Enable = false;
//	s_uart5.GPIO_REDE_Port = GPIOx;
//	s_uart5.GPIO_REDE_Pin = GPIO_PIN_xx;
#endif
	/* USER CODE END EXTRA_INIT_UART5 */

/*			USART1			*/
/* USER CODE BEGIN INIT_USART1 */

/* USER CODE END INIT_USART1 */
	// UART handle
	s_usart1.phuart = &husart1;
	s_usart1.u16_TxRx_Mask = 0xFFFF >> (16 - (8 - 0));
	s_usart1.oUartType = Uart_Classic;
	// RX buffer
	s_usart1.bRTS_Enable = false;
	s_usart1.pu8_uart_rx_buffer = au8_usart1_rx_buffer;
	s_usart1.u16_uart_rx_buffer_size = USART1_RX_BUFFER_SIZE;
	s_usart1.u16_uart_rx_read_ptr = 0;
	s_usart1.u16_uart_rx_write_ptr = 0;
	s_usart1.RX_init = UART_RX_IT_init;
	s_usart1.RX_buffer_clear = UART_RX_IT_buffer_clear;
	s_usart1.RX_get_buffer_usage = UART_RX_IT_get_buffer_usage;
	s_usart1.RX_get = UART_IT_pop;
	// TX buffer
	s_usart1.pu8_uart_tx_buffer = au8_usart1_tx_buffer;
	s_usart1.u16_uart_tx_buffer_size = USART1_TX_BUFFER_SIZE;
	s_usart1.u16_uart_tx_write_ptr = 0;
	s_usart1.u16_uart_tx_read_ptr = 0;
	s_usart1.TX_get_buffer_usage = UART_TX_DMA_get_buffer_usage;
	s_usart1.TX_put = UART_DMA_put;
	// DMA TX
	__HAL_LINKDMA(&s_usart1, hdmatx, hdma_usart1_tx);

	/* USER CODE BEGIN EXTRA_INIT_USART1 */
#if (ENABLE_SOFT_RE_DE_4_RS485 != 0)
	s_usart1.bREDE_Soft_Enable = false;
//	s_usart1.GPIO_REDE_Port = GPIOx;
//	s_usart1.GPIO_REDE_Pin = GPIO_PIN_xx;
#endif
	/* USER CODE END EXTRA_INIT_USART1 */

/* USER CODE BEGIN Init */

/* USER CODE END Init */

	// Init UART5 RX
	s_uart5.RX_init(&s_uart5);
	s_uart5.RX_buffer_clear(&s_uart5);

	// Init USART1 RX
	s_usart1.RX_init(&s_usart1);
	s_usart1.RX_buffer_clear(&s_usart1);

/* USER CODE BEGIN Init 2 */

/* USER CODE END Init 2 */

}

/***************************** POOLING GENERIC FUNCTIONS ************************/

uint8_t _UART_get(struct __HandlerUart_s* s_uart_info, uint8_t* pu8_data)
{
  if (s_uart_info->phuart->Instance->ISR & USART_ISR_ORE)		// overrun flag is set ?
    s_uart_info->phuart->Instance->ICR |= USART_ICR_ORECF;	// clear !

  if (s_uart_info->phuart->Instance->ISR & UART_FLAG_RXNE)
  {
    *pu8_data = s_uart_info->phuart->Instance->RDR;
    return 1;
  }
  else
    return 0;
}

uint8_t _UART_put(struct __HandlerUart_s* s_uart_info, uint8_t* pData, uint16_t u16_size)
{
#if (ENABLE_SOFT_RE_DE_4_RS485 != 0)
	if (s_uart_info->bREDE_Soft_Enable)
		s_uart_info->GPIO_REDE_Port->BSRR = s_uart_info->GPIO_REDE_Pin;
#endif

  while (u16_size > 0)
  {
    if (s_uart_info->phuart->Instance->ISR & USART_ISR_ORE)		// overrun flag is set ?
      s_uart_info->phuart->Instance->ICR |= USART_ICR_ORECF;	// clear !

      // if output shifter not empty
    while (~s_uart_info->phuart->Instance->ISR & UART_FLAG_TXE);
    //		return 0;	// can't send byte now.

        // transmit the byte
    s_uart_info->phuart->Instance->TDR = *(pData++);
    u16_size--;
  }
#if (ENABLE_SOFT_RE_DE_4_RS485 != 0)
  if (s_uart_info->bREDE_Soft_Enable)
#if defined(STM32F7)
    s_uart_info->GPIO_REDE_Port->BSRR = ((uint32_t)s_uart_info->GPIO_REDE_Pin << 16);
#else
    s_uart_info->GPIO_REDE_Port->BRR = s_uart_info->GPIO_REDE_Pin;
#endif
#endif

  return 1;
}

uint8_t _HalfDuplex_UART_put(struct __HandlerUart_s* s_uart_info, uint8_t* pData, uint16_t u16_size)
{
  if (s_uart_info->oUartType == Uart_HalfDuplex)
  {
    // disable Receiver (for no echo receive)
    __HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_PE);
    __HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_ERR);
    __HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_RXNE);
    __HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_TC);
    __HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_TXE);
    CLEAR_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_RE);
    // enable transmitter
    SET_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_TE);
  }

  uint8_t u8_Ret = _UART_put(s_uart_info, pData, u16_size);

  if (s_uart_info->oUartType == Uart_HalfDuplex)
  {
    // wait until all is transmitted
    while (~s_uart_info->phuart->Instance->ISR & UART_FLAG_TXE);
    while (~s_uart_info->phuart->Instance->ISR & UART_FLAG_TC);

    // disable transmitter
    CLEAR_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_TE);
    // enable receiver
    SET_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_RE);
		s_uart_info->RX_init(s_uart_info);
		s_uart_info->RX_buffer_clear(s_uart_info);
  }

  return u8_Ret;
}

void Dummy_init(struct __HandlerUart_s* s_uart_info)
{
  if (s_uart_info->oUartType == Uart_HalfDuplex)
  {
    // disable transmitter
    CLEAR_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_TE);
    // enable receiver
    SET_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_RE);
  }
#if (ENABLE_SOFT_RE_DE_4_RS485 != 0)
  if (s_uart_info->bREDE_Soft_Enable)
#if defined(STM32F7)
    s_uart_info->GPIO_REDE_Port->BSRR = ((uint32_t)s_uart_info->GPIO_REDE_Pin << 16);
#else
    s_uart_info->GPIO_REDE_Port->BRR = s_uart_info->GPIO_REDE_Pin;
#endif
#endif
}

uint16_t Dummy_RX_get_buffer_usage(struct __HandlerUart_s* s_uart_info)
{
  if (s_uart_info->phuart->Instance->ISR & UART_FLAG_RXNE)
    return 1;
  else
    return 0;
}
void Dummy_buffer_clear(struct __HandlerUart_s* s_uart_info)
{
  uint16_t u16DummyRead = s_uart_info->phuart->Instance->RDR;
  UNUSED(u16DummyRead);
}
uint16_t Dummy_TX_get_buffer_usage(struct __HandlerUart_s* s_uart_info)
{
  UNUSED(s_uart_info);
  return 0;
}

/***************************** IT RX GENERIC FUNCTIONS ************************/

//! put a byte of data into fifo
static void UART_RX_put_fifo(uart_s* s_uart_info)
{
#if (MODBUS_ENABLE != 0)
  if ((s_uart_info == &ModBusUartHandler) && (bModBusInitialized != false) && (bRunTest == false))
  {
		if (pxMBFrameCBByteReceived != NULL)
			pxMBFrameCBByteReceived();
    __HAL_UART_SEND_REQ(s_uart_info->phuart, UART_RXDATA_FLUSH_REQUEST);
  }
	else
#endif
	{
		// copy DR register value to rx buffer
		s_uart_info->pu8_uart_rx_buffer[s_uart_info->u16_uart_rx_write_ptr++] = s_uart_info->phuart->Instance->RDR & s_uart_info->u16_TxRx_Mask;
		s_uart_info->u16_uart_rx_write_ptr %= s_uart_info->u16_uart_rx_buffer_size;
	}

  if (s_uart_info->bRTS_Enable)
  {
    // check if RTS must be set or clear
    if (UART_RX_IT_get_buffer_usage(s_uart_info) >= s_uart_info->u16_uart_rx_usage_4_rts)
    {
#if (UART_RTS_DBG == 1)
      if ((s_uart_info->GPIO_RTS_Port->ODR & (uint32_t)s_uart_info->GPIO_RTS_Pin) == 0)
        LOG_WARNING("UART Buffer Almost full !");
#endif
      // set RTS to indicate that we are almost full
      s_uart_info->GPIO_RTS_Port->BSRR = (uint32_t)s_uart_info->GPIO_RTS_Pin;
    }
    else
    {
#if (UART_RTS_DBG == 1)
      if ((s_uart_info->GPIO_RTS_Port->ODR & (uint32_t)s_uart_info->GPIO_RTS_Pin) != 0)
        LOG_INFO("UART Buffer Freed");
#endif
      // clear RTS to indicate that we are ready to receive data
#if defined(STM32F7)
			s_uart_info->GPIO_RTS_Port->BSRR = ((uint32_t)s_uart_info->GPIO_RTS_Pin << 16);
#else
      s_uart_info->GPIO_RTS_Port->BRR = (uint32_t)s_uart_info->GPIO_RTS_Pin;
#endif
    }
  }
}

//! start a reception over it
void UART_RX_IT_init(struct __HandlerUart_s* s_uart_info)
{
  /* Enable the UART Parity Error Interrupt */
  __HAL_UART_ENABLE_IT(s_uart_info->phuart, UART_IT_PE);

  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(s_uart_info->phuart, UART_IT_ERR);

  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(s_uart_info->phuart, UART_IT_RXNE);

  if (s_uart_info->bRTS_Enable)
  {
    // clear RTS to indicate that we are ready to receive data
#if defined(STM32F7)
    s_uart_info->GPIO_RTS_Port->BSRR = ((uint32_t)s_uart_info->GPIO_RTS_Pin << 16);
#else
    s_uart_info->GPIO_RTS_Port->BRR = (uint32_t)s_uart_info->GPIO_RTS_Pin;
#endif
  }

  if (s_uart_info->oUartType == Uart_HalfDuplex)
  {
    // disable transmitter
    CLEAR_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_TE);
    // enable receiver
    SET_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_RE);
  }
#if (ENABLE_SOFT_RE_DE_4_RS485 != 0)
  if (s_uart_info->bREDE_Soft_Enable)
#if defined(STM32F7)
    s_uart_info->GPIO_REDE_Port->BSRR = ((uint32_t)s_uart_info->GPIO_REDE_Pin << 16);
#else
    s_uart_info->GPIO_REDE_Port->BRR = s_uart_info->GPIO_REDE_Pin;
#endif
#endif
}

//! get a byte from fifo. return count of data returned ( 0 or 1 )
uint8_t UART_IT_pop(struct __HandlerUart_s* s_uart_info, uint8_t* pu8_data)
{
  uint8_t u8_Ret = 0;
	if ( s_uart_info->u16_uart_rx_read_ptr != s_uart_info->u16_uart_rx_write_ptr )	// in index == out index => fifo is empty.
	{
		// extract a byte of data, and store in caller pointer.
		*pu8_data = s_uart_info->pu8_uart_rx_buffer[s_uart_info->u16_uart_rx_read_ptr++];
		// bound index to fifo buffer
		s_uart_info->u16_uart_rx_read_ptr %= s_uart_info->u16_uart_rx_buffer_size;
    u8_Ret = 1;
	}
  if (s_uart_info->bRTS_Enable)
  {
    // check if RTS must be set or clear
    if (UART_RX_IT_get_buffer_usage(s_uart_info) >= s_uart_info->u16_uart_rx_usage_4_rts)
    {
#if (UART_RTS_DBG == 1)
      if ((s_uart_info->GPIO_RTS_Port->ODR & (uint32_t)s_uart_info->GPIO_RTS_Pin) == 0)
        LOG_WARNING("UART Buffer Almost full !");
#endif
      // set RTS to indicate that we are almost full
      s_uart_info->GPIO_RTS_Port->BSRR = (uint32_t)s_uart_info->GPIO_RTS_Pin;
    }
    else
    {
#if (UART_RTS_DBG == 1)
      if ((s_uart_info->GPIO_RTS_Port->ODR & (uint32_t)s_uart_info->GPIO_RTS_Pin) != 0)
        LOG_INFO("UART Buffer Freed");
#endif
      // clear RTS to indicate that we are ready to receive data
#if defined(STM32F7)
      s_uart_info->GPIO_RTS_Port->BSRR = ((uint32_t)s_uart_info->GPIO_RTS_Port << 16);
#else
      s_uart_info->GPIO_RTS_Port->BRR = (uint32_t)s_uart_info->GPIO_RTS_Pin;
#endif
    }
  }
  return u8_Ret;
}

//! return amount of data in receiver fifo buffer ( 0 = empty )
uint16_t UART_RX_IT_get_buffer_usage(struct __HandlerUart_s* s_uart_info)
{
	if ( s_uart_info->u16_uart_rx_read_ptr != s_uart_info->u16_uart_rx_write_ptr )
		return (((s_uart_info->u16_uart_rx_buffer_size - s_uart_info->u16_uart_rx_read_ptr) + s_uart_info->u16_uart_rx_write_ptr)%s_uart_info->u16_uart_rx_buffer_size);
	else
		return 0;
}

//! clear the fifo
void UART_RX_IT_buffer_clear(struct __HandlerUart_s* s_uart_info)
{
	// clear FIFO : move read pointer to the current write pointer position.
	s_uart_info->u16_uart_rx_read_ptr = s_uart_info->u16_uart_rx_write_ptr;
  if (s_uart_info->bRTS_Enable)
  {
    // clear RTS to indicate that we are ready to receive data
#if defined(STM32F7)
    s_uart_info->GPIO_RTS_Port->BSRR = ((uint32_t)s_uart_info->GPIO_RTS_Pin << 16);
#else
    s_uart_info->GPIO_RTS_Port->BRR = (uint32_t)s_uart_info->GPIO_RTS_Pin;
#endif
  }
}

/***************************** IT TX GENERIC FUNCTIONS ************************/

//! put bytes into buffer for send it by IT
uint8_t UART_IT_put(struct __HandlerUart_s* s_uart_info, uint8_t *pData, uint16_t u16_size )
{
	uint32_t u32_free_buffer_memory;
	uint32_t u32_tickstart;
	uint32_t u32_loop;

	// if no buffer, exit function
	if (s_uart_info->pu8_uart_tx_buffer == NULL)
		return 1;

#if (ENABLE_SOFT_RE_DE_4_RS485 != 0)
  if (s_uart_info->bREDE_Soft_Enable)
    s_uart_info->GPIO_REDE_Port->BSRR = s_uart_info->GPIO_REDE_Pin;
#endif

	u32_tickstart = HAL_GetTick();

	do{
		// compute free memory in buffer
		u32_free_buffer_memory = s_uart_info->u16_uart_tx_buffer_size - UART_TX_IT_get_buffer_usage(s_uart_info) - 1;

		// check if enough space in buffer to copy data, else wait until end of previous transfer
		if (u16_size > u32_free_buffer_memory)
		{
			// if no IT transfer, start it, else wait until IT of TransferComplete to start the new one
			if (s_uart_info->phuart->gState == HAL_UART_STATE_READY)
			{
				s_uart_info->phuart->gState = HAL_UART_STATE_BUSY_TX;
				/* Enable the UART Transmit Data Register Empty Interrupt */
				__HAL_UART_ENABLE_IT(s_uart_info->phuart, UART_IT_TXE);
			}

			if ((SCB->ICSR & 0x0000003F) == 0)
			{	// not from ISR => wait some time...
				if ((u16_size > s_uart_info->u16_uart_tx_buffer_size) ||
					((HAL_GetTick() - u32_tickstart) > 1000))
				{
					//LOG_ERROR( "UART : IT Tx overflow");
					return 2;
				}
				HAL_Delay(10);
			}
			else // from ISR => return immediatly
				return 2;

			//if ((s_uart_info->phuart->Instance->ISR & (USART_ISR_TC | USART_ISR_TXE)) != 0)
			//	UART_IrqHandler(s_uart_info);
		}
	}while (u32_free_buffer_memory < u16_size); // until exit on error or enough space

	/* copy data to tx buffer */
	// if data can be put at the end of buffer
	if (u16_size < (s_uart_info->u16_uart_tx_buffer_size - s_uart_info->u16_uart_tx_write_ptr))
	{
//		memcpy(&s_uart_info->pu8_uart_tx_buffer[s_uart_info->u16_uart_tx_write_ptr], pData, u16_size);
		for (u32_loop = 0;u32_loop < u16_size;u32_loop++)
			s_uart_info->pu8_uart_tx_buffer[s_uart_info->u16_uart_tx_write_ptr + u32_loop] = *(pData++);
	}
	else // put first part of data at the end of buffer and the last part at the top of buffer
	{
		uint16_t u16_size_first_part = s_uart_info->u16_uart_tx_buffer_size - s_uart_info->u16_uart_tx_write_ptr;
		// copy first part
//		memcpy(&s_uart_info->pu8_uart_tx_buffer[s_uart_info->u16_uart_tx_write_ptr], pData, u16_size_first_part);
		for (u32_loop = 0;u32_loop < u16_size_first_part;u32_loop++)
			s_uart_info->pu8_uart_tx_buffer[s_uart_info->u16_uart_tx_write_ptr + u32_loop] = *(pData++);
		// copy last part
//		memcpy(&s_uart_info->pu8_uart_tx_buffer[0], &pData[u16_size_first_part], (u16_size-u16_size_first_part));
		for (u32_loop = 0;u32_loop < (u16_size - u16_size_first_part);u32_loop++)
			s_uart_info->pu8_uart_tx_buffer[u32_loop] = *(pData++);
	}

	// compute new write pointer
	s_uart_info->u16_uart_tx_write_ptr += u16_size;
	s_uart_info->u16_uart_tx_write_ptr %= s_uart_info->u16_uart_tx_buffer_size;

	// if no IT transfer, start it, else wait until IT of TransferComplete to start the new one
	if (s_uart_info->phuart->gState == HAL_UART_STATE_READY)
	{
		s_uart_info->phuart->gState = HAL_UART_STATE_BUSY_TX;
		/* Enable the UART Transmit Data Register Empty Interrupt */
		__HAL_UART_ENABLE_IT(s_uart_info->phuart, UART_IT_TXE);
	}

	return 0;
}

uint8_t HalfDuplex_UART_IT_put(struct __HandlerUart_s* s_uart_info, uint8_t *pData, uint16_t u16_size )
{
	// if no IT transfer, start it, else wait until IT of TransferComplete to start the new one
	if (s_uart_info->phuart->gState == HAL_UART_STATE_READY)
	{
		// disable Receiver (for no echo receive)
		__HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_PE);
		__HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_ERR);
		__HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_RXNE);
		__HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_TC);
		__HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_TXE);
		CLEAR_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_RE);
		// enable transmitter
		SET_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_TE);
	}

	return UART_IT_put(s_uart_info, pData, u16_size );
}

//! send next data on uart
void UART_TX_IT_send_next_data(uart_s* s_uart_info)
{
	// copy byte into uart register
  s_uart_info->phuart->Instance->TDR = s_uart_info->pu8_uart_tx_buffer[s_uart_info->u16_uart_tx_read_ptr] & s_uart_info->u16_TxRx_Mask;

	// increase read pointer
	s_uart_info->u16_uart_tx_read_ptr ++;
	s_uart_info->u16_uart_tx_read_ptr %= s_uart_info->u16_uart_tx_buffer_size;

	/* Enable the UART Transmit Data Register Empty Interrupt */
	__HAL_UART_ENABLE_IT(s_uart_info->phuart, UART_IT_TXE);
}

//! compute remaining data to send by the IT buffer.
uint16_t UART_TX_IT_get_buffer_usage(struct __HandlerUart_s* s_uart_info)
{
	if ( s_uart_info->u16_uart_tx_read_ptr != s_uart_info->u16_uart_tx_write_ptr )
	{
		return (((s_uart_info->u16_uart_tx_buffer_size - s_uart_info->u16_uart_tx_read_ptr) + s_uart_info->u16_uart_tx_write_ptr) % s_uart_info->u16_uart_tx_buffer_size);
	}
	else
		return 0;
}

/***************************** DMA RX GENERIC FUNCTIONS ************************/

/**
 *
 * 	UART use a circular DMA buffer for reception
 *
 * 	- UART_DMA_init() start DMA receiver
 * 	- each time a byte is received, data is stored into pc_usart_rx_buffer
 * 	- each time the DMA buffer loop, over the 256 bytes, HAL_UART_RxCpltCallback is called : this is use to add a virtual +256 byte to a 16 bits address.
 * 	- when caller get a byte from buffer, by calling UART_DMA_pop(),
 * 			16 bits buffers pointer are compared by UART_DMA_get_buffer_usage()
 * 			this permit to detect buffer overflow.
 *
 *
 */

void UART_DMA_RxCpltCallback(DMA_HandleTypeDef *hdma)
{
	uart_s* ps_uart = (uart_s*)(hdma->Parent);

	//! count number of full buffer rx DMA : this permit to create a virtual MSB of write pointer to detect overflow.
	ps_uart->u16_uart_rx_write_ptr++;

  if (ps_uart->bRTS_Enable)
  {
    // check if RTS must be set or clear
    if (UART_RX_DMA_get_buffer_usage(ps_uart) >= ps_uart->u16_uart_rx_usage_4_rts)
    {
#if (UART_RTS_DBG == 1)
      if ((ps_uart->GPIO_RTS_Port->ODR & (uint32_t)ps_uart->GPIO_RTS_Pin) == 0)
        LOG_WARNING("UART Buffer Almost full !");
#endif
      // set RTS to indicate that we are almost full
      ps_uart->GPIO_RTS_Port->BSRR = (uint32_t)ps_uart->GPIO_RTS_Pin;
    }
    else
    {
#if (UART_RTS_DBG == 1)
      if ((ps_uart->GPIO_RTS_Port->ODR & (uint32_t)ps_uart->GPIO_RTS_Pin) != 0)
        LOG_INFO("UART Buffer Freed");
#endif
      // clear RTS to indicate that we are ready to receive data
#if defined(STM32F7)
      ps_uart->GPIO_RTS_Port->BSRR = ((uint32_t)ps_uart->GPIO_RTS_Pin << 16);
#else
      ps_uart->GPIO_RTS_Port->BRR = (uint32_t)ps_uart->GPIO_RTS_Pin;
#endif
    }
  }
}

void UART_DMA_RxHalfCpltCallback(DMA_HandleTypeDef* hdma)
{
  uart_s* ps_uart = (uart_s*)(hdma->Parent);

  if (ps_uart->bRTS_Enable)
  {
    // check if RTS must be set or clear
    if (UART_RX_DMA_get_buffer_usage(ps_uart) >= ps_uart->u16_uart_rx_usage_4_rts)
    {
#if (UART_RTS_DBG == 1)
      if ((ps_uart->GPIO_RTS_Port->ODR & (uint32_t)ps_uart->GPIO_RTS_Pin) == 0)
        LOG_WARNING("UART Buffer Almost full !");
#endif
      // set RTS to indicate that we are almost full
      ps_uart->GPIO_RTS_Port->BSRR = (uint32_t)ps_uart->GPIO_RTS_Pin;
    }
    else
    {
#if (UART_RTS_DBG == 1)
      if ((ps_uart->GPIO_RTS_Port->ODR & (uint32_t)ps_uart->GPIO_RTS_Pin) != 0)
        LOG_INFO("UART Buffer Freed");
#endif
      // clear RTS to indicate that we are ready to receive data
#if defined(STM32F7)
      ps_uart->GPIO_RTS_Port->BSRR = ((uint32_t)ps_uart->GPIO_RTS_Pin << 16);
#else
      ps_uart->GPIO_RTS_Port->BRR = (uint32_t)ps_uart->GPIO_RTS_Pin;
#endif
    }
  }
}

void UART_DMA_RxErrorCallback(DMA_HandleTypeDef *hdma)
{
	uart_s* ps_uart = (uart_s*)(hdma->Parent);

	/* Abort DMA RX */
	HAL_DMA_Abort_IT(ps_uart->phuart->hdmarx);

	UART_RX_DMA_init(ps_uart); /* start new DMA reception */
}

//! initialize and start DMA reception
void UART_RX_DMA_init(struct __HandlerUart_s* s_uart_info)
{
	// if no buffer, exit function
	if (s_uart_info->pu8_uart_rx_buffer == NULL)
		return;

  /* Enable the DMA channel */
	// TODO is it really needed ???
	s_uart_info->phuart->pRxBuffPtr = (uint8_t*)s_uart_info->pu8_uart_rx_buffer;
	s_uart_info->phuart->RxXferSize = s_uart_info->u16_uart_rx_buffer_size;

  /* Disable the peripheral */
  __HAL_DMA_DISABLE(s_uart_info->hdmarx);

  /* Enable the UART Parity Error Interrupt */
  __HAL_UART_ENABLE_IT(s_uart_info->phuart, UART_IT_PE);

  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(s_uart_info->phuart, UART_IT_ERR);

#if defined(STM32F7)
  /* calculate DMA base and stream number */
  DMA_Base_Registers *regs = (DMA_Base_Registers *)s_uart_info->hdmarx->StreamBaseAddress;

  /* Clear DBM bit */
  s_uart_info->hdmarx->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);

  /* Configure DMA Channel data length */
  s_uart_info->hdmarx->Instance->NDTR = s_uart_info->u16_uart_rx_buffer_size;

  /* Configure DMA Channel destination address */
	s_uart_info->hdmarx->Instance->M0AR = (uint32_t)s_uart_info->pu8_uart_rx_buffer;

  /* Configure DMA Channel source address */
  s_uart_info->hdmarx->Instance->PAR = (uint32_t)&s_uart_info->phuart->Instance->RDR;

  /* Clear all interrupt flags at correct offset within the register */
  regs->IFCR = 0x3FU << s_uart_info->hdmarx->StreamIndex;
#else
  /* Configure DMA Channel data length */
  s_uart_info->hdmarx->Instance->CNDTR = s_uart_info->u16_uart_rx_buffer_size;

  /* Configure DMA Channel destination address */
  //s_uart_info->hdmarx->Instance->CMAR = *(uint32_t*)&s_uart_info->pu8_uart_rx_buffer;
  s_uart_info->hdmarx->Instance->CMAR = (uint32_t)s_uart_info->pu8_uart_rx_buffer;

  /* Configure DMA Channel source address */
  s_uart_info->hdmarx->Instance->CPAR = (uint32_t)&s_uart_info->phuart->Instance->RDR;
#endif
  /* Enable the transfer complete interrupt */
  __HAL_DMA_ENABLE_IT(s_uart_info->hdmarx, (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE));

  /* Initialize the error code */
  s_uart_info->hdmarx->ErrorCode = HAL_DMA_ERROR_NONE;
  s_uart_info->hdmarx->State = HAL_DMA_STATE_BUSY;

    /* Enable the Peripheral */
  __HAL_DMA_ENABLE(s_uart_info->hdmarx);

  /* Enable the DMA transfer for the receiver request by setting the DMAR bit
      in the UART CR3 register */
	s_uart_info->phuart->Instance->CR3 |= USART_CR3_DMAR;

	s_uart_info->hdmarx->XferCpltCallback = UART_DMA_RxCpltCallback;
  s_uart_info->hdmarx->XferHalfCpltCallback = UART_DMA_RxHalfCpltCallback;
	s_uart_info->hdmarx->XferErrorCallback = UART_DMA_RxErrorCallback;

  if (s_uart_info->bRTS_Enable)
  {
    // clear RTS to indicate that we are ready to receive data
#if defined(STM32F7)
    s_uart_info->GPIO_RTS_Port->BSRR = ((uint32_t)s_uart_info->GPIO_RTS_Port << 16);
#else
    s_uart_info->GPIO_RTS_Port->BRR = (uint32_t)s_uart_info->GPIO_RTS_Pin;
#endif
  }

	if (s_uart_info->oUartType == Uart_HalfDuplex)
	{
		// disable transmitter
		CLEAR_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_TE);
		// enable receiver
		SET_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_RE);
	}
#if (ENABLE_SOFT_RE_DE_4_RS485 != 0)
  if (s_uart_info->bREDE_Soft_Enable)
#if defined(STM32F7)
    s_uart_info->GPIO_REDE_Port->BSRR = ((uint32_t)s_uart_info->GPIO_REDE_Pin << 16);
#else
    s_uart_info->GPIO_REDE_Port->BRR = s_uart_info->GPIO_REDE_Pin;
#endif
#endif
}

//! stop DMA reception
uint8_t UART_RX_DMA_stop(uart_s* s_uart_info)
{
	/* Disable the UART Rx DMA requests */
	s_uart_info->phuart->Instance->CR3 &= ~USART_CR3_DMAR;

	/* Disable the channel */
	__HAL_DMA_DISABLE(s_uart_info->hdmarx);

	/* Get tick */
	uint32_t u32_tickstart = HAL_GetTick();

	/* Check if the DMA Channel is effectively disabled */
#if defined(STM32F7)
	while ((s_uart_info->hdmarx->Instance->CR & DMA_SxCR_EN) != 0)
#else
	while((s_uart_info->hdmarx->Instance->CCR & DMA_CCR_EN) != 0)
#endif
	{
		/* Check for the Timeout */
		if((HAL_GetTick() - u32_tickstart) > 1000) // timeout at 1s
			return 1;
	}

	return 0;
}

//! compute a 16 bits extended FIFO write pointer, by using count of full buffer fill (HAL_UART_RxCpltCallback)
volatile uint16_t _UART_RX_DMA_get_write_buffer(uart_s* s_uart_info)
{
	uint16_t u16_write_ptr_lsb;		// LSB
	uint16_t u16_write_ptr;		// MSB + LSB

	// position in buffer from DMA CNDTR reg : CNDTRx hold remaining data.
#if defined(STM32F7)
	u16_write_ptr_lsb = (uint32_t)s_uart_info->u16_uart_rx_buffer_size - ((s_uart_info->hdmarx->Instance->NDTR)&0xFFFF);
#else
	u16_write_ptr_lsb = (uint32_t)s_uart_info->u16_uart_rx_buffer_size - ((s_uart_info->hdmarx->Instance->CNDTR) & 0xFFFF);
#endif
	// load MSB, updated in interrupt : count of full XXX bytes of data buffer
	u16_write_ptr = 0;// TODO a verifier si c'est bon... ((uint16_t)s_uart_info->u16_uart_rx_write_ptr) * s_uart_info->u16_uart_rx_buffer_size;
	u16_write_ptr |= u16_write_ptr_lsb;						// computed 16 bits extended index.
	return u16_write_ptr;
}

//! because DMA register CNDTR is updated by hardware, and u8_write_ptr_msb is updated in interrupt, we need to read twice to assure this 2 data are coherent
uint16_t UART_RX_DMA_get_write_buffer(uart_s* s_uart_info)
{
	uint16_t u16_write_ptr1, u16_write_ptr2;
	u16_write_ptr1 = _UART_RX_DMA_get_write_buffer(s_uart_info);
	do
	{
		u16_write_ptr2 = u16_write_ptr1;
		u16_write_ptr1 = _UART_RX_DMA_get_write_buffer(s_uart_info);
	} while ( u16_write_ptr1-u16_write_ptr2 );
	return u16_write_ptr1;
}

//! pop a byte from the DMA buffer FIFO.
uint8_t UART_DMA_pop(struct __HandlerUart_s* s_uart_info, uint8_t* pu8_data)
{
  uint8_t u8_Ret = 0;
	uint16_t u16_buffer_usage;
	u16_buffer_usage = UART_RX_DMA_get_buffer_usage(s_uart_info);	// compute current usage of buffer

	// on overflow, return error and clear DMA FIFO buffer.
	if ( u16_buffer_usage >= s_uart_info->u16_uart_rx_buffer_size )
	{
		UART_RX_DMA_buffer_clear(s_uart_info);	// clear FIFO.
		return 2;	// return overflow
	}
	if ( u16_buffer_usage )	// one or more byte available, return it	// return a char if data available
	{
		*pu8_data = s_uart_info->pu8_uart_rx_buffer[s_uart_info->u16_uart_rx_read_ptr];
		s_uart_info->u16_uart_rx_read_ptr++;
		s_uart_info->u16_uart_rx_read_ptr %= s_uart_info->u16_uart_rx_buffer_size;
		u8_Ret = 1;
	}
  if (s_uart_info->bRTS_Enable)
  {
    // check if RTS must be set or clear
    if (UART_RX_DMA_get_buffer_usage(s_uart_info) >= s_uart_info->u16_uart_rx_usage_4_rts)
    {
#if (UART_RTS_DBG == 1)
      if ((s_uart_info->GPIO_RTS_Port->ODR & (uint32_t)s_uart_info->GPIO_RTS_Pin) == 0)
        LOG_WARNING("UART Buffer Almost full !");
#endif
      // set RTS to indicate that we are almost full
      s_uart_info->GPIO_RTS_Port->BSRR = (uint32_t)s_uart_info->GPIO_RTS_Pin;
    }
    else
    {
#if (UART_RTS_DBG == 1)
      if ((s_uart_info->GPIO_RTS_Port->ODR & (uint32_t)s_uart_info->GPIO_RTS_Pin) != 0)
        LOG_INFO("UART Buffer Freed");
#endif
      // clear RTS to indicate that we are ready to receive data
#if defined(STM32F7)
      s_uart_info->GPIO_RTS_Port->BSRR = ((uint32_t)s_uart_info->GPIO_RTS_Port << 16);
#else
      s_uart_info->GPIO_RTS_Port->BRR = (uint32_t)s_uart_info->GPIO_RTS_Pin;
#endif
    }
  }
  return u8_Ret;
}

//! compute current data stored into the circular DMA buffer.
uint16_t UART_RX_DMA_get_buffer_usage(struct __HandlerUart_s* s_uart_info)
{
	uint16_t u16_buffer_usage;		// 16 bits FIFO usage with overflow measure

	uint16_t u16_DmaWrPt = UART_RX_DMA_get_write_buffer(s_uart_info);
	if (s_uart_info->u16_uart_rx_read_ptr != u16_DmaWrPt)
		u16_buffer_usage = (((s_uart_info->u16_uart_rx_buffer_size - s_uart_info->u16_uart_rx_read_ptr) + u16_DmaWrPt) % s_uart_info->u16_uart_rx_buffer_size);
	else
		u16_buffer_usage = 0;

	return u16_buffer_usage;
}

//! clear the DMA buffer : move the read pointer to the write pointer position.
void UART_RX_DMA_buffer_clear(struct __HandlerUart_s* s_uart_info)
{
	// clear FIFO : move read pointer to the current write ptr position.
	s_uart_info->u16_uart_rx_read_ptr = UART_RX_DMA_get_write_buffer(s_uart_info);
  if (s_uart_info->bRTS_Enable)
  {
    // clear RTS to indicate that we are ready to receive data
#if defined(STM32F7)
    s_uart_info->GPIO_RTS_Port->BSRR = ((uint32_t)s_uart_info->GPIO_RTS_Port << 16);
#else
    s_uart_info->GPIO_RTS_Port->BRR = (uint32_t)s_uart_info->GPIO_RTS_Pin;
#endif
  }
}

/***************************** DMA TX GENERIC FUNCTIONS ************************/

/**
 *
 * 	UART use a normal DMA buffer for transmission
 *
 * 	- UART_DMA_put() start DMA transmit
 * 	- each time a some bytes must be send, data is stored into pc_usart_tx_buffer
 * 	- each time the DMA as no activity and buffer not tx not empty, the DMA is started to send data
 * 	- remaining bytes to send can be known by calling UART_TX_DMA_get_buffer_usage()
 *
 *
 */

 //! start a DMA transfer for the next block of data
void UART_TX_DMA_send_next_data_block(uart_s* s_uart_info)
{
	uint16_t u16_size_to_send;

	// if no buffer, exit function
	if (s_uart_info->pu8_uart_tx_buffer == NULL)
		return;

	s_uart_info->hdmatx->XferCpltCallback = UART_DMA_TxCpltCallback;

	// compute data length
	if (s_uart_info->u16_uart_tx_write_ptr > s_uart_info->u16_uart_tx_read_ptr)
		u16_size_to_send = s_uart_info->u16_uart_tx_write_ptr - s_uart_info->u16_uart_tx_read_ptr;
	else if (s_uart_info->u16_uart_tx_write_ptr < s_uart_info->u16_uart_tx_read_ptr)
		u16_size_to_send = s_uart_info->u16_uart_tx_buffer_size - s_uart_info->u16_uart_tx_read_ptr;
	else // write and read pointer are same
		return;

	/* Disable the peripheral */
	__HAL_DMA_DISABLE(s_uart_info->hdmatx);
  CLEAR_BIT(s_uart_info->phuart->Instance->CR3, USART_CR3_DMAT);

#if defined(STM32G0)
  /* Clear the DMAMUX synchro overrun flag */
	s_uart_info->hdmatx->DMAmuxChannelStatus->CFR = s_uart_info->hdmatx->DMAmuxChannelStatusMask;

  if (s_uart_info->hdmatx->DMAmuxRequestGen != 0U)
  {
    /* Clear the DMAMUX request generator overrun flag */
		s_uart_info->hdmatx->DMAmuxRequestGenStatus->RGCFR = s_uart_info->hdmatx->DMAmuxRequestGenStatusMask;
  }
#endif

  /* Clear all flags */
#if defined(STM32G0) && !defined(DMA2)
  __HAL_DMA_CLEAR_FLAG(s_uart_info->hdmatx, (DMA_FLAG_GI1 << (s_uart_info->hdmatx->ChannelIndex & 0x1CU)));
#elif defined(STM32F7)
	CLEAR_BIT(s_uart_info->hdmatx->Instance->CR, DMA_SxCR_DBM);
#else
  s_uart_info->hdmatx->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << (s_uart_info->hdmatx->ChannelIndex & 0x1cU));
#endif

#if defined(STM32F7)
  /* calculate DMA base and stream number */
  DMA_Base_Registers *regs = (DMA_Base_Registers *)s_uart_info->hdmatx->StreamBaseAddress;

	/* Configure DMA Channel data length */
	s_uart_info->hdmatx->Instance->NDTR = u16_size_to_send;

	/* Configure DMA Channel destination address */
	s_uart_info->hdmatx->Instance->PAR = (uint32_t)&s_uart_info->phuart->Instance->TDR;

	/* Configure DMA Channel source address */
	s_uart_info->hdmatx->Instance->M0AR = (uint32_t)(&s_uart_info->pu8_uart_tx_buffer[s_uart_info->u16_uart_tx_read_ptr]);

  /* Clear all interrupt flags at correct offset within the register */
  regs->IFCR = 0x3FU << s_uart_info->hdmatx->StreamIndex;
#else
  /* Configure DMA Channel data length */
  s_uart_info->hdmatx->Instance->CNDTR = u16_size_to_send;

  /* Configure DMA Channel destination address */
  s_uart_info->hdmatx->Instance->CPAR = (uint32_t)&s_uart_info->phuart->Instance->TDR;

  /* Configure DMA Channel source address */
  //s_uart_info->hdmatx->Instance->CMAR = *(uint32_t*)&s_uart_info->pu8_uart_tx_buffer + s_uart_info->u16_uart_tx_read_ptr;
  s_uart_info->hdmatx->Instance->CMAR = (uint32_t)(&s_uart_info->pu8_uart_tx_buffer[s_uart_info->u16_uart_tx_read_ptr]);
#endif

	// increase read pointer
	s_uart_info->u16_uart_tx_read_ptr += u16_size_to_send;
	s_uart_info->u16_uart_tx_read_ptr %= s_uart_info->u16_uart_tx_buffer_size;

	/* Enable the transfer complete interrupt */
	/* TODO Enable the Half transfer complete interrupt and manage it */
	/* TODO Enable the transfer Error interrupt and manage it */
	__HAL_DMA_ENABLE_IT(s_uart_info->hdmatx, (DMA_IT_TC /*| DMA_IT_HT | DMA_IT_TE*/));
  __HAL_DMA_DISABLE_IT(s_uart_info->hdmatx, (DMA_IT_HT | DMA_IT_TE));

#if defined(STM32G0)
  /* Check if DMAMUX Synchronization is enabled*/
  if ((s_uart_info->hdmatx->DMAmuxChannel->CCR & DMAMUX_CxCR_SE) != 0U)
  {
    /* Enable DMAMUX sync overrun IT*/
		s_uart_info->hdmatx->DMAmuxChannel->CCR |= DMAMUX_CxCR_SOIE;
  }

  if (s_uart_info->hdmatx->DMAmuxRequestGen != 0U)
  {
    /* if using DMAMUX request generator, enable the DMAMUX request generator overrun IT*/
    /* enable the request gen overrun IT*/
		s_uart_info->hdmatx->DMAmuxRequestGen->RGCR |= DMAMUX_RGxCR_OIE;
  }
#endif

  /* Initialize the error code */
  s_uart_info->hdmatx->ErrorCode = HAL_DMA_ERROR_NONE;
  s_uart_info->hdmatx->State = HAL_DMA_STATE_BUSY;

	/* Enable the Peripheral */
	__HAL_DMA_ENABLE(s_uart_info->hdmatx);

	/* Clear the TC flag in the ICR register */
	__HAL_UART_CLEAR_FLAG(s_uart_info->phuart, UART_CLEAR_TCF);
  /* Disable the UART Transmit Complete Interrupt */
  __HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_TC);

  /* Enable the DMA transfer for transmit request by setting the DMAT bit
  in the UART CR3 register */
  SET_BIT(s_uart_info->phuart->Instance->CR3, USART_CR3_DMAT);
}

void UART_DMA_TxCpltCallback(DMA_HandleTypeDef *hdma)
{
	uart_s* ps_uart = (uart_s*)(hdma->Parent);

	//// if a new DMA transfer must be started
	//if (ps_uart->u16_uart_tx_write_ptr != ps_uart->u16_uart_tx_read_ptr)
	//	UART_TX_DMA_send_next_data_block(ps_uart);

  /* Disable the DMA transfer for transmit request by resetting the DMAT bit
    in the UART CR3 register */
  CLEAR_BIT(ps_uart->phuart->Instance->CR3, USART_CR3_DMAT);
  /* Enable the UART Transmit Complete Interrupt */
  SET_BIT(ps_uart->phuart->Instance->CR1, USART_CR1_TCIE);
}

//! pop a byte from the DMA buffer FIFO.
uint8_t UART_DMA_put(struct __HandlerUart_s* s_uart_info, uint8_t* pData, uint16_t u16_size )
{
	uint32_t u32_dma_read_ptr;
	uint32_t u32_free_buffer_memory;
	uint32_t u32_tickstart;
	uint32_t u32_loop;

	// if no buffer, exit function
	if (s_uart_info->pu8_uart_tx_buffer == NULL)
		return 1;

#if (ENABLE_SOFT_RE_DE_4_RS485 != 0)
  if (s_uart_info->bREDE_Soft_Enable)
    s_uart_info->GPIO_REDE_Port->BSRR = s_uart_info->GPIO_REDE_Pin;
#endif

	u32_tickstart = HAL_GetTick();

	do{
    // compute the actual read pointer of DMA
#if defined(STM32F7)
		if (s_uart_info->u16_uart_tx_read_ptr > s_uart_info->hdmatx->Instance->NDTR)
			u32_dma_read_ptr = s_uart_info->u16_uart_tx_read_ptr - s_uart_info->hdmatx->Instance->NDTR;
		else
			u32_dma_read_ptr = s_uart_info->u16_uart_tx_buffer_size - s_uart_info->hdmatx->Instance->NDTR;
#else
    if (s_uart_info->u16_uart_tx_read_ptr > s_uart_info->hdmatx->Instance->CNDTR)
      u32_dma_read_ptr = s_uart_info->u16_uart_tx_read_ptr - s_uart_info->hdmatx->Instance->CNDTR;
    else
      u32_dma_read_ptr = s_uart_info->u16_uart_tx_buffer_size - s_uart_info->hdmatx->Instance->CNDTR;
#endif

		// compute free memory in buffer
		if (u32_dma_read_ptr > s_uart_info->u16_uart_tx_write_ptr)
			u32_free_buffer_memory = u32_dma_read_ptr - s_uart_info->u16_uart_tx_write_ptr;
		else
			u32_free_buffer_memory = s_uart_info->u16_uart_tx_buffer_size -
																s_uart_info->u16_uart_tx_write_ptr +
																u32_dma_read_ptr;

		// to avoid max size
		u32_free_buffer_memory--;

		// check if enough space in buffer to copy data
		if (u16_size > u32_free_buffer_memory)
		{
			// if no DMA transfer, start it, else wait until IT of TransferComplete to start the new one
#if defined(STM32F7)
			if (s_uart_info->hdmatx->Instance->NDTR == 0)
#else
			if (s_uart_info->hdmatx->Instance->CNDTR == 0)
#endif
			{
				UART_TX_DMA_send_next_data_block(s_uart_info);
			}

			if ((SCB->ICSR & 0x0000003F) == 0)
      {	// not from ISR => wait some time...
#if defined(STM32F7)
				if ((s_uart_info->hdmatx->Instance->NDTR == 0) ||
#else
				if ((s_uart_info->hdmatx->Instance->CNDTR == 0) ||
#endif
					(u16_size > s_uart_info->u16_uart_tx_buffer_size) ||
					((HAL_GetTick() - u32_tickstart) > 1000))
				{
					//LOG_ERROR( "UART : DMA Tx overflow");
					return 2;
				}
        // if DMA already transmitting something, wait end of previous transfer
#if defined(STM32F7)
				while (s_uart_info->hdmatx->Instance->NDTR != 0)
#else
				while (s_uart_info->hdmatx->Instance->CNDTR != 0)
#endif
					HAL_Delay(10);
			}
			else // from ISR => return immediatly
				return 2;
		}
	}while (u32_free_buffer_memory < u16_size); // until exit on error or enough space

	/* copy data to tx buffer */
	// if data can be put at the end of buffer
	if (u16_size < (s_uart_info->u16_uart_tx_buffer_size - s_uart_info->u16_uart_tx_write_ptr))
	{
		//memcpy(&s_uart_info->pu8_uart_tx_buffer[s_uart_info->u16_uart_tx_write_ptr], pcData, u16_size);
		for (u32_loop=0;u32_loop<u16_size;u32_loop++)
			s_uart_info->pu8_uart_tx_buffer[s_uart_info->u16_uart_tx_write_ptr+u32_loop] = *(pData+u32_loop);
	}
	else // put first part of data at the end of buffer and the last part at the top of buffer
	{
		uint16_t u16_size_first_part = s_uart_info->u16_uart_tx_buffer_size - s_uart_info->u16_uart_tx_write_ptr;
		// copy first part
		//memcpy(&s_uart_info->pu8_uart_tx_buffer[s_uart_info->u16_uart_tx_write_ptr], &pcData[0], u16_size_first_part);
		for (u32_loop=0;u32_loop<u16_size_first_part;u32_loop++)
			s_uart_info->pu8_uart_tx_buffer[s_uart_info->u16_uart_tx_write_ptr+u32_loop] = *(pData+u32_loop);
		// copy last part
		//memcpy(&s_uart_info->pu8_uart_tx_buffer[0], &pcData[u16_size_first_part], (u16_size-u16_size_first_part));
		for (u32_loop=0;u32_loop<(u16_size-u16_size_first_part);u32_loop++)
			s_uart_info->pu8_uart_tx_buffer[u32_loop] = *(pData+u16_size_first_part+u32_loop);
	}
	// compute new write pointer
	s_uart_info->u16_uart_tx_write_ptr += u16_size;
	s_uart_info->u16_uart_tx_write_ptr %= s_uart_info->u16_uart_tx_buffer_size;

	// if no DMA transfer, start it, else wait until IT of TransferComplete to start the new one
#if defined(STM32F7)
	if (s_uart_info->hdmatx->Instance->NDTR == 0)
#else
	if(s_uart_info->hdmatx->Instance->CNDTR == 0)
#endif
	{
		UART_TX_DMA_send_next_data_block(s_uart_info);
	}

	return 0;
}

uint8_t HalfDuplex_UART_DMA_put(struct __HandlerUart_s* s_uart_info, uint8_t* pData, uint16_t u16_size )
{
#if defined(STM32F7)
  if (s_uart_info->hdmatx->Instance->NDTR == 0)
#else
  if (s_uart_info->hdmatx->Instance->CNDTR == 0)
#endif
	{
		// disable Receiver (for no echo receive)
		__HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_PE);
		__HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_ERR);
		__HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_RXNE);
		__HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_TC);
		__HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_TXE);
		CLEAR_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_RE);
		// enable transmitter
		SET_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_TE);
	}

  return UART_DMA_put(s_uart_info, pData, u16_size );
}

//! stop DMA transmission
uint8_t UART_TX_DMA_stop(uart_s* s_uart_info)
{
	/* Disable the UART Tx DMA requests */
	s_uart_info->phuart->Instance->CR3 &= ~USART_CR3_DMAT;

	/* Disable the channel */
	__HAL_DMA_DISABLE(s_uart_info->hdmatx);

	/* Get tick */
	uint32_t u32_tickstart = HAL_GetTick();

	/* Check if the DMA Channel is effectively disabled */
#if defined(STM32F7)
	while((s_uart_info->hdmatx->Instance->CR & DMA_SxCR_EN) != 0)
#else
	while ((s_uart_info->hdmatx->Instance->CCR & DMA_CCR_EN) != 0)
#endif
	{
		/* Check for the Timeout */
		if((HAL_GetTick() - u32_tickstart) > 1000) // timeout at 1s
			return 1;
	}

	return 0;
}

//! compute remaining data to send by the DMA buffer.
uint16_t UART_TX_DMA_get_buffer_usage(struct __HandlerUart_s* s_uart_info)
{
	uint16_t u16_buffer_usage;
	// TODO a verifier si le calcul est bon....
	if ( s_uart_info->u16_uart_tx_write_ptr != s_uart_info->u16_uart_tx_read_ptr )
		//u16_buffer_usage = (s_uart_info->u16_uart_tx_buffer_size +
		//					(s_uart_info->u16_uart_tx_read_ptr - s_uart_info->hdmatx->Instance->CNDTR) -
		//					s_uart_info->u16_uart_tx_write_ptr)%s_uart_info->u16_uart_tx_buffer_size;
#if defined(STM32F7)
		u16_buffer_usage = ((s_uart_info->u16_uart_tx_buffer_size -
											(s_uart_info->u16_uart_tx_read_ptr - s_uart_info->hdmatx->Instance->NDTR)) +
											s_uart_info->u16_uart_tx_write_ptr) % s_uart_info->u16_uart_tx_buffer_size;
#else
		u16_buffer_usage = ((s_uart_info->u16_uart_tx_buffer_size -
											(s_uart_info->u16_uart_tx_read_ptr - s_uart_info->hdmatx->Instance->CNDTR)) +
											s_uart_info->u16_uart_tx_write_ptr) % s_uart_info->u16_uart_tx_buffer_size;
#endif
	else
		u16_buffer_usage = 0;

	return u16_buffer_usage;
}

/***************************** IT GENERIC CALLBACK FUNCTIONS ************************/

//! local handler for IT/DMA transfer rx and/or tx of UART data
void UART_IrqHandler(uart_s* s_uart_info)
{
	uint32_t isrflags;
	uint32_t cr1its;
	uint32_t cr3its;
	uint32_t errorflags = 0x00U;
	uint32_t ErrorCode = 0;

	isrflags = READ_REG(s_uart_info->phuart->Instance->ISR);
	cr1its = READ_REG(s_uart_info->phuart->Instance->CR1);
	cr3its = READ_REG(s_uart_info->phuart->Instance->CR3);

	/* If no error occurs */
	errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE));
	if (errorflags == RESET)
	{
		/* UART in mode Receiver -------------------------------------------------*/
#if defined(STM32G0)
		if (((isrflags & USART_ISR_RXNE_RXFNE) != RESET) && ((cr1its & USART_CR1_RXNEIE_RXFNEIE) != RESET))
#else
		if (((isrflags & USART_ISR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
#endif
		{
			UART_RX_put_fifo(s_uart_info);
			/* Clear RXNE interrupt flag */
			__HAL_UART_CLEAR_FLAG(s_uart_info->phuart, UART_FLAG_RXNE);
		}
	}

	/* If some errors occur */
#if defined(STM32G0)
	if ((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE_RXFNEIE | USART_CR1_PEIE)) != RESET)))
#else
	if ((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))
#endif
	{
		/* UART parity error interrupt occurred ----------------------------------*/
		if (((isrflags & USART_ISR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))
		{
			ErrorCode |= HAL_UART_ERROR_PE;
			__HAL_UART_CLEAR_FLAG(s_uart_info->phuart, UART_FLAG_PE);
		}

		/* UART frame error interrupt occurred -----------------------------------*/
		if (((isrflags & USART_ISR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
		{
			ErrorCode |= HAL_UART_ERROR_FE;
			__HAL_UART_CLEAR_FLAG(s_uart_info->phuart, UART_FLAG_FE);
		}

		/* UART noise error interrupt occurred -----------------------------------*/
		if (((isrflags & USART_ISR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
		{
			ErrorCode |= HAL_UART_ERROR_NE;
			__HAL_UART_CLEAR_FLAG(s_uart_info->phuart, UART_FLAG_NE);
		}

		/* UART Over-Run interrupt occurred --------------------------------------*/
		if (((isrflags & USART_ISR_ORE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
		{
			ErrorCode |= HAL_UART_ERROR_ORE;
			__HAL_UART_CLEAR_FLAG(s_uart_info->phuart, UART_FLAG_ORE);
		}

		/* Call UART Error Call back function if need be --------------------------*/
		if (ErrorCode != HAL_UART_ERROR_NONE)
		{
			/* UART in mode Receiver -----------------------------------------------*/
#if defined(STM32G0)
			if (((isrflags & USART_ISR_RXNE_RXFNE) != RESET) && ((cr1its & USART_CR1_RXNEIE_RXFNEIE) != RESET))
#else
			if (((isrflags & USART_ISR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
#endif
			{
				UART_RX_put_fifo(s_uart_info);
				/* Clear RXNE interrupt flag */
				__HAL_UART_CLEAR_FLAG(s_uart_info->phuart, UART_FLAG_RXNE);
			}

			/* If Overrun error occurs, or if any error occurs in DMA mode reception,
			consider error as blocking */
			if (((ErrorCode & HAL_UART_ERROR_ORE) != RESET) ||
					(HAL_IS_BIT_SET(s_uart_info->phuart->Instance->CR3, USART_CR3_DMAR)))
			{
				/* Blocking error : transfer is aborted
				Set the UART state ready to be able to start again the process,
				Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
				/* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
#if defined(STM32G0)
				CLEAR_BIT(s_uart_info->phuart->Instance->CR1, (USART_CR1_RXNEIE_RXFNEIE | USART_CR1_PEIE));
#else
				CLEAR_BIT(s_uart_info->phuart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
#endif
				CLEAR_BIT(s_uart_info->phuart->Instance->CR3, USART_CR3_EIE);

				/* At end of Rx process, restore huart->RxState to Ready */
				s_uart_info->phuart->RxState = HAL_UART_STATE_READY;

				/* Disable the UART DMA Rx request if enabled */
				if (HAL_IS_BIT_SET(s_uart_info->phuart->Instance->CR3, USART_CR3_DMAR))
				{
					CLEAR_BIT(s_uart_info->phuart->Instance->CR3, USART_CR3_DMAR);

					/* Abort the UART DMA Rx channel */
					if (s_uart_info->phuart->hdmarx != NULL)
					{
						/* Abort DMA RX */
						HAL_DMA_Abort_IT(s_uart_info->phuart->hdmarx);

						UART_RX_DMA_init(s_uart_info); /* start new DMA reception */
					}
				}
				else
				{
					UART_RX_IT_init(s_uart_info); /* start new interrupt reception */
				}
			}
			else
			{
				/* Non Blocking error : transfer could go on.  */
			}
		}
	}

	/* UART in mode Transmitter ------------------------------------------------*/
#if defined(STM32G0)
	if (((isrflags & USART_ISR_TXE_TXFNF) != RESET) && ((cr1its & USART_CR1_TXEIE_TXFNFIE) != RESET))
#else
	if (((isrflags & USART_ISR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
#endif
	{
#if (MODBUS_ENABLE != 0)
    if ((s_uart_info == &ModBusUartHandler) && (bModBusInitialized != false) && (bRunTest == false))
    {
			if (pxMBFrameCBTransmitterEmpty != NULL)
				pxMBFrameCBTransmitterEmpty();
    }
		else
#endif
		{
			// if a new IT data must be transfer
			if (s_uart_info->u16_uart_tx_write_ptr != s_uart_info->u16_uart_tx_read_ptr)
				UART_TX_IT_send_next_data(s_uart_info);
			else
			{
				/* Disable the UART Transmit Data Register Empty Interrupt */
				__HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_TXE);
				/* Enable the UART Transmit Complete Interrupt */
				__HAL_UART_ENABLE_IT(s_uart_info->phuart, UART_IT_TC);
			}
		}
	}

	/* UART in mode Transmitter (transmission end) -----------------------------*/
	if (((isrflags & USART_ISR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
	{
		/* Disable the UART Transmit Complete Interrupt */
		__HAL_UART_DISABLE_IT(s_uart_info->phuart, UART_IT_TC);

		// if a new IT data must be transfer
    if (s_uart_info->u16_uart_tx_write_ptr != s_uart_info->u16_uart_tx_read_ptr)
    {
      if (s_uart_info->phuart->hdmatx != NULL)
        UART_TX_DMA_send_next_data_block(s_uart_info);
      else
        UART_TX_IT_send_next_data(s_uart_info);
    }
		else
		{
			s_uart_info->phuart->gState = HAL_UART_STATE_READY;

			if (s_uart_info->oUartType == Uart_HalfDuplex)
			{
				// disable transmitter
				CLEAR_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_TE);
				// enable receiver
				SET_BIT(s_uart_info->phuart->Instance->CR1, USART_CR1_RE);
				s_uart_info->RX_init(s_uart_info);
				s_uart_info->RX_buffer_clear(s_uart_info);
			}
#if (ENABLE_SOFT_RE_DE_4_RS485 != 0)
      if (s_uart_info->bREDE_Soft_Enable)
#if defined(STM32F7)
        s_uart_info->GPIO_REDE_Port->BSRR = ((uint32_t)s_uart_info->GPIO_REDE_Pin << 16);
#else
        s_uart_info->GPIO_REDE_Port->BRR = s_uart_info->GPIO_REDE_Pin;
#endif
#endif
		}
	}
}

void UART_SetBaudrate(uart_s* s_uart_info, uint32_t u32_BaudRate)
{
	s_uart_info->phuart->Init.BaudRate = u32_BaudRate;

/* USER CODE BEGIN SetBaudRate LP Uart */
  if (UART_INSTANCE_LOWPOWER(s_uart_info->phuart))
    HAL_UART_DeInit(s_uart_info->phuart);
/* USER CODE END SetBaudRate LP Uart */

	if (s_uart_info->oUartType == Uart_HalfDuplex)
	{
		if (HAL_HalfDuplex_Init(s_uart_info->phuart) != HAL_OK)
		{
			Error_Handler();
		}
	}
	else if (s_uart_info->oUartType == Uart_485)
	{
	    if (HAL_RS485Ex_Init(s_uart_info->phuart, s_uart_info->u32_485_Polarity, s_uart_info->u8_485_AssertTime, s_uart_info->u8_485_DeassertTime) != HAL_OK)
		{
			Error_Handler();
		}
	}
	else
	{
		if (HAL_UART_Init(s_uart_info->phuart) != HAL_OK)
		{
			Error_Handler();
		}
	}

/* USER CODE BEGIN SetBaudRate */
	if (s_uart_info->RX_get == UART_DMA_pop)
	{
		// DMA RX
		//__HAL_LINKDMA(&s_uart_info, hdmarx, hdma_usartx_rx);
		// TODO Mettre les handlers DMA si besoin
	}
	if (s_uart_info->TX_put == UART_DMA_put)
	{
		// DMA TX
		//__HAL_LINKDMA(&s_uart_info, hdmatx, hdma_usartx_tx);
		// TODO Mettre les handlers DMA si besoin
	}
/* USER CODE END SetBaudRate */

  if (s_uart_info->RX_buffer_clear != NULL)
    s_uart_info->RX_buffer_clear(s_uart_info);

  if (s_uart_info->RX_init != NULL)
    s_uart_info->RX_init(s_uart_info);
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

