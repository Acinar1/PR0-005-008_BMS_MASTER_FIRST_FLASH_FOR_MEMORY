/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"

#include "fdcan.h"
#include "Can_BMS.h"
#include "CAN_Retrofit.h"
#include "iwdg.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "charger.h"
#include "relay.h"
#include "SOC_Computation.h"
#include "ErrorManagement.h"
#include "Consumption.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Debug/debug.h"
#include "Debug/Test.h"
#include "Flash/SST25VF080.h"
#include "Init/Init.h"
#include "Extra_Init/patched_it.h"
#include "Extra_Init/usart_ex.h"
#include "IO/IO.h"
#include "UpDate/UpDate.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// Definition des donnees echangees avec le bootloader
__attribute__((section(".btl_ram.BOOT_VERSION")))				volatile uint32_t u32_BootVersion;
__attribute__((section(".btl_ram.BOOT_MODE")))				  volatile uint32_t u32_BootMode;
__attribute__((section(".btl_ram.BOOT_HARDFAULT_PC")))	volatile uint32_t u32_HardFault_PC;
__attribute__((section(".btl_ram.BOOT_HARDFAULT_LR")))	volatile uint32_t u32_HardFault_LR;

// Definition des donnees utilisateur en RAM non initialisee (non modifiee lors d'un reset du micro
__attribute__((section(".no_init")))	volatile UserStruct_s oUserStruct;

// Definition des inforamtions de secteur flash interne pour le decoupage bootloader/applicatif
extern uint32_t _main_app_start_address;
extern uint32_t _firmware_length;
uint32_t u32_MemoryAddr = (uint32_t)&_main_app_start_address;
uint32_t u32_FirmwareSizeMax = (uint32_t)&_firmware_length;



	

bool endOfCharging = false;  


int BMS_State = BMS_IDLE_STATE;
int BMS_ErrorId = 0;
int BMS_SOC = 105*3600*50*0.99; // 100% -> 70*3*3600*50 = 37 800 000
uint16_t BMS_SOC_percentage = 99;
uint16_t BMS_SOC_percentage_before_charging = 0;
//int relay12V_State = RELAY_OPEN;
int periodicite100 = 0;
int periodicite1000 = 0;
int periodicite60000 = 0;
int inverterStatus = 0;
int MCUStatus = 0;
unsigned short voltageCellMax = 0;
int idVoltageCellMax = 0;
unsigned short voltageCellMin = 40000;
int idVoltageCellMin = 0;
int tempCellMax = 0;
int idTempCellMax = 0;
int tempCellMin = 100;
int idTempCellMin = 0;
int DCDC_Current = 0;
unsigned long pulseWidth; // Define Pulsewidth variable
int HallEffectCurrent = 0; //(x10)
int HighCurrentValue = 0; //channel 2
int LowCurrentValue = 0; //channel 1

int CAN_InverterCurrent = 0;
int currentLimitDrive = 200;
int currentLimitDriveZ1 = 200;
int TMDCLimitValue = 0;
int MDCLimitValue = 0;
int currentLimitRegen = 105;
int tempoMaxCurrent = 0;
int compteurBalancing = 0;
int regenCurrent = 0;
int chargerCurrent = 0;
int chargerVoltage = 0;
int Charger_State;
int Charger_SectorPresence;
int Charger_PlugPresence;
int Charger_Power;
int rawCurrent = 0;
int batteryVoltage = 0;
int overHeatBalancing = 0;
unsigned long pulseStartTimeZ1 = 0;
int engineSpeedRawCAN = 0;
int cptCharge = 0; //à sauvegarder dans la flash
int cptChargeWh = 0;
int cptChargeWs = 0;
int cptChargekWh = 0;
int cptSec = 0;


int cptTimeDriving = 0;

int cptSecondeCharging = 0;


int cptTimeCharging = 0;


int averageSpeed = 0;
int averageSpeedTemp = 0;
int cptSpeed = 0;
int averageVoltage = 0;
int averageVoltageTemp = 0;
int cptVolt = 0;
int CAN_Speedometer = 0;
int CAN_WheelSpeed = 0;
int CAN_CapaVoltage = 0;
int InstantConsumption = 0;
int AverageConsumption = 2000; //(x100)
int InstantGenerative = 0;
int AverageGenerative = 500; //(x100)
int Range = 0;
int Conso = 0;
int Vehicle_Speed = 0;
int cptHall = 0;
int cptHallRegen = 0;
int averageHall = 0;
int averageHallRegen = 0;
int averageHallTemp = 0;
int averageHallRegenTemp = 0;

int VERSION_BMS_MASTER_MAJ = 1;
int VERSION_BMS_MASTER_MIN = 0;

