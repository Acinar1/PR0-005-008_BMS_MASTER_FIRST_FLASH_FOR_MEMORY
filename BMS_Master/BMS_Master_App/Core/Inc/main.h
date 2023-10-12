/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../Common.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef struct
{
  uint32_t u32_User1;
  uint32_t u32_User2;
  uint32_t u32_User3;
  uint32_t u32_User4;
}UserStruct_s;
  
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

#define BMS_IDLE_STATE 0
#define BMS_DRIVING_STATE 1
#define BMS_CHARGING_STATE 2
#define BMS_ERROR_STATE 3

//#define RELAY_OPEN 1
//#define RELAY_CLOSED 2  
	
#define nBModule 2
	

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void handler20ms();
	

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EtatVerrouPrise_Pin GPIO_PIN_13
#define EtatVerrouPrise_GPIO_Port GPIOC
#define VerrouSens1_Pin GPIO_PIN_14
#define VerrouSens1_GPIO_Port GPIOC
#define VerrouSens2_Pin GPIO_PIN_15
#define VerrouSens2_GPIO_Port GPIOC
#define MesureBattService_Pin GPIO_PIN_0
#define MesureBattService_GPIO_Port GPIOC
#define LowCurrentRange_Pin GPIO_PIN_1
#define LowCurrentRange_GPIO_Port GPIOC
#define HighCurrentRange_Pin GPIO_PIN_2
#define HighCurrentRange_GPIO_Port GPIOC
#define MesureRcCable_Pin GPIO_PIN_3
#define MesureRcCable_GPIO_Port GPIOC
#define TensionPackAccus_Pin GPIO_PIN_0
#define TensionPackAccus_GPIO_Port GPIOA
#define MesureFuite1_Pin GPIO_PIN_1
#define MesureFuite1_GPIO_Port GPIOA
#define MesureFuite2_Pin GPIO_PIN_2
#define MesureFuite2_GPIO_Port GPIOA
#define ModifImpedanceCP_Pin GPIO_PIN_3
#define ModifImpedanceCP_GPIO_Port GPIOA
#define Flash_CS_Pin GPIO_PIN_4
#define Flash_CS_GPIO_Port GPIOA
#define Ana_Tor1_Pin GPIO_PIN_4
#define Ana_Tor1_GPIO_Port GPIOC
#define Ana_Tor2_Pin GPIO_PIN_5
#define Ana_Tor2_GPIO_Port GPIOC
#define Ana_Tor3_Pin GPIO_PIN_0
#define Ana_Tor3_GPIO_Port GPIOB
#define ContactClef_Pin GPIO_PIN_1
#define ContactClef_GPIO_Port GPIOB
#define FrequenceCP_Pin GPIO_PIN_2
#define FrequenceCP_GPIO_Port GPIOB
#define Tor4_Pin GPIO_PIN_10
#define Tor4_GPIO_Port GPIOB
#define CdeSortieReserve_Pin GPIO_PIN_11
#define CdeSortieReserve_GPIO_Port GPIOB
#define CdeSortieReserve2_Pin GPIO_PIN_12
#define CdeSortieReserve2_GPIO_Port GPIOB
#define CdeSortieReserve3_Pin GPIO_PIN_13
#define CdeSortieReserve3_GPIO_Port GPIOB
#define CdeSortieReserve4_Pin GPIO_PIN_14
#define CdeSortieReserve4_GPIO_Port GPIOB
#define CdeAlimCapteurCourant_Pin GPIO_PIN_15
#define CdeAlimCapteurCourant_GPIO_Port GPIOB
#define Led_Rouge_Pin GPIO_PIN_6
#define Led_Rouge_GPIO_Port GPIOC
#define Led_Verte_Pin GPIO_PIN_7
#define Led_Verte_GPIO_Port GPIOC
#define HVIL3_Pin GPIO_PIN_8
#define HVIL3_GPIO_Port GPIOC
#define HVIL2_Pin GPIO_PIN_9
#define HVIL2_GPIO_Port GPIOC
#define HVIL1_Pin GPIO_PIN_8
#define HVIL1_GPIO_Port GPIOA
#define TXDebug_Pin GPIO_PIN_9
#define TXDebug_GPIO_Port GPIOA
#define RXDebug_Pin GPIO_PIN_10
#define RXDebug_GPIO_Port GPIOA
#define RXcan_BMS_Pin GPIO_PIN_11
#define RXcan_BMS_GPIO_Port GPIOA
#define TXcan_BMS_Pin GPIO_PIN_12
#define TXcan_BMS_GPIO_Port GPIOA
#define CdeConvertisseurDCDC_Pin GPIO_PIN_15
#define CdeConvertisseurDCDC_GPIO_Port GPIOA
#define CdeAlimBmsSlave_Pin GPIO_PIN_10
#define CdeAlimBmsSlave_GPIO_Port GPIOC
#define CdeRelaisDePuissance_Pin GPIO_PIN_11
#define CdeRelaisDePuissance_GPIO_Port GPIOC
#define RX_IOT_Pin GPIO_PIN_12
#define RX_IOT_GPIO_Port GPIOC
#define TX_IOT_Pin GPIO_PIN_2
#define TX_IOT_GPIO_Port GPIOD
#define RXcan_RTF2_Pin GPIO_PIN_3
#define RXcan_RTF2_GPIO_Port GPIOB
#define TXcan_RTF2_Pin GPIO_PIN_4
#define TXcan_RTF2_GPIO_Port GPIOB
#define RXcan_RTF_Pin GPIO_PIN_5
#define RXcan_RTF_GPIO_Port GPIOB
#define TXcan_RTF_Pin GPIO_PIN_6
#define TXcan_RTF_GPIO_Port GPIOB
#define TestIsolementPlus_Pin GPIO_PIN_7
#define TestIsolementPlus_GPIO_Port GPIOB
#define TestIsolementMoins_Pin GPIO_PIN_9
#define TestIsolementMoins_GPIO_Port GPIOB
	

