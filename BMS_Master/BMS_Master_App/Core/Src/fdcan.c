/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    fdcan.c
  * @brief   This file provides code for the configuration
  *          of the FDCAN instances.
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
#include "fdcan.h"


/* USER CODE BEGIN 0 */
#include <string.h>
#include "Debug/debug.h"
#include "CAN_Retrofit.h"
#include "Can_BMS.h"



CanFrame_t aoLastCanMsg[MAX_NUM_CAN];

/* USER CODE END 0 */

FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;
FDCAN_HandleTypeDef hfdcan3;


/* FDCAN1 init function */
void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = DISABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 1;
  hfdcan1.Init.NominalSyncJumpWidth = 31;
  hfdcan1.Init.NominalTimeSeg1 = 32;
  hfdcan1.Init.NominalTimeSeg2 = 31;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 7;
  hfdcan1.Init.DataTimeSeg1 = 8;
  hfdcan1.Init.DataTimeSeg2 = 7;
  hfdcan1.Init.StdFiltersNbr = 2;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */

  /* USER CODE END FDCAN1_Init 2 */

}
/* FDCAN2 init function */
void MX_FDCAN2_Init(void)
{

  /* USER CODE BEGIN FDCAN2_Init 0 */

  /* USER CODE END FDCAN2_Init 0 */

  /* USER CODE BEGIN FDCAN2_Init 1 */

  /* USER CODE END FDCAN2_Init 1 */
  hfdcan2.Instance = FDCAN2;
  hfdcan2.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan2.Init.AutoRetransmission = DISABLE;
  hfdcan2.Init.TransmitPause = DISABLE;
  hfdcan2.Init.ProtocolException = DISABLE;
  hfdcan2.Init.NominalPrescaler = 1;
  hfdcan2.Init.NominalSyncJumpWidth = 7;
  hfdcan2.Init.NominalTimeSeg1 = 8;
  hfdcan2.Init.NominalTimeSeg2 = 7;
  hfdcan2.Init.DataPrescaler = 1;
  hfdcan2.Init.DataSyncJumpWidth = 7;
  hfdcan2.Init.DataTimeSeg1 = 8;
  hfdcan2.Init.DataTimeSeg2 = 7;
  hfdcan2.Init.StdFiltersNbr = 5;
  hfdcan2.Init.ExtFiltersNbr = 0;
  hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN2_Init 2 */

  /* USER CODE END FDCAN2_Init 2 */

}
/* FDCAN3 init function */
void MX_FDCAN3_Init(void)
{

  /* USER CODE BEGIN FDCAN3_Init 0 */

  /* USER CODE END FDCAN3_Init 0 */

  /* USER CODE BEGIN FDCAN3_Init 1 */

  /* USER CODE END FDCAN3_Init 1 */
  hfdcan3.Instance = FDCAN3;
  hfdcan3.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan3.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan3.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan3.Init.AutoRetransmission = DISABLE;
  hfdcan3.Init.TransmitPause = DISABLE;
  hfdcan3.Init.ProtocolException = DISABLE;
  hfdcan3.Init.NominalPrescaler = 1;
  hfdcan3.Init.NominalSyncJumpWidth = 7;
  hfdcan3.Init.NominalTimeSeg1 = 8;
  hfdcan3.Init.NominalTimeSeg2 = 7;
  hfdcan3.Init.DataPrescaler = 1;
  hfdcan3.Init.DataSyncJumpWidth = 7;
  hfdcan3.Init.DataTimeSeg1 = 8;
  hfdcan3.Init.DataTimeSeg2 = 7;
  hfdcan3.Init.StdFiltersNbr = 3;
  hfdcan3.Init.ExtFiltersNbr = 0;
  hfdcan3.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN3_Init 2 */

  /* USER CODE END FDCAN3_Init 2 */

}

