
/**
  ******************************************************************************
  * File Name          : Test.c
  * Version            : 1.0
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */

/* Includes */
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "Test.h"
#include "debug.h"
#include "main.h"
#include "Init/Init.h"
#if defined(STM32G4)
#include "stm32g4xx_ll_utils.h"
#endif

#if __has_include("OneWireUART/ow.h")
#include "OneWireUART/ow.h"
#define OneWire2Test		1
#else
#define OneWire2Test		0
#endif

#if __has_include("RTC/PCF2129.h")
#include "RTC/PCF2129.h"
#define PCF21292Test		1
#else
#define PCF21292Test		0
#endif

#if __has_include("RTC/RX8111CE.h")
#include "PCF2129/RX8111CE.h"
#define RX8111CE2Test		1
#else
#define RX8111CE2Test		0
#endif

#if __has_include("Extra_Init/Time_RTC.h")
#include "Extra_Init/Time_RTC.h"
#define RTC2Test		1
#else
#define RTC2Test		0
#endif

#if __has_include("Flash/FlashManager.h")
#include "Flash/FlashManager.h"
#define FlashManagerToTest		1
#else
#define FlashManagerToTest		0
#if __has_include("Flash/AT25XExxx.h")
#include "Flash/AT25XExxx.h"
#define FlashAT25ToTest		1
#else
#define FlashAT25ToTest		0
#endif
#if __has_include("Flash/SST25VF080.h")
#include "Flash/SST25VF080.h"
#define FlashSST25ToTest		1
#else
#define FlashSST25ToTest		0
#endif
#if __has_include("Flash/W25Q01JV.h")
#include "Flash/W25Q01JV.h"
#define FlashW25ToTest		1
#else
#define FlashW25ToTest		0
#endif
#endif

/* USER CODE BEGIN include */
#include "IO/IO.h"
#include "fdcan.h"
#include "adc.h"
#include "tim.h"
/* USER CODE END include */

/* definitions */

#define CONSOLE_BUFF_SIZE		128

#define CMD_TEST_OK         "TEST_OK\r\n"
#define CMD_TEST_KO         "TEST_FAILED\r\n"
#define CMD_PROMPT          "<TESTINNO> "

#define CMD_HELP				    "HELP"
#define CMD_ABOUT				    "ABOUT"
#define CMD_TEMPO           "TEMPO"
#define CMD_TEST_EXIT			  "TEST_EXIT"
#define CMD_TEST_UC_ID			"TEST_UC_ID"
#define CMD_READY						"READY_FOR_TEST\r\n"

#if (OneWire2Test != 0)
#define CMD_TEST_OW	        "TEST_OW"
#endif

#if (PCF21292Test != 0) || (RTC2Test != 0) || (RX8111CE2Test != 0)
#define CMD_RTC_UPDATE      "SET_TIME"
#endif

#if (FlashAT25ToTest != 0) || (FlashSST25ToTest != 0) || (FlashW25ToTest != 0) || (FlashManagerToTest != 0)
#define CMD_TEST_FLASH		  "TEST_FLASH"
#endif

/* USER CODE BEGIN Private defines */

#define TEST_CMD_HEADER      "**"

#define CMD_TEST_LED        "TEST_LED"
#define CMD_TEST_IN_D       "TEST_DIN"
#define CMD_CHECK_IN_D      "CHECK_DIN"
#define CMD_GET_IN_D        "GET_DIN"
#define CMD_TEST_OUT_D      "TEST_DOUT"
#define CMD_TEST_CAN			  "TEST_CAN"
#define CMD_GET_IN_A        "GET_AIN"
#define CMD_GET_RC          "GET_RC"

#define TEST_UART            s_usart1

#define TIMEOUT_MODE            1
#define MAX_TIME_TEST_MODE_MS   300000      // 5 minutes

#define PutWrite(Buff, Len)     TEST_UART.TX_put(&TEST_UART, Buff, Len)

/* USER CODE END Private defines */

#ifndef TEST_CMD_HEADER
#define TEST_CMD_HEADER      "**"
#endif

#ifndef PutWrite
#define PutWrite(Buff, Len)     TEST_UART.TX_put(&TEST_UART, Buff, Len)
#endif

