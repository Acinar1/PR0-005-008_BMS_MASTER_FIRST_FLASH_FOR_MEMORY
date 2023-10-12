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
#include "crc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Flash/Flash.h"
#include "UpDate/UpDate.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Verbose mode
 * 0: only important informations are displayed
 * 1: error details are displayed
 * 2: info details are displayed
 */
#define VERBOSE_MODE			1

#define FLASH_START_ADDRESS					      0x08000000			//! location of CPU flash bank.

#define ADDR_NewFirmwareBinary            (ADDR_NewFirmwareHeader + sizeof(UpDateFileHeader_t))

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// main app start address defined in linker file
extern uint32_t _main_app_start_address;
extern uint32_t _firmware_length;

uint32_t u32_MemoryAddr = (uint32_t)&_main_app_start_address;
uint32_t u32_FirmwareSizeMax = (uint32_t)&_firmware_length;

uint32_t u32_BootLoader_Version = 0x00000001;

__attribute__((section(".btl_ram.BOOT_VERSION")))				volatile uint32_t u32_BootVersion;
__attribute__((section(".btl_ram.BOOT_MODE")))				  volatile uint32_t u32_BootMode;
__attribute__((section(".btl_ram.BOOT_HARDFAULT_PC")))	volatile uint32_t u32_HardFault_PC;
__attribute__((section(".btl_ram.BOOT_HARDFAULT_LR")))	volatile uint32_t u32_HardFault_LR;

uint32_t ADDR_NewFirmwareHeader = FlashAddrOfNewFirmware;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

