/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = TXDebug_Pin|RXDebug_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, TXDebug_Pin|RXDebug_Pin);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void usart_init(void)
{
  uint32_t usartdiv = 0x00000000U;
  uint32_t pclk;

  /*-------------------------- USART1 clock source configuration -------------------*/
  /* Configure the USART1 clock source */
  __HAL_RCC_USART1_CONFIG(RCC_USART1CLKSOURCE_PCLK2);

  /* USART1 clock enable */
  __HAL_RCC_USART1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // GPIO A9 A10 (USART1)
  GPIOA->AFR[1] &= 0xFFFFF00F;
  GPIOA->AFR[1] |= 0x00000770;
  GPIOA->MODER &= 0xFFC3FFFF;
  GPIOA->MODER |= 0x00280000;
  GPIOA->OSPEEDR &= 0xFFC3FFFF;
  GPIOA->OTYPER &= 0xFFFFF9FF;
  GPIOA->PUPDR &= 0xFFC3FFFF;

  
  __HAL_UART_DISABLE(&huart1);

  /* Set the UART Communication parameters */
  /*-------------------------- USART CR1 Configuration -----------------------*/
  /* Clear M, PCE, PS, TE, RE and OVER8 bits and configure
  *  the UART Word Length, Parity, Mode and oversampling:
  *  set the M bits according to huart->Init.WordLength value
  *  set PCE and PS bits according to huart->Init.Parity value
  *  set TE and RE bits according to huart->Init.Mode value
  *  set OVER8 bit according to huart->Init.OverSampling value */
  MODIFY_REG(huart1.Instance->CR1, (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8 | 
      USART_CR1_FIFOEN), (UART_WORDLENGTH_8B | UART_PARITY_NONE | UART_MODE_TX_RX | UART_OVERSAMPLING_16));
  /*-------------------------- USART CR2 Configuration -----------------------*/
  /* Configure the UART Stop Bits: Set STOP[13:12] bits according
  * to huart->Init.StopBits value */
  MODIFY_REG(huart1.Instance->CR2, USART_CR2_STOP, UART_STOPBITS_1);
  /*-------------------------- USART CR3 Configuration -----------------------*/
  /* Configure
  * - UART HardWare Flow Control: set CTSE and RTSE bits according
  *   to huart->Init.HwFlowCtl value
  * - one-bit sampling method versus three samples' majority rule according
  *   to huart->Init.OneBitSampling (not applicable to LPUART) */
  MODIFY_REG(huart1.Instance->CR3, (USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT | 
    USART_CR3_TXFTCFG | USART_CR3_RXFTCFG), (UART_HWCONTROL_NONE | UART_ONE_BIT_SAMPLE_DISABLE));
  /*-------------------------- USART PRESC Configuration -----------------------*/
  /* Configure
  * - UART Clock Prescaler : set PRESCALER according to huart->Init.ClockPrescaler value */
  MODIFY_REG(huart1.Instance->PRESC, USART_PRESC_PRESCALER, UART_PRESCALER_DIV1);
  /*-------------------------- USART BRR Configuration -----------------------*/
  pclk = HAL_RCC_GetPCLK2Freq();
  usartdiv = (uint16_t)(UART_DIV_SAMPLING16(pclk, 115200, UART_PRESCALER_DIV1));
  huart1.Instance->BRR = usartdiv;

  /* Update TX threshold configuration */
  MODIFY_REG(huart1.Instance->CR3, USART_CR3_TXFTCFG, UART_TXFIFO_THRESHOLD_1_8);
  
  /* Update RX threshold configuration */
  MODIFY_REG(huart1.Instance->CR3, USART_CR3_RXFTCFG, UART_RXFIFO_THRESHOLD_1_8);
  
  /* Disable FIFO mode */
  CLEAR_BIT(huart1.Instance->CR1, USART_CR1_FIFOEN);
  
//  /* In asynchronous mode, the following bits must be kept cleared:
//  - LINEN and CLKEN bits in the USART_CR2 register,
//  - SCEN, HDSEL and IREN  bits in the USART_CR3 register.*/
//  CLEAR_BIT(huart1.Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
//  CLEAR_BIT(huart1.Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

  __HAL_UART_ENABLE(&huart1);

//  /* TEACK and/or REACK to check before moving huart->gState and huart->RxState to Ready */
//  UART_CheckIdleState(&huart1);
}

void _UART1_put(uint8_t u8_data)
{
  if (USART1->ISR & USART_ISR_ORE)		// overrun flag is set ?
    USART1->ICR |= USART_ICR_ORECF;	// clear !
  // if output shifter not empty
  while (~USART1->ISR & UART_FLAG_TXE);
  //		return 0;	// can't send byte now.

  // transmit the byte
  USART1->TDR = u8_data;
}

void _UART1_print(char* pc_data)
{
  while (*pc_data)
  {
    _UART1_put((uint8_t)*pc_data++);
  }
}

void _UART1_print_hex(uint8_t u8_data)
{
  uint8_t u8_val = (u8_data & 0xF0) >> 4;

  if (u8_val > 9)
    _UART1_put((u8_val - 0x0A) + 'A');
  else
    _UART1_put(u8_val + '0');

  u8_val = u8_data & 0x0F;
  if (u8_val > 9)
    _UART1_put((u8_val - 0x0A) + 'A');
  else
    _UART1_put(u8_val + '0');
}

void _UART1_print_hex32(uint32_t u32_data)
{
  uint8_t u8_data = (u32_data & 0xFF000000) >> 24;
  _UART1_print_hex(u8_data);
  u8_data = (u32_data & 0x00FF0000) >> 16;
  _UART1_print_hex(u8_data);
  u8_data = (u32_data & 0x0000FF00) >> 8;
  _UART1_print_hex(u8_data);
  u8_data = (u32_data & 0x000000FF);
  _UART1_print_hex(u8_data);
}

void _UART1_print_dec(uint8_t u8_data)
{
  if (u8_data > 99)
  {
    _UART1_put((u8_data / 100) + '0');
    u8_data %= 100;
  }
  if (u8_data > 9)
  {
    _UART1_put((u8_data / 10) + '0');
    u8_data %= 10;
  }
  _UART1_put(u8_data + '0');
}

/* USER CODE END 1 */
