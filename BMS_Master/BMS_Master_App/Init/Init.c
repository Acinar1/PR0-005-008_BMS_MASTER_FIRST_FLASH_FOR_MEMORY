/**
  ******************************************************************************
  * File Name          : Init.c
  * Version            : 1.0
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */

/* Includes */
#include "main.h"
#include "Init.h"
#include "Debug/debug.h"

#if __has_include("ModbusSlave/modbus.h")
#include "ModbusSlave/modbus.h"
#define ModBusSlaveToInitAndUse		1
#else
#define ModBusSlaveToInitAndUse		0
#endif

#if __has_include("Debug/Test.h")
#include "Debug/Test.h"
#define TestToInitAndUse		1
#else
#define TestToInitAndUse		0
#endif

#if __has_include("DriverLeds/lp5024.h")
#include "DriverLeds/lp5024.h"
#define LP5024ToInitAndUse		1
#else
#define LP5024ToInitAndUse		0
#endif

#if __has_include("DriverLeds/IS31FL3218.h")
#include "DriverLeds/IS31FL3218.h"
#define IS31FL3218ToInitAndUse		1
#else
#define IS31FL3218ToInitAndUse		0
#endif

#define FlashToInit		        0
#define FlashManagerToInit		0
#if __has_include("Flash/FlashManager.h")
#include "Flash/FlashManager.h"
#undef FlashManagerToInit
#define FlashManagerToInit		1
#else
#if __has_include("Flash/AT25XExxx.h")
#include "Flash/AT25XExxx.h"
#undef FlashToInit
#define FlashToInit		1
#endif
#if __has_include("Flash/SST25VF080.h")
#include "Flash/SST25VF080.h"
#undef FlashToInit
#define FlashToInit		1
#endif
#if __has_include("Flash/W25Q01JV.h")
#include "Flash/W25Q01JV.h"
#undef FlashToInit
#define FlashToInit		1
#endif
#endif

#if __has_include("RTC/PCF2129.h")
#include "RTC/PCF2129.h"
#define PCFToInit		1
#else
#define PCFToInit		0
#endif

#if __has_include("RTC/RX8111CE.h")
#include "RTC/RX8111CE.h"
#define RX8111ToInit		1
#else
#define RX8111ToInit		0
#endif

#if __has_include("OneWireUART/ow.h")
#include "OneWireUART/ow.h"
#define OneWireToInit		1
#else
#define OneWireToInit		0
#endif

#include "iwdg.h"

/* USER CODE BEGIN include */

/* USER CODE END include */

/* definitions */
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/* variables */

/* USER CODE BEGIN Private variables */

/* USER CODE END Private variables */

/* local functions prototypes */

/* USER CODE BEGIN Private function prototypes */

/* USER CODE END Private function prototypes */

/* functions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void Init_Micro(void)
{
/* USER CODE BEGIN Init 0 */

/* USER CODE END Init 0 */

#if defined(STM32G0)
// Dans le cas d'un G0 il faut desactiver le bit BOOT_SEL pour pouvoir programmer avec le hotprog (en mode bootloader interne)
  FLASH_OBProgramInitTypeDef pOBTest;
  //Force BOOT_SEL to 0 to insure further HotProg programming
  HAL_FLASHEx_OBGetConfig(&pOBTest);
  if((pOBTest.USERConfig & FLASH_OPTR_nBOOT_SEL) != 0)
  {
    if (HAL_FLASH_Unlock() == HAL_OK)
    {
      if (HAL_FLASH_OB_Unlock() == HAL_OK)
      {
        pOBTest.USERConfig &= ~FLASH_OPTR_nBOOT_SEL;
        /* Clear SR register */
        FLASH->SR = FLASH_SR_CLEAR;
        HAL_FLASHEx_OBProgram(&pOBTest);
        HAL_FLASH_OB_Launch();
      }
    }
	HAL_FLASH_OB_Lock();
	HAL_FLASH_Lock();
  }
#endif

// TODO a tester sur tous les micro !!
// Valide sur G0B1
// non fonctionnel sur F030
// non teste sur G473
// non teste sur F7xx
#if defined(STM32G0)
  __HAL_RCC_DBGMCU_CLK_ENABLE();
  __HAL_DBGMCU_FREEZE_IWDG();
#endif

  UART_Ex_Init();

  /* USER CODE BEGIN Init Uart */

  /* USER CODE END Init Uart */

  // check reset cause
  // WatchDog occurred
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
  {
    LOG_ERROR("WatchDog Reset Occurred !!!");
    /* USER CODE BEGIN WatchDog Reset */
    /* USER CODE END WatchDog Reset */
  }
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
  {
    LOG_ERROR("Window WatchDog Reset Occurred !!!");
    /* USER CODE BEGIN Window WatchDog Reset */
    /* USER CODE END Window WatchDog Reset */
  }
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
  {
    LOG_WARNING("Software Reset Occurred !!!");
    /* USER CODE BEGIN Software Reset */
    /* USER CODE END Software Reset */
  }