static uint32_t HAL_RCC_FDCAN_CLK_ENABLED=0;

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspInit 0 */

  /* USER CODE END FDCAN1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_HSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN1 clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */
    GPIO_InitStruct.Pin = RXcan_BMS_Pin|TXcan_BMS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* FDCAN1 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
    HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
  /* USER CODE BEGIN FDCAN1_MspInit 1 */

  /* USER CODE END FDCAN1_MspInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN2)
  {
  /* USER CODE BEGIN FDCAN2_MspInit 0 */

  /* USER CODE END FDCAN2_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_HSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN2 clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**FDCAN2 GPIO Configuration
    PB5     ------> FDCAN2_RX
    PB6     ------> FDCAN2_TX
    */
    GPIO_InitStruct.Pin = RXcan_RTF_Pin|TXcan_RTF_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* FDCAN2 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
    HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
  /* USER CODE BEGIN FDCAN2_MspInit 1 */

  /* USER CODE END FDCAN2_MspInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN3)
  {
  /* USER CODE BEGIN FDCAN3_MspInit 0 */

  /* USER CODE END FDCAN3_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_HSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN3 clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**FDCAN3 GPIO Configuration
    PB3     ------> FDCAN3_RX
    PB4     ------> FDCAN3_TX
    */
    GPIO_InitStruct.Pin = RXcan_RTF2_Pin|TXcan_RTF2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF11_FDCAN3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* FDCAN3 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN3_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN3_IT0_IRQn);
    HAL_NVIC_SetPriority(FDCAN3_IT1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN3_IT1_IRQn);
  /* USER CODE BEGIN FDCAN3_MspInit 1 */

  /* USER CODE END FDCAN3_MspInit 1 */
  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspDeInit 0 */

  /* USER CODE END FDCAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_FDCAN_CLK_ENABLED--;
    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
      __HAL_RCC_FDCAN_CLK_DISABLE();
    }

    /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, RXcan_BMS_Pin|TXcan_BMS_Pin);

    /* FDCAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
    HAL_NVIC_DisableIRQ(FDCAN1_IT1_IRQn);
  /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

  /* USER CODE END FDCAN1_MspDeInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN2)
  {
  /* USER CODE BEGIN FDCAN2_MspDeInit 0 */

  /* USER CODE END FDCAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_FDCAN_CLK_ENABLED--;
    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
      __HAL_RCC_FDCAN_CLK_DISABLE();
    }

    /**FDCAN2 GPIO Configuration
    PB5     ------> FDCAN2_RX
    PB6     ------> FDCAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, RXcan_RTF_Pin|TXcan_RTF_Pin);

    /* FDCAN2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
    HAL_NVIC_DisableIRQ(FDCAN2_IT1_IRQn);
  /* USER CODE BEGIN FDCAN2_MspDeInit 1 */

  /* USER CODE END FDCAN2_MspDeInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN3)
  {
  /* USER CODE BEGIN FDCAN3_MspDeInit 0 */

  /* USER CODE END FDCAN3_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_FDCAN_CLK_ENABLED--;
    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
      __HAL_RCC_FDCAN_CLK_DISABLE();
    }

    /**FDCAN3 GPIO Configuration
    PB3     ------> FDCAN3_RX
    PB4     ------> FDCAN3_TX
    */
    HAL_GPIO_DeInit(GPIOB, RXcan_RTF2_Pin|TXcan_RTF2_Pin);

    /* FDCAN3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN3_IT0_IRQn);
    HAL_NVIC_DisableIRQ(FDCAN3_IT1_IRQn);
  /* USER CODE BEGIN FDCAN3_MspDeInit 1 */

  /* USER CODE END FDCAN3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void Start_Can(void)
{
  

  // TODO a faire pour tous les CAN, ceci n'est qu'un exemple
  
  // TODO modifier dans les fonctions MX_FDCANx_Init le nombre de filtre standard ou extended (modifiable egalement via CubeMX)
  
  // CAN 1
// 1 filtre active avec une plage de 0x000 a 0x7FF (toutes les trames en mode standard)
// le filtre appel l'IT 0
	FDCAN_FilterTypeDef sFilterConfig10;
	sFilterConfig10.IdType = FDCAN_STANDARD_ID;
	sFilterConfig10.FilterIndex = 0;
	sFilterConfig10.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig10.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig10.FilterID1 = 0x0300;
	sFilterConfig10.FilterID2 = 0x0700;
	
	FDCAN_FilterTypeDef sFilterConfig11;
	sFilterConfig11.IdType = FDCAN_STANDARD_ID;
	sFilterConfig11.FilterIndex = 0;
	sFilterConfig11.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig11.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig11.FilterID1 = 0x0400;
	sFilterConfig11.FilterID2 = 0x0700;
	
	
  
  if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig10) != HAL_OK)
  {
    LOG_ERROR("Error during CAN 1 Filtering configuration !");
  }
	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig11) != HAL_OK)
	{
		LOG_ERROR("Error during CAN 1 Filtering configuration !");
	}
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
  {
    LOG_ERROR("Error during CAN 1 Global Filtering configuration !");
  }
  if (HAL_FDCAN_ActivateNotification(&hfdcan1, 
    (FDCAN_IT_RX_HIGH_PRIORITY_MSG | FDCAN_IT_LIST_RX_FIFO0 | FDCAN_IT_LIST_RX_FIFO1 | 
      FDCAN_IT_TX_COMPLETE | FDCAN_IT_TX_ABORT_COMPLETE | FDCAN_IT_TX_FIFO_EMPTY),
    (FDCAN_TX_BUFFER0 | FDCAN_TX_BUFFER1 | FDCAN_TX_BUFFER2)) != HAL_OK)
  {
    LOG_ERROR("Error during CAN 1 Notification !");
  }
  /* Start the FDCAN module */
  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
  {
    LOG_ERROR("Error during CAN 1 Start !");
  }
  
  // CAN 2
  // 1 filtre active avec une plage de 0x000 a 0x7FF (toutes les trames en mode standard)