#define CheckTestCmd(tst_cmd)       (strncmp((char*)au8_ConsoleBuff, TEST_CMD_HEADER tst_cmd, (sizeof(tst_cmd) + sizeof(TEST_CMD_HEADER) - 2)) == 0)
#define SetCharTestPointer(tst_cmd) ((char*)au8_ConsoleBuff + sizeof(tst_cmd) + sizeof(TEST_CMD_HEADER) - 2)

#define DInMacroCheck0(IOName)       if ((MakePortPin(IOName, GPIO_Port)->IDR& MakePortPin(IOName, Pin)) == 0) { bOk = true; bEnd = true; }
#define DInMacroCheck1(IOName)       if ((MakePortPin(IOName, GPIO_Port)->IDR& MakePortPin(IOName, Pin)) != 0) { bOk = true; bEnd = true; }

/* variables */
uint8_t au8_ConsoleBuff[CONSOLE_BUFF_SIZE];
uint32_t u32_ConsoleWrPt = 0;
bool bRunTest = false;
static bool bReadySended = false;

/* USER CODE BEGIN Private variables */

/* USER CODE END Private variables */

/* local functions prototypes */

/* USER CODE BEGIN Private function prototypes */

/* USER CODE END Private function prototypes */

/* functions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
void InnoTest_Console(void)
{
  uint8_t u8_RcvChar;
  static uint32_t u32RunTestStart;
  char* pEnd;

  /* USER CODE BEGIN 1 */
  uint32_t i;
  /* USER CODE END 1 */

	// at first call send that the tested board is ready
	if (bReadySended == false)
	{
		bReadySended = true;
    PutWrite((uint8_t*)TEST_CMD_HEADER, strlen(TEST_CMD_HEADER));
    PutWrite((uint8_t*)CMD_READY, strlen(CMD_READY));
	}

  while ((TEST_UART.RX_get_buffer_usage(&TEST_UART) != 0)
#if (TIMEOUT_MODE != 0)
     || ((bRunTest) && ((HAL_GetTick() - u32RunTestStart) < MAX_TIME_TEST_MODE_MS))
#endif
    )
  {
	WatchDogKick();

    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    if (TEST_UART.RX_get_buffer_usage(&TEST_UART) != 0)
    {
      TEST_UART.RX_get(&TEST_UART, &u8_RcvChar);
      if ((isprint(u8_RcvChar)) && (u32_ConsoleWrPt < CONSOLE_BUFF_SIZE))
      {
        /* USER CODE BEGIN 3 */

        /* USER CODE END 3 */
        au8_ConsoleBuff[u32_ConsoleWrPt++] = u8_RcvChar;
      }
      if (u8_RcvChar == '\r')
      {
		/* USER CODE BEGIN 3BIS */
        PutWrite(au8_ConsoleBuff, u32_ConsoleWrPt);
        PutWrite(&u8_RcvChar, 1);
        u8_RcvChar = '\n';
        PutWrite(&u8_RcvChar, 1);
		/* USER CODE END 3BIS */
        au8_ConsoleBuff[u32_ConsoleWrPt++] = '\0';
        // execute the command
        if (strncmp((char*)au8_ConsoleBuff, CMD_HELP, (sizeof(CMD_HELP) - 1)) == 0)
        {
          bDisableDebugLog = true;
          bRunTest = true;
          u32RunTestStart = HAL_GetTick();
          sprintf((char*)(au8_ConsoleBuff), CMD_HELP" : Display this help\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_ABOUT" : Display info about the device\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));

#if (OneWire2Test != 0)
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_TEST_OW"x : Check if a device is found over 1-Wire link x (x is optionnal)\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
#endif

#if (PCF21292Test != 0) || (RTC2Test != 0) || (RX8111CE2Test != 0)
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_RTC_UPDATE" : Set date/time by timestamp\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
#endif

#if (FlashAT25ToTest != 0) || (FlashSST25ToTest != 0) || (FlashW25ToTest != 0)
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_TEST_FLASH" : Check the JDEC flash ID\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
#endif
          /* USER CODE BEGIN 4 */
          
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_TEST_LED"x,y : Set Led X at value Y\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_TEST_IN_D"x : Wait until Digital Input X level change from 0 to 1 (set X at 0 to get the input list)\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_CHECK_IN_D"x,y : Check if Digital Input X is at level Y\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_GET_IN_D"x : Return value of Digital Input X (set X at 0 to get the input list)\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_TEST_OUT_D"x,y : Set Digital Output X at value Y (set X at 0 to get the output list)\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_TEST_CAN"x : Get last frame received over CAN x (1 to 3, 0 for test all CAN)\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_GET_IN_A"x : Return value of Analog Input X (set X at 0 to get the input list)\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_GET_RC" : Return the duty cycle of input CP\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          
          /* USER CODE END 4 */

          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_TEMPO"x : do a delay of x seconds\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_TEST_UC_ID" : return the device UID of uC\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          sprintf((char*)(au8_ConsoleBuff), TEST_CMD_HEADER CMD_TEST_EXIT" : Exit test mode\r\n");
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
        }
        else if (CheckTestCmd(CMD_ABOUT))
        {
          bDisableDebugLog = true;
          bRunTest = true;
          u32RunTestStart = HAL_GetTick();

          /* USER CODE BEGIN 5 */
          sprintf((char*)(au8_ConsoleBuff), "BMS Master V%d.%d - %s - %s\r\n",
            VERSION_BMS_MASTER_MAJ, VERSION_BMS_MASTER_MIN, __DATE__, __TIME__);
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          sprintf((char*)(au8_ConsoleBuff), "Bootloader V%08X\r\n", (unsigned int)u32_BootVersion);
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));

          /* USER CODE END 5 */

          PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
        }