#if defined(STM32G4) || defined(STM32L4) || defined(STM32F7)
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST))
  {
    LOG_WARNING("BOR Reset Occurred !!!");
    /* USER CODE BEGIN BOR Reset */
    /* USER CODE END BOR Reset */
  }
#else
#if defined(STM32G0)
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_PWRRST))
#else
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
#endif
  {
    LOG_INFO("POR/PDR Reset Occurred !!!");
    /* USER CODE BEGIN POR/PDR Reset */
    /* USER CODE END POR/PDR Reset */
  }
#endif
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
  {
    LOG_WARNING("Pin Reset Occurred !!!");
    /* USER CODE BEGIN Pin Reset */
    /* USER CODE END Pin Reset */
  }
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
  {
    LOG_WARNING("Low-Power Reset Occurred !!!");
    /* USER CODE BEGIN Low-Power Reset */
    /* USER CODE END Low-Power Reset */
  }

#if defined(STM32F7) || defined(STM32L4)
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY))
  {
    LOG_WARNING("LSI Ready Reset Occurred !!!");
    /* USER CODE BEGIN LSI Ready Reset */
    /* USER CODE END LSI Ready Reset */
  }
#endif

#if defined(STM32L4)
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_FWRST))
  {
    LOG_WARNING("Firewall Reset Occurred !!!");
    /* USER CODE BEGIN LSI Ready Reset */
    /* USER CODE END LSI Ready Reset */
  }
#endif

#if !defined(STM32F7)
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_OBLRST))
  {
    LOG_WARNING("Option byte loader Reset Occurred !!!");
    /* USER CODE BEGIN Option byte Reset */
    /* USER CODE END Option byte Reset */
  }
#endif
  // clear WatchDog flag
  __HAL_RCC_CLEAR_RESET_FLAGS();

/* USER CODE BEGIN Init 01 */

/* USER CODE END Init 01 */

#if (LP5024ToInitAndUse != 0)
  /* USER CODE BEGIN Init LP5024 */
  LP5024_Init(adresse);
  /* USER CODE END Init LP5024 */
#endif

#if (IS31FL3218ToInitAndUse != 0)
  /* USER CODE BEGIN Init IS31FL3218 */
  IS31FL3218_Init();
  /* USER CODE END Init IS31FL3218 */
#endif

#if (FlashToInit != 0) || (FlashManagerToInit != 0)
  /* USER CODE BEGIN PrevInit Flash */
  /* USER CODE END PrevInit Flash */
#if (FlashManagerToInit != 0)
  FlashDiscoverAndInit();
#else
  FlashInit();
#endif
  /* USER CODE BEGIN AfterInit Flash */
  /* USER CODE END AfterInit Flash */
#endif

#if (PCFToInit != 0)
  /* USER CODE BEGIN PrevInit PCF */
  /* USER CODE END PrevInit PCF */
  PCF2129_Init();
  /* USER CODE BEGIN AfterInit PCF */
  /* USER CODE END AfterInit PCF */
#endif

#if (RX8111ToInit != 0)
  /* USER CODE BEGIN PrevInit RX8111 */
  /* USER CODE END PrevInit RX8111 */
  RX8111_Init();
  /* USER CODE BEGIN AfterInit RX8111 */
  /* USER CODE END AfterInit RX8111 */
#endif

#if (ModBusSlaveToInitAndUse != 0)
  /* USER CODE BEGIN PrevInit ModBus */
  /* USER CODE END PrevInit ModBus */
#if ((MB_RTU_ENABLED != 0) || (MB_ASCII_ENABLED != 0))
  ModbusSlaveStart(MODBUS_ADDRESS, MODBUS_BAUDRATE, MODBUS_PARITY);
#else
  ModbusSlaveStart(MODBUS_TCP_PORT);
#endif

  /* USER CODE BEGIN AfterInit ModBus */
  /* USER CODE END AfterInit ModBus */
#endif

#if (OneWireToInit != 0)
  /* USER CODE BEGIN OneWire */
  // Faire une init par bus
  OneWire_GetAllDevice(0, &hOW1, &s_usartx, MAX_1WIRE, oSondeToRead_ow1, 200, 4);
  /* USER CODE END OneWire */
#endif

/* USER CODE BEGIN Init 1 */

/* USER CODE END Init 1 */
}

void Divers_Task(void)
{
/* USER CODE BEGIN Task 0 */

/* USER CODE END Task 0 */

  WatchDogKick();

#if (TestToInitAndUse != 0)
  // for unitary test
  InnoTest_Console();
#endif

#if (ModBusSlaveToInitAndUse != 0)
  // ModBus
  /* USER CODE BEGIN Prev ModBus */
  /* USER CODE END Prev ModBus */
  ModbusSlaveManager();
  /* USER CODE BEGIN After ModBus */
  /* USER CODE END After ModBus */
#endif

/* USER CODE BEGIN Task 1 */

/* USER CODE END Task 1 */
}

void WatchDogKick(void)
{
  HAL_IWDG_Refresh(&hiwdg);
}

void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY)
  {
    wait += (uint32_t)(uwTickFreq);
  }

  while ((HAL_GetTick() - tickstart) < wait)
  {
    WatchDogKick();
  }
}