// le filtre appel l'IT 1
	FDCAN_FilterTypeDef sFilterConfig20;
	sFilterConfig20.IdType = FDCAN_STANDARD_ID;
	sFilterConfig20.FilterIndex = 0;
	sFilterConfig20.FilterType =  FDCAN_FILTER_MASK;
	sFilterConfig20.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
	sFilterConfig20.FilterID1 = 0x0726;
	sFilterConfig20.FilterID2 = 0x07FF;
	
  FDCAN_FilterTypeDef sFilterConfig21;
	sFilterConfig21.IdType = FDCAN_STANDARD_ID;
	sFilterConfig21.FilterIndex = 0;
	sFilterConfig21.FilterType =  FDCAN_FILTER_MASK;
	sFilterConfig21.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
	sFilterConfig21.FilterID1 = 0x0010;
	sFilterConfig21.FilterID2 = 0x07FF;
	
	FDCAN_FilterTypeDef sFilterConfig22;
	sFilterConfig22.IdType = FDCAN_STANDARD_ID;
	sFilterConfig22.FilterIndex = 0;
	sFilterConfig22.FilterType =  FDCAN_FILTER_MASK;
	sFilterConfig22.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
	sFilterConfig22.FilterID1 = 0x02A6;
	sFilterConfig22.FilterID2 = 0x04FF;
	
	FDCAN_FilterTypeDef sFilterConfig23;
	sFilterConfig23.IdType = FDCAN_STANDARD_ID;
	sFilterConfig23.FilterIndex = 0;
	sFilterConfig23.FilterType =  FDCAN_FILTER_MASK;
	sFilterConfig23.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
	sFilterConfig23.FilterID1 = 0x0704;
	sFilterConfig23.FilterID2 = 0x07FF;
	
	FDCAN_FilterTypeDef sFilterConfig24;
	sFilterConfig24.IdType = FDCAN_STANDARD_ID;
	sFilterConfig24.FilterIndex = 0;
	sFilterConfig24.FilterType =  FDCAN_FILTER_MASK;
	sFilterConfig24.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
	sFilterConfig24.FilterID1 = 0x0705;
	sFilterConfig24.FilterID2 = 0x07FF;
		
  
  if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig20) != HAL_OK)
  {
    LOG_ERROR("Error during CAN 2 Filtering configuration !");
  }
	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig21) != HAL_OK)
	{
		LOG_ERROR("Error during CAN 1 Filtering configuration !");
	}
	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig22) != HAL_OK)
	{
		LOG_ERROR("Error during CAN 1 Filtering configuration !");
	}
	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig23) != HAL_OK)
	{
		LOG_ERROR("Error during CAN 1 Filtering configuration !");
	}
	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig24) != HAL_OK)
	{
		LOG_ERROR("Error during CAN 1 Filtering configuration !");
	}
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan2, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
  {
    LOG_ERROR("Error during CAN 2 Global Filtering configuration !");
  }
  if (HAL_FDCAN_ActivateNotification(&hfdcan2, 
    (FDCAN_IT_RX_HIGH_PRIORITY_MSG | FDCAN_IT_LIST_RX_FIFO0 | FDCAN_IT_LIST_RX_FIFO1 | 
      FDCAN_IT_TX_COMPLETE | FDCAN_IT_TX_ABORT_COMPLETE | FDCAN_IT_TX_FIFO_EMPTY),
    (FDCAN_TX_BUFFER0 | FDCAN_TX_BUFFER1 | FDCAN_TX_BUFFER2)) != HAL_OK)
  {
    LOG_ERROR("Error during CAN 2 Notification !");
  }
  /* Start the FDCAN module */
  if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
  {
    LOG_ERROR("Error during CAN 2 Start !");
  }
  
  // CAN 3
  // 1 filtre active avec une plage de 0x100 a 0x2FF