#if (PCF21292Test != 0) || (RTC2Test != 0) || (RX8111CE2Test != 0)
        else if (CheckTestCmd(CMD_RTC_UPDATE))
        {
          bRunTest = true;
          bDisableDebugLog = true;
          u32RunTestStart = HAL_GetTick();

          pEnd = SetCharTestPointer(CMD_RTC_UPDATE);
          pEnd++; // skip '='
          uint32_t u32_TimeStamp = strtoul(pEnd, NULL, 10);
          if (RTC_SetDateTimeByTimeStamp(u32_TimeStamp) == false)
			PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
          else
			PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
        }
#endif

#if (OneWire2Test != 0)
        else if (CheckTestCmd(CMD_TEST_OW))
        {
          bRunTest = true;
          bDisableDebugLog = true;
          u32RunTestStart = HAL_GetTick();
          pEnd = SetCharTestPointer(CMD_TEST_OW);
          uint32_t u32_OneWireLink = strtoul(pEnd, NULL, 10);
          ow_handler_s* phOW = NULL;
          uart_s* poUart = NULL;
          OWI_DS18B20Device* poSondeToRead = NULL;
          switch (u32_OneWireLink)
          {
            /* USER CODE BEGIN OneWire Test */
          case 0:
            phOW = &hOW1;
            poUart = &s_usartx;
            poSondeToRead = oSondeToRead_ow1;
            break;
            // Faire ici le test des eventuel autres bus OneWire

          default:
            break;
            /* USER CODE END OneWire Test */
          }

          if ((phOW != NULL) && (poUart != NULL) && (poSondeToRead != NULL))
          {
            OneWire_GetAllDevice(u32_OneWireLink, phOW, poUart, MAX_1WIRE, poSondeToRead, 200, 4);
            sprintf((char*)(au8_ConsoleBuff), "rom_found=%d\r\n", phOW->rom_found);

            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            if (phOW->rom_found == 0)
			  PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
            else
            {
              ReinitTime4OneWire(phOW);
              while (ReadTemperature_Blocked(phOW) == false)
			  {
				WatchDogKick();
			  }
              for (i = 0; i < phOW->rom_found; i++)
              {
                sprintf((char*)(au8_ConsoleBuff),
                  "Device %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X => %d.%d degrees\r\n",
                  (unsigned)phOW->poSondeToRead[i].OW.rom.rom[0],
                  (unsigned)phOW->poSondeToRead[i].OW.rom.rom[1],
                  (unsigned)phOW->poSondeToRead[i].OW.rom.rom[2],
                  (unsigned)phOW->poSondeToRead[i].OW.rom.rom[3],
                  (unsigned)phOW->poSondeToRead[i].OW.rom.rom[4],
                  (unsigned)phOW->poSondeToRead[i].OW.rom.rom[5],
                  (unsigned)phOW->poSondeToRead[i].OW.rom.rom[6],
                  (unsigned)phOW->poSondeToRead[i].OW.rom.rom[7],
                  (int)phOW->poSondeToRead[i].fTemperature,
                  (int)((phOW->poSondeToRead[i].fTemperature * 1000.0f) - (((int)phOW->poSondeToRead[i].fTemperature) * 1000)));
                PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              }
              PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
            }
          }
          else
          {
			PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
          }
        }