volatile int BMS_Temperature[nBModule][5]; // Tableau des températures des 12 modules. Température index 4 étant la température intérieure puis les 4 autres températures.
volatile int BMS_Voltage[nBModule][9]; // Tableau des tensions des 12*8 cellules. Tension index 8 étant la tension du module.	volatile int BMS_Balancing[12][8]; // Tableau de commande du balancing
volatile int BMS_Balancing[nBModule][8]; // Tableau de commande du balancing

bool CellOvervoltage = false;
bool CellUndervoltage = false;
bool CellOverheat = false;	
bool cellBalancingStatus = false;
bool cellBalancingStatusZ1 = false;



volatile unsigned long isrPulsewidth; // Define Interrupt Service Routine (ISR) pulsewidth
volatile unsigned long isrCycleWidth; //Define interrupt service routine cycle width

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void init_BMS()
{
	int firstIndex = 0;
	int secondIndex = 0;
	
	WriteAllFlash(0, 0, 0, 0, 0, 0, 2500);
	//WriteAllFlash(cptCharge, AverageConsumption, AverageGenerative, cptTimeCharging,cptTimeDriving, cptChargeWh, BMS_SOC_percentage);
	//ReadAllFlash(&cptCharge, &AverageConsumption, &AverageGenerative, &cptTimeCharging, &cptTimeDriving, &cptChargeWh, &BMS_SOC_percentage, &BMS_SOC);

	
	

	// Retrieve SOC value in flash memory
	
  

	// Mise à 0 des tableaux de température, de tension et de commande de balancing
	
}
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

    // only to reserve data address
  __ASM __volatile__(""::"m" (u32_BootVersion));
  __ASM __volatile__(""::"m" (u32_BootMode));
  __ASM __volatile__(""::"m" (u32_HardFault_PC));
  __ASM __volatile__(""::"m" (u32_HardFault_LR));
  __ASM __volatile__(""::"m" (oUserStruct));

  // Switch vector table
  SCB->VTOR = u32_MemoryAddr;
  __enable_irq();
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CRC_Init();
  MX_IWDG_Init();
  MX_USART1_UART_Init();
  //MX_FDCAN1_Init();
  //MX_FDCAN2_Init();
  //MX_FDCAN3_Init();
  MX_SPI1_Init();
  MX_UART5_Init();
  //MX_ADC1_Init();
  //MX_TIM5_Init();
  //MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  Init_Micro();
  
  LOG_INFO("Start BMS Master V%d.%02d - Build %s - %s", VERSION_BMS_MASTER_MAJ, VERSION_BMS_MASTER_MIN, __DATE__, __TIME__);
  
  switch (u32_BootMode)
  {
  case BOOT_MODE_UPDATE_DONE:
    LOG_INFO("Firmware updated sucessfully, clear previous defaults");
    // TODO voir si on efface la zone uniquement lors d'une MAJ reussi ?
    ClearUpdate();
    break;
  case BOOT_MODE_ERROR_CRC:
    LOG_ERROR("Firmware update failed !! 0x%08X", u32_BootMode);
    break;
  case BOOT_MODE_ERROR_SIZE:
    LOG_ERROR("Firmware update failed !! 0x%08X", u32_BootMode);
    break;
  case BOOT_MODE_ERROR_FLASH_LOCK:
    LOG_ERROR("Firmware update failed !! 0x%08X", u32_BootMode);
    break;
  case BOOT_MODE_ERROR_BLANK:
    LOG_ERROR("Firmware update failed !! 0x%08X", u32_BootMode);
    break;
  case BOOT_MODE_ERROR_VERIFY:
    LOG_ERROR("Firmware update failed !! 0x%08X", u32_BootMode);
    break;
  case BOOT_MODE_ERROR_NULL:
    LOG_ERROR("Firmware update failed !! 0x%08X", u32_BootMode);
    break;
  case BOOT_MODE_ERROR_HARD_FAULT:
    LOG_ERROR("Hard Fault occurred ! PC=0x%08X LR=0x%08X", u32_HardFault_PC, u32_HardFault_LR);
    break;
  }
  // reinit all BOOT infos
  if (u32_BootMode != BOOT_MODE_UPDATE_TO_DO) // efface uniquement si aucune mise a jour n'est a realiser
    u32_BootMode = 0;
  // TODO voir si on efface forcement la zone ici ou uniquement lors d'une MAJ reussi ?
  // ClearUpdate();
  u32_HardFault_PC = 0;
  u32_HardFault_LR = 0;
	
 // Start_ADC_Read();
  //Start_Can(); 
  //HAL_TIM_Base_Start_IT(&htim3); // On lance le timer 3
  //HAL_TIM_Base_Start(&htim5); // On lance le timer 5
	//Start_handler;// j'avais fait ca en m'inspirant de ce qu'il avait fait pour la fréquence CP mais inutile car j'ai rajouter "_IT" dans le HAL_TIM_BASE_START
  
	init_BMS();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //InPutFilter();
    
    Divers_Task();
    
    // TODO faire la comm inter micro avec IOT
    // TODO faire la MAJ
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