// le filtre appel l'IT 0
	FDCAN_FilterTypeDef sFilterConfig30;
	sFilterConfig30.IdType = FDCAN_STANDARD_ID;
    sFilterConfig30.FilterIndex = 0;
	sFilterConfig30.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig30.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
	sFilterConfig30.FilterID1 = 0x620;
	sFilterConfig30.FilterID2 = 0x7FF;
	
	
	 FDCAN_FilterTypeDef sFilterConfig31;
	sFilterConfig31.IdType = FDCAN_STANDARD_ID;
	sFilterConfig31.FilterIndex = 0;
	sFilterConfig31.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig31.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
	sFilterConfig31.FilterID1 = 0x670;
	sFilterConfig31.FilterID2 = 0x7FF;
	
	FDCAN_FilterTypeDef sFilterConfig32;
	sFilterConfig32.IdType = FDCAN_STANDARD_ID;
	sFilterConfig32.FilterIndex = 0;
	sFilterConfig32.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig32.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
	sFilterConfig32.FilterID1 = 0x610;
	sFilterConfig32.FilterID2 = 0x7FF;
	
	
	
  if (HAL_FDCAN_ConfigFilter(&hfdcan3, &sFilterConfig30) != HAL_OK)
  {
    LOG_ERROR("Error during CAN 3 Filtering configuration !");
  }
	if (HAL_FDCAN_ConfigFilter(&hfdcan3, &sFilterConfig31) != HAL_OK)
	{
		LOG_ERROR("Error during CAN 3 Filtering configuration !");
	}
	if (HAL_FDCAN_ConfigFilter(&hfdcan3, &sFilterConfig32) != HAL_OK)
	{
		LOG_ERROR("Error during CAN 3 Filtering configuration !");
	}
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan3, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
  {
    LOG_ERROR("Error during CAN 3 Global Filtering configuration !");
  }
  if (HAL_FDCAN_ActivateNotification(&hfdcan3, 
    (FDCAN_IT_RX_HIGH_PRIORITY_MSG | FDCAN_IT_LIST_RX_FIFO0 | FDCAN_IT_LIST_RX_FIFO1 | 
      FDCAN_IT_TX_COMPLETE | FDCAN_IT_TX_ABORT_COMPLETE | FDCAN_IT_TX_FIFO_EMPTY),
    (FDCAN_TX_BUFFER0 | FDCAN_TX_BUFFER1 | FDCAN_TX_BUFFER2)) != HAL_OK)
  {
    LOG_ERROR("Error during CAN 3 Notification !");
  }
  /* Start the FDCAN module */
  if (HAL_FDCAN_Start(&hfdcan3) != HAL_OK)
  {
    LOG_ERROR("Error during CAN 3 Start !");
  }
}

// IT de reception CAN