#endif

#if (FlashAT25ToTest != 0) || (FlashSST25ToTest != 0) || (FlashW25ToTest != 0) || (FlashManagerToTest != 0)
        else if (CheckTestCmd(CMD_TEST_FLASH))
        {
          bRunTest = true;
          u32RunTestStart = HAL_GetTick();
          bDisableDebugLog = true;
#if (FlashManagerToTest != 0)
          uint32_t u32_JDEC = oFlash.FlashReadDevID();
#else
          uint32_t u32_JDEC = FlashReadDevID();
#endif
          sprintf((char*)au8_ConsoleBuff, "JDEC=%02X-%02X-%02X\r\n",
            (unsigned int)((u32_JDEC & 0x00FF0000) >> 16),
            (unsigned int)((u32_JDEC & 0x0000FF00) >> 8),
            (unsigned int)(u32_JDEC & 0x000000FF));
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
#if (FlashManagerToTest != 0)
          if (FlashGetType() != Flash_None)
#elif (FlashAT25ToTest != 0)
          if (u32_JDEC == ADESTO_AT25_DEVID)
#elif (FlashSST25ToTest != 0)
          if (u32_JDEC == SST25_DEVID)
#elif (FlashW25ToTest != 0)
          if (u32_JDEC == W25Q01JV_DEVID)
#else
#error "Unknown flash type !!"
#endif
          {
			PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
          }
          else
          {
			PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
          }
        }