/* USER CODE BEGIN Private defines */


	extern volatile uint32_t u32_BootVersion;
	extern volatile uint32_t u32_BootMode;
	extern volatile uint32_t u32_HardFault_PC;
	extern volatile uint32_t u32_HardFault_LR;
	extern uint32_t u32_MemoryAddr;
	extern uint32_t u32_FirmwareSizeMax;  
	
	extern volatile int BMS_Temperature[nBModule][5]; // Tableau des températures des 12 modules. Température index 4 étant la température intérieure puis les 4 autres températures.
	extern volatile int BMS_Voltage[nBModule][9]; // Tableau des tensions des 12*8 cellules. Tension index 8 étant la tension du module.	volatile int BMS_Balancing[12][8]; // Tableau de commande du balancing
	extern volatile int BMS_Balancing[nBModule][8]; // Tableau de commande du balancing
	extern bool CellOvervoltage;
	extern bool CellUndervoltage;
	extern bool CellOverheat;	
	extern bool cellBalancingStatus;
	extern bool cellBalancingStatusZ1;
	extern int inverterStatus;
	extern int engineSpeedRawCAN;
	extern int CAN_InverterCurrent;
	extern int averageSpeed;
	extern int averageSpeedTemp ;
	extern int cptSpeed ;
	extern int averageVoltage;
	extern int averageVoltageTemp;
	extern int cptVolt;
	extern int CAN_Speedometer;
	extern int CAN_WheelSpeed;
	extern int CAN_CapaVoltage;
	extern int regenCurrent;
	extern int DCDC_Current;
	extern int chargerCurrent ;
	extern int MCUStatus;
	extern int chargerVoltage ;
	extern int Charger_State ;
	extern int Charger_SectorPresence ;
	extern int Charger_PlugPresence;
	extern int Charger_Power;
	extern uint16_t BMS_SOC_percentage;
	
	
	
	extern int VERSION_BMS_MASTER_MAJ;
	extern int VERSION_BMS_MASTER_MIN;
	
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