void TreatCanMsg(uint8_t u8_RxFifo, uint8_t u8CanNum, FDCAN_RxHeaderTypeDef* pRxHeader, uint8_t* pau8RxData)
{
  if (u8CanNum >= MAX_NUM_CAN)
    return;
  
  // copie les donnees recu dans le buffer temporaire
     aoLastCanMsg[u8CanNum].u1_Valid = 1;
     aoLastCanMsg[u8CanNum].u29_ID = (pRxHeader->Identifier & 0x1FFFFFFF);
     aoLastCanMsg[u8CanNum].u8_DataLen = (pRxHeader->DataLength >> 16);
     memcpy(aoLastCanMsg[u8CanNum].au8_Data, pau8RxData, aoLastCanMsg[u8CanNum].u8_DataLen);
	
	if (u8CanNum==0) {
	 gotBMSFrameVoltage(u8CanNum);
	 gotBMSFrameTemperature(u8CanNum);
	}
	
	else if  (u8CanNum == 1) {	
	gotInverterFrameStatus(u8CanNum);
	gotFrameInverter(u8CanNum);
	gotMCUStatusFrame(u8CanNum);
	gotWheelSpeedFrame(u8CanNum);
	gotMCUFrameStatus(u8CanNum);
	}
	
	else if(u8CanNum == 2) {
	gotFrameDCDC(u8CanNum);
	gotChargerFrame(u8CanNum);
	gotChargerState(u8CanNum);
	}
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs)
{
  FDCAN_RxHeaderTypeDef RxHeader;
  uint8_t RxData[8];
  uint8_t u8CanNum = 0;
  if (hfdcan->Instance == FDCAN1) u8CanNum = 0;
  else if (hfdcan->Instance == FDCAN2) u8CanNum = 1;
  else if (hfdcan->Instance == FDCAN3) u8CanNum = 2;
  else return;

  if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {
    /* Retrieve Rx messages from RX FIFO0 */
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
      // Message CAN recu
      TreatCanMsg(0, u8CanNum, &RxHeader, RxData);
    }
  }
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo1ITs)
{
  FDCAN_RxHeaderTypeDef RxHeader;
  uint8_t RxData[8];
  uint8_t u8CanNum = 0;
  if (hfdcan->Instance == FDCAN1) u8CanNum = 0;
  else if (hfdcan->Instance == FDCAN2) u8CanNum = 1;
  else if (hfdcan->Instance == FDCAN3) u8CanNum = 2;
  else return;

  if ((RxFifo1ITs & FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != RESET)
  {
    /* Retrieve Rx messages from RX FIFO1 */
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &RxHeader, RxData) == HAL_OK)
    {
      // Message CAN recu
      TreatCanMsg(1, u8CanNum, &RxHeader, RxData);
    }
  }
}



// Gestion de l'emission de messages CAN
// Uniquement a titre d'exemple

typedef struct {
  uint32_t u29_ID : 29;
  uint32_t u3_Spare : 3;
  uint8_t u8_MsgLen;
  uint8_t au8_Data[8];
}CANTxMsg;


#define CAN_TX_BUFF_SIZE      128
CANTxMsg aoCanTxBuff[MAX_NUM_CAN][CAN_TX_BUFF_SIZE];
uint32_t au32_CanTxWrPt[MAX_NUM_CAN] = { 0 };
uint32_t au32_CanTxRdPt[MAX_NUM_CAN] = { 0 };