#endif

        /* USER CODE BEGIN 6 */

        else if(CheckTestCmd(CMD_TEST_LED))
        {
          bRunTest = true;
          bDisableDebugLog = true;
          u32RunTestStart = HAL_GetTick();
          uint8_t u8_LedVal = 0;
          pEnd = SetCharTestPointer(CMD_TEST_LED);
          if (*pEnd == 'A') // All Output
          {
            pEnd += 2; // skipp A and coma
            u8_LedVal = strtoul(pEnd, &pEnd, 10);

            if (u8_LedVal == 0)
            {
              ResetOut(Led_Verte);
              ResetOut(Led_Rouge);
            }
            else
            {
              SetOut(Led_Verte);
              SetOut(Led_Rouge);
            }
            PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
          }
          else
          {
            uint8_t u8_LedNum = strtoul(pEnd, &pEnd, 10);
            pEnd++; // skipp comma
            u8_LedVal = strtoul(pEnd, &pEnd, 10);

            if ((u8_LedNum > 2) || (u8_LedNum <= 0))
            {
              PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
            }
            else
            {
              switch (u8_LedNum)
              {
              case 1:
                if (u8_LedVal == 0) ResetOut(Led_Verte);
                else SetOut(Led_Verte);
                break;
              case 2:
                if (u8_LedVal == 0) ResetOut(Led_Rouge);
                else SetOut(Led_Rouge);
                break;
              }
              PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
            }
          }
        }
        else if(CheckTestCmd(CMD_TEST_IN_D))
        {
          bRunTest = true;
          bDisableDebugLog = true;
          u32RunTestStart = HAL_GetTick();

#define MAX_TIME_FOR_READ_INPUT_MS         30000
          pEnd = SetCharTestPointer(CMD_TEST_IN_D);
          uint8_t u8_InSel = strtoul(pEnd, &pEnd, 10);

          if (u8_InSel > 9)
            PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
          else if (u8_InSel == 0)
          {
            sprintf((char*)(au8_ConsoleBuff), "In select 1 = Etat Verrou Prise\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 2 = Contact Clef\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 3 = HVIL 1\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 4 = HVIL 2\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 5 = HVIL 3\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 6 = TOR 1\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 7 = TOR 2\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 8 = TOR 3\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 9 = TOR 4\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          }
          else
          {
            bool bEnd = false;
            bool bOk = false;

            if (u8_InSel <= 9)
            {
              // Wait unitl no BP pressed
              while (bEnd == false)
              {
                WatchDogKick();
                switch (u8_InSel)
                {
                case 1: DInMacroCheck0(EtatVerrouPrise) break;
                case 2: DInMacroCheck0(ContactClef) break;
                case 3: DInMacroCheck0(HVIL1) break;
                case 4: DInMacroCheck0(HVIL2) break;
                case 5: DInMacroCheck0(HVIL3) break;
                case 6: DInMacroCheck0(Ana_Tor1) break;
                case 7: DInMacroCheck0(Ana_Tor2) break;
                case 8: DInMacroCheck0(Ana_Tor3) break;
                case 9: DInMacroCheck0(Tor4) break;
                default: break;
                }

                if ((HAL_GetTick() - u32RunTestStart) > MAX_TIME_FOR_READ_INPUT_MS) { bEnd = true; bOk = false; }
              }

              if (bOk != false)
              {
                bOk = false;
                bEnd = false;
                while (bEnd == false)
                {
                  WatchDogKick();
                  switch (u8_InSel)
                  {
                  case 1: DInMacroCheck1(EtatVerrouPrise) break;
                  case 2: DInMacroCheck1(ContactClef) break;
                  case 3: DInMacroCheck1(HVIL1) break;
                  case 4: DInMacroCheck1(HVIL2) break;
                  case 5: DInMacroCheck1(HVIL3) break;
                  case 6: DInMacroCheck1(Ana_Tor1) break;
                  case 7: DInMacroCheck1(Ana_Tor2) break;
                  case 8: DInMacroCheck1(Ana_Tor3) break;
                  case 9: DInMacroCheck1(Tor4) break;
                  default: break;
                  }

                  if ((HAL_GetTick() - u32RunTestStart) > MAX_TIME_FOR_READ_INPUT_MS) { bEnd = true; bOk = false; }
                }
              }
            }
            
            if (bOk == false)
              PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
            else
              PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
          }
        }
        else if(CheckTestCmd(CMD_CHECK_IN_D))
        {
          bRunTest = true;
          bDisableDebugLog = true;
          u32RunTestStart = HAL_GetTick();
          bool bInValue;
          bool bRealValue;

          pEnd = SetCharTestPointer(CMD_CHECK_IN_D);
          if (*pEnd == 'A') // All Intput
          {
            pEnd += 2; // skipp A and coma
            bInValue = (strtoul(pEnd, &pEnd, 10) == 0 ? false : true);

            bRealValue = GetIn(EtatVerrouPrise) ||
                          GetIn(ContactClef) ||
                          GetIn(HVIL1) ||
                          GetIn(HVIL2) ||
                          GetIn(HVIL3) ||
                          GetIn(Ana_Tor1) ||
                          GetIn(Ana_Tor2) ||
                          GetIn(Ana_Tor3) ||
                          GetIn(Tor4);
            
            if (bRealValue == bInValue)
              PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
            else
              PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
          }
          else
          {
            uint8_t u8_InSel = strtoul(pEnd, &pEnd, 10);
            pEnd++;
            bInValue = (strtoul(pEnd, &pEnd, 10) == 0 ? false : true);
            bRealValue = false;

            if ((u8_InSel > 9) || (u8_InSel <= 0))
              PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
            else
            {
              switch (u8_InSel)
              {
              case 1: bRealValue = GetIn(EtatVerrouPrise); break;
              case 2: bRealValue = GetIn(ContactClef); break;
              case 3: bRealValue = GetIn(HVIL1); break;
              case 4: bRealValue = GetIn(HVIL2); break;
              case 5: bRealValue = GetIn(HVIL3); break;
              case 6: bRealValue = GetIn(Ana_Tor1); break;
              case 7: bRealValue = GetIn(Ana_Tor2); break;
              case 8: bRealValue = GetIn(Ana_Tor3); break;
              case 9: bRealValue = GetIn(Tor4); break;
              default: break;
              }
              if (bRealValue == bInValue)
                PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
              else
                PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
            }
          }
        }
        else if(CheckTestCmd(CMD_GET_IN_D))
        {
          bRunTest = true;
          bDisableDebugLog = true;
          u32RunTestStart = HAL_GetTick();

          pEnd = SetCharTestPointer(CMD_GET_IN_D);
          uint8_t u8_InSel = strtoul(pEnd, &pEnd, 10);
          bool bReadValue = false;

          if (u8_InSel > 9)
            PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
          else if (u8_InSel == 0)
          {
            sprintf((char*)(au8_ConsoleBuff), "In select 1 = Etat Verrou Prise\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 2 = Contact Clef\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 3 = HVIL 1\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 4 = HVIL 2\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 5 = HVIL 3\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 6 = TOR 1\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 7 = TOR 2\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 8 = TOR 3\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 9 = TOR 4\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          }
          else
          {
            switch (u8_InSel)
            {
            case 1: bReadValue = GetIn(EtatVerrouPrise); break;
            case 2: bReadValue = GetIn(ContactClef); break;
            case 3: bReadValue = GetIn(HVIL1); break;
            case 4: bReadValue = GetIn(HVIL2); break;
            case 5: bReadValue = GetIn(HVIL3); break;
            case 6: bReadValue = GetIn(Ana_Tor1); break;
            case 7: bReadValue = GetIn(Ana_Tor2); break;
            case 8: bReadValue = GetIn(Ana_Tor3); break;
            case 9: bReadValue = GetIn(Tor4); break;
            default: break;
            }
            sprintf((char*)(au8_ConsoleBuff), "%d\r\n", bReadValue);
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          }
        }  
        else if(CheckTestCmd(CMD_TEST_OUT_D))
        {
          bRunTest = true;
          bDisableDebugLog = true;
          u32RunTestStart = HAL_GetTick();
          uint8_t u8_OutVal = 0;

          pEnd = SetCharTestPointer(CMD_TEST_OUT_D);
          if (*pEnd == 'A') // All Output
          {
            pEnd += 2; // skipp A and coma
            u8_OutVal = strtoul(pEnd, &pEnd, 10);

            if (u8_OutVal == 0)
            {
              ResetOut(VerrouSens1);
              ResetOut(VerrouSens2);
              ResetOut(ModifImpedanceCP);
              ResetOut(CdeSortieReserve);
              ResetOut(CdeSortieReserve2);
              ResetOut(CdeSortieReserve3);
              ResetOut(CdeSortieReserve4);
              ResetOut(CdeAlimCapteurCourant);
              ResetOut(CdeConvertisseurDCDC);
              ResetOut(CdeAlimBmsSlave);
              ResetOut(CdeRelaisDePuissance);
              ResetOut(TestIsolementPlus);
              ResetOut(TestIsolementMoins);
            }
            else
            {
              SetOut(VerrouSens1);
              SetOut(VerrouSens2);
              SetOut(ModifImpedanceCP);
              SetOut(CdeSortieReserve);
              SetOut(CdeSortieReserve2);
              SetOut(CdeSortieReserve3);
              SetOut(CdeSortieReserve4);
              SetOut(CdeAlimCapteurCourant);
              SetOut(CdeConvertisseurDCDC);
              SetOut(CdeAlimBmsSlave);
              SetOut(CdeRelaisDePuissance);
              SetOut(TestIsolementPlus);
              SetOut(TestIsolementMoins);
            }
            PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
          }
          else
          {
            uint8_t u8_OutSel = strtoul(pEnd, &pEnd, 10);
            pEnd++; // skipp coma
            u8_OutVal = strtoul(pEnd, &pEnd, 10);

            if (u8_OutSel > 13)
              PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
            else if (u8_OutSel == 0)
            {
              sprintf((char*)(au8_ConsoleBuff), "Out select 1 = Verrou Sens 1\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              sprintf((char*)(au8_ConsoleBuff), "Out select 2 = Verrou Sens 2\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              sprintf((char*)(au8_ConsoleBuff), "Out select 3 = Modif Impedance CP\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              sprintf((char*)(au8_ConsoleBuff), "Out select 4 = Cde Sortie Reserve\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              sprintf((char*)(au8_ConsoleBuff), "Out select 5 = Cde Sortie Reserve 2\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              sprintf((char*)(au8_ConsoleBuff), "Out select 6 = Cde Sortie Reserve 3\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              sprintf((char*)(au8_ConsoleBuff), "Out select 7 = Cde Sortie Reserve 4\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              sprintf((char*)(au8_ConsoleBuff), "Out select 8 = Cde Alim Capteur Courant\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              sprintf((char*)(au8_ConsoleBuff), "Out select 9 = Cde Convertisseur DCDC\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              sprintf((char*)(au8_ConsoleBuff), "Out select 10 = Cde Alim Bms Slave\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              sprintf((char*)(au8_ConsoleBuff), "Out select 11 = Cde Relais De Puissance\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              sprintf((char*)(au8_ConsoleBuff), "Out select 12 = Test Isolement Plus\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
              sprintf((char*)(au8_ConsoleBuff), "Out select 13 = Test Isolement Moins\r\n");
              PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            }
            else
            {
              switch (u8_OutSel)
              {
              case 1:
                if (u8_OutVal == 0) ResetOut(VerrouSens1);
                else SetOut(VerrouSens1);
                break;
              case 2:
                if (u8_OutVal == 0) ResetOut(VerrouSens2);
                else SetOut(VerrouSens2);
                break;
              case 3:
                if (u8_OutVal == 0) ResetOut(ModifImpedanceCP);
                else SetOut(ModifImpedanceCP);
                break;
              case 4:
                if (u8_OutVal == 0) ResetOut(CdeSortieReserve);
                else SetOut(CdeSortieReserve);
                break;
              case 5:
                if (u8_OutVal == 0) ResetOut(CdeSortieReserve2);
                else SetOut(CdeSortieReserve2);
                break;
              case 6:
                if (u8_OutVal == 0) ResetOut(CdeSortieReserve3);
                else SetOut(CdeSortieReserve3);
                break;
              case 7:
                if (u8_OutVal == 0) ResetOut(CdeSortieReserve4);
                else SetOut(CdeSortieReserve4);
                break;
              case 8:
                if (u8_OutVal == 0) ResetOut(CdeAlimCapteurCourant);
                else SetOut(CdeAlimCapteurCourant);
                break;
              case 9:
                if (u8_OutVal == 0) ResetOut(CdeConvertisseurDCDC);
                else SetOut(CdeConvertisseurDCDC);
                break;
              case 10:
                if (u8_OutVal == 0) ResetOut(CdeAlimBmsSlave);
                else SetOut(CdeAlimBmsSlave);
                break;
              case 11:
                if (u8_OutVal == 0) ResetOut(CdeRelaisDePuissance);
                else SetOut(CdeRelaisDePuissance);
                break;
              case 12:
                if (u8_OutVal == 0) ResetOut(TestIsolementPlus);
                else SetOut(TestIsolementPlus);
                break;
              case 13:
                if (u8_OutVal == 0) ResetOut(TestIsolementMoins);
                else SetOut(TestIsolementMoins);
                break;
              }
              PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
            }
          }
        }       
        else if(CheckTestCmd(CMD_TEST_CAN))
        {
          bRunTest = true;
          bDisableDebugLog = true;
          u32RunTestStart = HAL_GetTick();

          pEnd = SetCharTestPointer(CMD_TEST_CAN);
          
          uint8_t u8_CanSel = strtoul(pEnd, &pEnd, 10);
          uint8_t u8_StartCan = 0;
          uint8_t u8_EndCan = 0;
          if ((u8_CanSel > 3) || (u8_CanSel < 0))
            PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
          else
          {
            if (u8_CanSel == 0)
            {
              u8_StartCan = 1;
              u8_EndCan = 3;
            }
            else
            {
              u8_StartCan = u8_CanSel;
              u8_EndCan = u8_CanSel;
            }
            
            for (i = u8_StartCan; i <= u8_EndCan; i++)
            {
              aoLastCanMsg[i].u1_Valid = 0;
            }

            // Do a delay to let CANAlyzer see some frames
            HAL_Delay(2500);

            uint8_t au8FrameCounter[MAX_NUM_CAN] = { 0 };
            for (i = u8_StartCan; i <= u8_EndCan; i++)
            {
              if ((aoLastCanMsg[i].u29_ID != 0) && (aoLastCanMsg[i].u1_Valid != 0))
              {
                sprintf((char*)au8_ConsoleBuff, "RX CAN%d from %04X [%d] :\r\n",
                  (int)i, (unsigned int)aoLastCanMsg[i].u29_ID, aoLastCanMsg[i].u8_DataLen);
                PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
                for (int k = 0; k < aoLastCanMsg[i].u8_DataLen; k++)
                {
                  sprintf((char*)au8_ConsoleBuff, "%02X ", aoLastCanMsg[i].au8_Data[k]);
                  PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
                }
                sprintf((char*)au8_ConsoleBuff, "\r\n");
                PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
                aoLastCanMsg[i].u29_ID = 0;
                au8FrameCounter[i]++;
              }
            }

            if (((u8_CanSel != 0) && (au8FrameCounter[u8_CanSel - 1] == 0)) ||
              ((u8_CanSel == 0) && (au8FrameCounter[0] == 0) && (au8FrameCounter[1] == 0) && (au8FrameCounter[2] == 0)))
              PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
            else
              PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
          }
        }
        else if(CheckTestCmd(CMD_GET_IN_A))
        {
          bRunTest = true;
          bDisableDebugLog = true;
          u32RunTestStart = HAL_GetTick();

          pEnd = SetCharTestPointer(CMD_GET_IN_A);
          uint8_t u8_InSel = strtoul(pEnd, &pEnd, 10);

          if (u8_InSel > NUM_ADC_INPUTS)
            PutWrite((uint8_t*)CMD_TEST_KO, strlen(CMD_TEST_KO));
          else if (u8_InSel == 0)
          {
            sprintf((char*)(au8_ConsoleBuff), "In select 1 = Mesure Batt Service\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 2 = Low Current Range\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 3 = High Current Range\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 4 = Mesure Rc Cable\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 5 = Tension Pack Accus\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 6 = Mesure Fuite 1\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
            sprintf((char*)(au8_ConsoleBuff), "In select 7 = Mesure Fuite 2\r\n");
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          }
          else
          {
            sprintf((char*)(au8_ConsoleBuff), "%d\r\n", au16_AdcFilteredValues[u8_InSel]);
            PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
          }
        }
        else if(CheckTestCmd(CMD_GET_RC))
        {
          bRunTest = true;
          bDisableDebugLog = true;
          u32RunTestStart = HAL_GetTick();
          
          sprintf((char*)(au8_ConsoleBuff), "%f\r\n", f_RapportCyclique);
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
        }

        /* USER CODE END 6 */
        else if (CheckTestCmd(CMD_TEMPO))
        {
          bRunTest = true;
          bDisableDebugLog = true;
          u32RunTestStart = HAL_GetTick();
          pEnd = SetCharTestPointer(CMD_TEMPO);
          uint8_t u8_Tempo_Seconds = strtoul(pEnd, &pEnd, 10);
          HAL_Delay(u8_Tempo_Seconds * 1000);
          PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
        }
        else if (CheckTestCmd(CMD_TEST_UC_ID))
        {
          bRunTest = true;
          bDisableDebugLog = true;
          u32RunTestStart = HAL_GetTick();
#if defined(STM32G4)
          sprintf((char*)(au8_ConsoleBuff), "%08X%08X%08X\r\n", (unsigned int)LL_GetUID_Word0(), (unsigned int)LL_GetUID_Word1(), (unsigned int)LL_GetUID_Word2());
#else
          sprintf((char*)(au8_ConsoleBuff), "%08X%08X%08X\r\n", (unsigned int)HAL_GetUIDw0(), (unsigned int)HAL_GetUIDw1(), (unsigned int)HAL_GetUIDw2());
#endif
          PutWrite(au8_ConsoleBuff, strlen((char*)au8_ConsoleBuff));
        }
        else if (CheckTestCmd(CMD_TEST_EXIT))
        {
          bRunTest = false;
          bDisableDebugLog = false;
          PutWrite((uint8_t*)CMD_TEST_OK, strlen(CMD_TEST_OK));
          HAL_Delay(200);
        }

        // send prompt
#ifndef DISABLE_PROMPT
		if (bRunTest != false)
			PutWrite((uint8_t*)CMD_PROMPT, strlen(CMD_PROMPT));
#endif

        // clear the previous command
        u32_ConsoleWrPt = 0;
      /* USER CODE BEGIN End loop */
      memset(au8_ConsoleBuff, 0, CONSOLE_BUFF_SIZE);
      /* USER CODE END End loop */
      }
      else if (u32_ConsoleWrPt >= CONSOLE_BUFF_SIZE)
      {
        u32_ConsoleWrPt = 0;
      }
    }
  }
#if (TIMEOUT_MODE != 0)
  bRunTest = false;
  bDisableDebugLog = false;
#endif
/* USER CODE BEGIN 7 */

/* USER CODE END 7 */
}

/* USER CODE BEGIN 8 */

/* USER CODE END 8 */