static uint32_t GetPage(uint32_t Address);
static uint32_t GetBank(uint32_t Address);
__STATIC_INLINE void config(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/**
* Jump to application
*/
void JumptoApp(void)
{
  // disable global interrupt
  __disable_irq();

  RCC->CICR = 0;

  // Disable all peripheral interrupts
  NVIC->ICER[0U] = 0xFFFFFFFF;

  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL = 0;

  // disable peripherals
  USART1->CR1 &= ~USART_CR1_UE;
  __HAL_RCC_USART1_CLK_DISABLE();

  SPI2->CR1 &= ~SPI_CR1_SPE;
  __HAL_RCC_SPI2_CLK_DISABLE();

  __HAL_RCC_APB1_FORCE_RESET();
  __HAL_RCC_APB1_RELEASE_RESET();

  __HAL_RCC_APB2_FORCE_RESET();
  __HAL_RCC_APB2_RELEASE_RESET();

  uint32_t* pMem = (uint32_t*)u32_MemoryAddr;

  // First address is the stack pointer initial value
  __set_MSP(*pMem); // Set stack pointer

                    // Now get main app entry point address
  pMem++;
  void(*pMainApp)(void) = (void(*)(void))(*pMem);

  // Jump to main application (0x0800 0004)
  pMainApp();
}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    return FLASH_BANK_1;
  else
    return FLASH_BANK_2;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  uint32_t i;
  uint32_t u32_address;
  uint64_t u64_data;
  uint8_t au8_Data[4];

  uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
  uint32_t PageError = 0;
  
  UpDateFileHeader_t oUpDateFileHeader = { 0 };

  /*Variable used for Erase procedure*/
  static FLASH_EraseInitTypeDef EraseInitStruct;

  // only to reserve data address
  __ASM __volatile__("" :: "m" (u32_BootVersion));
  __ASM __volatile__("" :: "m" (u32_BootMode));
  __ASM __volatile__("" :: "m" (u32_HardFault_PC));
  __ASM __volatile__("" :: "m" (u32_HardFault_LR));

  // update the version of local bootloader
  u32_BootVersion = u32_BootLoader_Version;

#if 0
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CRC_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
#endif

  config();
  gpio_init();
  crc_init();
  usart_init();
  spi_init();

  LED_R_ON;
  HAL_Delay(100);
  LED_R_OFF;
  LED_G_ON;
  HAL_Delay(100);
  LED_G_OFF;
  _UART1_print("BootLoader V");
  _UART1_print_hex32(u32_BootLoader_Version);
  _UART1_print("\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (u32_BootMode == BOOT_MODE_UPDATE_TO_DO)
	  {
	    // Read Length and CheckSum from External Flash memory
      FlashRead(ADDR_NewFirmwareHeader, (uint8_t*)&oUpDateFileHeader, sizeof(UpDateFileHeader_t));

      // an update must be done
      if ((oUpDateFileHeader.u32_ValidityKey == UpDateFileValidityKey) &&
        ((oUpDateFileHeader.u32_BinaryFileSize != 0) && (oUpDateFileHeader.u32_BinaryFileSize != 0xFFFFFFFF)))
      {
        _UART1_print("Updating\r\n");
        // Si taille MAJ differente de 0 on fait la MAJ
        if (oUpDateFileHeader.u32_BinaryFileSize != 0)
        {
          if (oUpDateFileHeader.u32_BinaryFileSize < u32_FirmwareSizeMax)
          {
            // reset CRC calculation
            CRC->CR |= CRC_CR_RESET;
            for (i = ADDR_NewFirmwareBinary; i < (ADDR_NewFirmwareBinary + oUpDateFileHeader.u32_BinaryFileSize); i += 4)
            {
              FlashRead(i, (uint8_t*)&au8_Data, 4);
              CRC->DR = ((uint32_t)au8_Data[0] << 24U) | ((uint32_t)au8_Data[1U] << 16U) | ((uint32_t)au8_Data[2U] << 8U) | (uint32_t)au8_Data[3U];
            }
            uint32_t u32_Crc = (CRC->DR ^ 0xffffffffL);
            if (u32_Crc != oUpDateFileHeader.u32_BinaryFileCRC32)
            {
              LED_R_ON;
              u32_BootMode = BOOT_MODE_ERROR_CRC;
              _UART1_print("/!\\Bad CRC\r\n");
  #if (VERBOSE_MODE > 0)
              _UART1_print("Expect ");
              _UART1_print_hex32(oUpDateFileHeader.u32_BinaryFileCRC32);
              _UART1_print(" Compute ");
              _UART1_print_hex32(u32_Crc);
              _UART1_print("\r\n");
  #endif
            }
          }
          else
          {
            LED_R_ON;
            u32_BootMode = BOOT_MODE_ERROR_SIZE;
            _UART1_print("/!\\Firm size over\r\n");
          }
        }
        else
          u32_BootMode = BOOT_MODE_ERROR_NULL;

        if ((u32_BootMode & 0xFF000000) != BOOT_MODE_ERROR_NULL) // MAJ
        {
          // unlock CPU flash
          // HAL_FLASH_Unlock();
          if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0U)
          {
            /* Authorize the FLASH Registers access */
            WRITE_REG(FLASH->KEYR, FLASH_KEY1);
            WRITE_REG(FLASH->KEYR, FLASH_KEY2);

            /* verify Flash is unlocked */
            if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0U)
            {
              u32_BootMode = BOOT_MODE_ERROR_FLASH_LOCK;
              _UART1_print("/!\\Unlock Fail\r\n");
            }
          }

          if ((u32_BootMode & 0xFF000000) != BOOT_MODE_ERROR_NULL)
          {
            // Effacement de toute la memoire d'application
  #if (VERBOSE_MODE > 1)
            _UART1_print("Erase\r\n");
  #endif
            /* Clear OPTVERR bit set on virgin samples */
            __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

            /* Erase the user Flash area
              (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

            // check if erase is only in first page or also on second page
            uint32_t u32_Size2Erase = FLASH_BANK_SIZE;
          
            /* Get the 1st page to erase */
            FirstPage = GetPage(u32_MemoryAddr);
            /* Get the number of pages to erase from 1st page */
            if ((u32_MemoryAddr + oUpDateFileHeader.u32_BinaryFileSize) >= FLASH_BANK_SIZE)
              u32_Size2Erase = FLASH_BANK_SIZE - (u32_MemoryAddr - FLASH_BASE) - 1;
            else
              u32_Size2Erase = oUpDateFileHeader.u32_BinaryFileSize;
            NbOfPages = GetPage((u32_MemoryAddr + u32_Size2Erase)) - FirstPage + 1;
            /* Get the bank */
            BankNumber = GetBank(u32_MemoryAddr);
            /* Fill EraseInit structure*/
            EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
            EraseInitStruct.Banks = BankNumber;
            EraseInitStruct.Page = FirstPage;
            EraseInitStruct.NbPages = NbOfPages;
            if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
            {
              LED_R_ON;
              _UART1_print("/!\\Erase fail\r\n");
              u32_BootMode = BOOT_MODE_ERROR_BLANK;
            }
            else
            {
              // Need to erase an other block
              if ((u32_MemoryAddr + oUpDateFileHeader.u32_BinaryFileSize) >= FLASH_BANK_SIZE)
              {
                /* Get the 1st page to erase */
                FirstPage = GetPage((FLASH_BASE + FLASH_BANK_SIZE));
                /* Get the number of pages to erase from 1st page */
                NbOfPages = GetPage((u32_MemoryAddr + oUpDateFileHeader.u32_BinaryFileSize)) - FirstPage + 1;
                /* Get the bank */
                BankNumber = FLASH_BANK_2;
                /* Fill EraseInit structure*/
                EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
                EraseInitStruct.Banks = BankNumber;
                EraseInitStruct.Page = FirstPage;
                EraseInitStruct.NbPages = NbOfPages;
                if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
                {
                  LED_R_ON;
                  _UART1_print("/!\\Erase fail\r\n");
                  u32_BootMode = BOOT_MODE_ERROR_BLANK;
                }
              }

              if ((u32_BootMode & 0xFF000000) != BOOT_MODE_ERROR_NULL)
              {
                // verification effacement
  #if (VERBOSE_MODE > 1)
  //					_UART1_print("Blank check\r\n");
  #endif
                for (u32_address = u32_MemoryAddr; u32_address <= (u32_MemoryAddr + oUpDateFileHeader.u32_BinaryFileSize); u32_address++)
                {
                  if (*(uint8_t*)u32_address != 0xFF)
                  {
                    LED_R_ON;
                    _UART1_print("/!\\Blank fail\r\n");
  #if (VERBOSE_MODE > 0)
                    _UART1_print("At @");
                    _UART1_print_hex32(u32_address);
                    _UART1_print("\r\n");
  #endif
                    u32_BootMode = BOOT_MODE_ERROR_BLANK;
                    break;
                  }
                }
              }

              if ((u32_BootMode & 0xFF000000) != BOOT_MODE_ERROR_NULL)
              {
                _UART1_print("Write & verify\r\n");
                for (u32_address = 0; u32_address < oUpDateFileHeader.u32_BinaryFileSize; u32_address += 8)
                {
                  FlashRead((ADDR_NewFirmwareBinary + u32_address), (uint8_t*)&u64_data, 8);
                  // programmation
                  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (u32_MemoryAddr + u32_address), u64_data);
                  // verification
                  if (*(uint64_t*)(u32_MemoryAddr + u32_address) != u64_data)
                  {
                    LED_R_ON;
                    u32_BootMode = BOOT_MODE_ERROR_VERIFY;
                    _UART1_print("/!\\Verif fail\r\n");
  #if (VERBOSE_MODE > 0)
                    _UART1_print("@");
                    _UART1_print_hex32((uint32_t)(u32_MemoryAddr + u32_address));
                    _UART1_print(" => ");	// Expected
                    _UART1_print_hex32((u64_data & 0xFFFFFFFF00000000) >> 32);
                    _UART1_print_hex32(u64_data & 0x00000000FFFFFFFF);
                    _UART1_print(" | ");	// Read
                    _UART1_print_hex32((*(uint64_t*)(u32_MemoryAddr + u32_address) & 0xFFFFFFFF00000000) >> 32);
                    _UART1_print_hex32((*(uint64_t*)(u32_MemoryAddr + u32_address) & 0x00000000FFFFFFFF));
                    _UART1_print("\r\n");
  #endif
                    break;
                  }
                }

                if ((u32_BootMode & 0xFF000000) != BOOT_MODE_ERROR_NULL)
                  u32_BootMode = 0;
              }
            }

            // Lock flash
            /* Set the LOCK Bit to lock the FLASH Registers access */
            SET_BIT(FLASH->CR, FLASH_CR_LOCK);
          }
        }
      }
	  }

    if ((u32_BootMode & 0xFF000000) != BOOT_MODE_ERROR_NULL)
      LED_R_OFF;
    _UART1_print("Start App\r\n");
    HAL_Delay(100);
    u32_BootVersion = u32_BootLoader_Version;
    JumptoApp();

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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void config(void)
{
  uint32_t prioritygroup;
  uint32_t wait_loop_index;
  uint32_t tickstart;
  uint32_t temp_sysclksrc;
  uint32_t temp_pllckcfg;
  uint32_t pllfreq;
  uint32_t hpre = RCC_SYSCLK_DIV1;

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  /* Set Interrupt Group Priority */
    /* Set the PRIGROUP[10:8] bits according to the PriorityGroup parameter value */
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* Use SysTick as time base source and configure 1ms tick (default clock after Reset is HSI) */
  /* Configure the SysTick to have interrupt in 1ms time basis*/
  if (SysTick_Config(SystemCoreClock / (1000U / uwTickFreq)) == 0U)
  {
    /* Configure the SysTick IRQ priority */
    prioritygroup = NVIC_GetPriorityGrouping();
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(prioritygroup, TICK_INT_PRIORITY, 0U));
    uwTickPrio = TICK_INT_PRIORITY;
  }
  else
  {
    return;
  }
  
  /* Init the low level hardware */
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/
  /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
  */
  /* Write 1 to disable the USB Type-C dead battery pull-down behavior */
  SET_BIT(PWR->CR3, PWR_CR3_UCPD_DBDIS);


  /* Configure the system clock */
  /** Configure the main internal regulator output voltage
  */
    /* If current range is range 2 */
  if (READ_BIT(PWR->CR1, PWR_CR1_VOS) == PWR_REGULATOR_VOLTAGE_SCALE2)
  {
    /* Make sure Range 1 Boost is disabled */
    SET_BIT(PWR->CR5, PWR_CR5_R1MODE);

    /* Set Range 1 */
    MODIFY_REG(PWR->CR1, PWR_CR1_VOS, PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Wait until VOSF is cleared */
    wait_loop_index = ((50UL * SystemCoreClock) / 1000000U) + 1U;
    while ((HAL_IS_BIT_SET(PWR->SR2, PWR_SR2_VOSF)) && (wait_loop_index != 0U))
    {
      wait_loop_index--;
    }
    if (HAL_IS_BIT_SET(PWR->SR2, PWR_SR2_VOSF))
    {
      return;
    }
  }
  /* If current range is range 1 normal or boost mode */
  else
  {
    /* Disable Range 1 Boost (no issue if bit already set) */
    SET_BIT(PWR->CR5, PWR_CR5_R1MODE);
  }
  
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  /*----------------------------- HSI Configuration --------------------------*/
  /* Check if HSI is used as system clock or as PLL source when PLL is selected as system clock */
  temp_sysclksrc = __HAL_RCC_GET_SYSCLK_SOURCE();
  temp_pllckcfg = __HAL_RCC_GET_PLL_OSCSOURCE();
  if (((temp_sysclksrc == RCC_CFGR_SWS_PLL) && (temp_pllckcfg == RCC_PLLSOURCE_HSI)) || (temp_sysclksrc == RCC_CFGR_SWS_HSI))
  {
    /* When HSI is used as system clock it will not be disabled */
    /* Otherwise, just the calibration is allowed */
    {
      /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
      __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_HSICALIBRATION_DEFAULT);

      /* Adapt Systick interrupt period */
      /* Configure the SysTick to have interrupt in 1ms time basis*/
      if (SysTick_Config(SystemCoreClock / (1000U / uwTickFreq)) == 0U)
      {
        /* Configure the SysTick IRQ priority */
        prioritygroup = NVIC_GetPriorityGrouping();
        NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(prioritygroup, uwTickPrio, 0U));
        uwTickPrio = uwTickPrio;
      }
      else
      {
        return;
      }
    }
  }
  else
  {
    /* Enable the Internal High Speed oscillator (HSI). */
    __HAL_RCC_HSI_ENABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till HSI is ready */
    while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0U)
    {
      if ((HAL_GetTick() - tickstart) > 2U)
      {
        return;
      }
    }

    /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
    __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_HSICALIBRATION_DEFAULT);
  }
  
  /** Initializes the CPU, AHB and APB buses clocks
  */
  /* To correctly read data from FLASH memory, the number of wait states (LATENCY)
    must be correctly programmed according to the frequency of the CPU clock
    (HCLK) and the supply voltage of the device. */

  /* Increasing the number of wait states because of higher CPU frequency */
  if (FLASH_LATENCY_0 > __HAL_FLASH_GET_LATENCY())
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_0);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if (__HAL_FLASH_GET_LATENCY() != FLASH_LATENCY_0)
    {
      return;
    }
  }
  
  /*------------------------- SYSCLK Configuration ---------------------------*/
  /* HSI is selected as System Clock Source */
  /* Check the HSI ready flag */
  if (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0U)
  {
    return;
  }
  /* Overshoot management when going down from PLL as SYSCLK source and frequency above 80Mhz */
  pllfreq = HAL_RCC_GetSysClockFreq();

  /* Intermediate step with HCLK prescaler 2 necessary before to go under 80Mhz */
  if (pllfreq > 80000000U)
  {
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_SYSCLK_DIV2);
    hpre = RCC_SYSCLK_DIV2;
  }
  
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_SYSCLKSOURCE_HSI);

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  while (__HAL_RCC_GET_SYSCLK_SOURCE() != (RCC_SYSCLKSOURCE_HSI << RCC_CFGR_SWS_Pos))
  {
    if ((HAL_GetTick() - tickstart) > 5000U)
    {
      return;
    }
  }
  
  /*-------------------------- HCLK Configuration --------------------------*/
  /* Set the highest APB divider in order to ensure that we do not go through
    a non-spec phase whatever we decrease or increase HCLK. */
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_HCLK_DIV16);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_HCLK_DIV16);

  /* Set the new HCLK clock divider */
  MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_SYSCLK_DIV1);
  
  /* Decreasing the number of wait states because of lower CPU frequency */
  if (FLASH_LATENCY_0 < __HAL_FLASH_GET_LATENCY())
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_0);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by polling the FLASH_ACR register */
    tickstart = HAL_GetTick();

    while (__HAL_FLASH_GET_LATENCY() != FLASH_LATENCY_0)
    {
      if ((HAL_GetTick() - tickstart) > 5000U)
      {
        return;
      }
    }
  }
  
  /*-------------------------- PCLK1 Configuration ---------------------------*/
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_HCLK_DIV1);

  /*-------------------------- PCLK2 Configuration ---------------------------*/
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, (RCC_HCLK_DIV1 << 3U));

  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HAL_RCC_GetSysClockFreq() >> (AHBPrescTable[READ_BIT(RCC->CFGR, RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos] & 0x1FU);

  /* Configure the source of time base considering new system clocks settings*/
  /* Configure the SysTick to have interrupt in 1ms time basis*/
  if (SysTick_Config(SystemCoreClock / (1000U / uwTickFreq)) == 0U)
  {
    /* Configure the SysTick IRQ priority */
    prioritygroup = NVIC_GetPriorityGrouping();
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(prioritygroup, uwTickPrio, 0U));
    uwTickPrio = uwTickPrio;
  }
  else
  {
    return;
  }
}
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