// send next data on CAN
void CAN_TX_IT_send_next_data(FDCAN_HandleTypeDef* hfdcan)
{
  FDCAN_TxHeaderTypeDef TxHeader;

  uint8_t u8CanNum = 0;
  if (hfdcan->Instance == FDCAN1) u8CanNum = 0;
  else if (hfdcan->Instance == FDCAN2) u8CanNum = 1;
  else if (hfdcan->Instance == FDCAN3) u8CanNum = 2;
  else return;

  if ((au32_CanTxRdPt[u8CanNum] == au32_CanTxWrPt[u8CanNum]) || (HAL_FDCAN_GetTxFifoFreeLevel(hfdcan) == 0))
    return;

  // copy data into registers
  TxHeader.Identifier = aoCanTxBuff[u8CanNum][au32_CanTxRdPt[u8CanNum]].u29_ID;                                       /*!< Specifies the identifier.*/
  TxHeader.IdType = FDCAN_STANDARD_ID;  // ou FDCAN_EXTENDED_ID
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;                                                                            /*!< Specifies the frame type of the message that will be transmitted.*/
  TxHeader.DataLength = (aoCanTxBuff[u8CanNum][au32_CanTxRdPt[u8CanNum]].u8_MsgLen << 16);                            /*!< Specifies the length of the frame that will be transmitted. */
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;                                                                    /*!< Specifies the error state indicator. */
  TxHeader.BitRateSwitch = FDCAN_BRS_OFF;                                                                             /*!< Specifies whether the Tx frame will be transmitted with or without bit rate switching. */
  TxHeader.FDFormat = FDCAN_CLASSIC_CAN;                                                                              /*!< Specifies whether the Tx frame will be transmitted in classic or FD format. */
  TxHeader.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;                                                                /*!< Specifies the event FIFO control. */
  TxHeader.MessageMarker = au32_CanTxRdPt[u8CanNum] & 0xFF;                                                           /*!< Specifies the message marker to be copied into Tx Event FIFOelement for identification of Tx message status. */

  HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &TxHeader, aoCanTxBuff[u8CanNum][au32_CanTxRdPt[u8CanNum]].au8_Data);

  // increase read pointer
  au32_CanTxRdPt[u8CanNum]++;
  au32_CanTxRdPt[u8CanNum] %= CAN_TX_BUFF_SIZE;
}

void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef* hfdcan, uint32_t BufferIndexes)
{
  // send next data if any
  CAN_TX_IT_send_next_data(hfdcan);
}

// compute remaining data to send by the IT buffer.
uint16_t CAN_TX_IT_get_buffer_usage(uint8_t u8CanNum)
{
  if (au32_CanTxRdPt[u8CanNum] != au32_CanTxWrPt[u8CanNum])
  {
    return (((CAN_TX_BUFF_SIZE - au32_CanTxRdPt[u8CanNum]) + au32_CanTxWrPt[u8CanNum]) % CAN_TX_BUFF_SIZE);
  }
  else
    return 0;
}

uint8_t CAN_IT_put(uint8_t u8CanNum, uint32_t u32_ID, uint8_t* pData, uint16_t u16_size)
{
  uint32_t u32_free_buffer_memory;
  FDCAN_HandleTypeDef* hfdcan = NULL;

  if (u16_size > 8)
  {
    LOG_WARNING("CAN msg ignore due to length exceed 8 bytes");
    return 0;
  }
  
  if (u8CanNum == 0)
    hfdcan = &hfdcan1;
  else if (u8CanNum == 1)
    hfdcan = &hfdcan2;
  else if (u8CanNum == 2)
    hfdcan = &hfdcan3;
  else
    return 1;

  do {
    // compute free memory in buffer
    u32_free_buffer_memory = CAN_TX_BUFF_SIZE - CAN_TX_IT_get_buffer_usage(u8CanNum) - 1;

    // check if enough space in buffer to copy data, else wait until end of previous transfer
    if (u32_free_buffer_memory < 1)
    {
      // supress older message
      au32_CanTxRdPt[u8CanNum]++;
      au32_CanTxRdPt[u8CanNum] %= CAN_TX_BUFF_SIZE;
    }
  } while (u32_free_buffer_memory < 1); // until exit on error or enough space

  /* copy data to tx buffer */
  aoCanTxBuff[u8CanNum][au32_CanTxWrPt[u8CanNum]].u29_ID = u32_ID;
  aoCanTxBuff[u8CanNum][au32_CanTxWrPt[u8CanNum]].u8_MsgLen = u16_size;
  memcpy(aoCanTxBuff[u8CanNum][au32_CanTxWrPt[u8CanNum]].au8_Data, pData, u16_size);

  // compute new write pointer
  au32_CanTxWrPt[u8CanNum]++;
  au32_CanTxWrPt[u8CanNum] %= CAN_TX_BUFF_SIZE;

  CAN_TX_IT_send_next_data(hfdcan);

  return 0;
}

/* USER CODE END 1 */
