/**
  ******************************************************************************
  * File Name          : UpDate.c
  * Version            : 1.0
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "crc.h"
#include "UpDate.h"

/* USER CODE BEGIN UPDATE include */

/* USER CODE END UPDATE include */

/*
 * UpDate File Header description
 */


 /* USER CODE BEGIN UPDATE FirmAddr */
#define ADDR_NewFirmwareHeader            NEW_FIRMWARE_ADDR
/* USER CODE END UPDATE FirmAddr */
#define ADDR_NewFirmwareBinary            (ADDR_NewFirmwareHeader + sizeof(UpDateFileHeader_t))

/* USER CODE BEGIN UPDATE Private defines */
#define SIZE_BUFF_UPDATE                  4096

/* USER CODE END UPDATE Private defines */

/* USER CODE BEGIN UPDATE Private typedef */

/* USER CODE END UPDATE Private typedef */

uint8_t au8_Buff4UpDate[SIZE_BUFF_UPDATE];
uint32_t u32_Buff4UpDatePt = 0;

/* USER CODE BEGIN UPDATE Private variables */

/* USER CODE END UPDATE Private variables */

/* local functions prototypes */

/* USER CODE BEGIN UPDATE Private function prototypes */

/* USER CODE END UPDATE Private function prototypes */

/* functions */

/* USER CODE BEGIN UPDATE 0 */

/* USER CODE END UPDATE 0 */

void UpDatePrepare(void)
{
  uint32_t u32_Addr;
/* USER CODE BEGIN UPDATE 1 */

/* USER CODE END UPDATE 1 */
}

int8_t Updating(
	uint32_t u32_SizeOfUpdate
  /* USER CODE BEGIN UPDATE 2 */

  /* USER CODE END UPDATE 2 */
	)
{
  uint32_t u32_SizeFirmware = 0;
  UpDateFileHeader_t oUpdate;

  /* USER CODE BEGIN UPDATE 2bis */
  uint32_t u32_DataAlreadyWrite = 0;
  uint8_t u8Step = 0;

  /* USER CODE END UPDATE 2bis */
  u32_Buff4UpDatePt = 0;

  while (u32_SizeFirmware < u32_SizeOfUpdate)
  {
    /* USER CODE BEGIN UPDATE 3 */

    /* USER CODE END UPDATE 3 */
  }
  // store last data to flash
  if (u32_Buff4UpDatePt > 0)
  {
    /* USER CODE BEGIN UPDATE 4 */

    /* USER CODE END UPDATE 4 */
  }

  /* USER CODE BEGIN UPDATE 5 */
  // All is received => Get file header
  /* USER CODE END UPDATE 5 */

  int32_t i32Ret = CheckUpDateHeader(oUpdate, u32_SizeFirmware
    /* USER CODE BEGIN CheckUpdateCall 0 */
    /* USER CODE END CheckUpdateCall 0 */
  );

/* USER CODE BEGIN UPDATE 6 */
  return i32Ret;
/* USER CODE END UPDATE 6 */
}

int32_t CheckUpDateHeader(UpDateFileHeader_t oUpDate, uint32_t u32_FirmSize
  /* USER CODE BEGIN CheckUpdate 0 */

  /* USER CODE END CheckUpdate 0 */
  )
{
  uint32_t u32_Crc32 = 0;

  /* USER CODE BEGIN CheckUpdate 1 */

  /* USER CODE END CheckUpdate 1 */

  // check firmware size
  /* USER CODE BEGIN CheckUpdate 2 */
  if ((u32_FirmSize - sizeof(UpDateFileHeader_t)) != oUpDate.u32_BinaryFileSize)
  {
    return -1;
  }
  /* USER CODE END CheckUpdate 2 */

  /* USER CODE BEGIN CheckUpdate 3 */
  // check validity key
  if (oUpDate.u32_ValidityKey != UpDateFileValidityKey)
  {
    return -2;
  }
  /* USER CODE END CheckUpdate 3 */

  /* USER CODE BEGIN CheckUpdate 4 */
  // Check Product ID
  if (oUpDate.u32_ProductID != ProductID)
  {
    return -3;
  }
  /* USER CODE END CheckUpdate 4 */

  /* USER CODE BEGIN CheckUpdate 5 */
  // Check CPU ID
  if (oUpDate.u32_CpuID != CpuID)
  {
    return -4;
  }
  /* USER CODE END CheckUpdate 5 */

  /* USER CODE BEGIN CheckUpdate 6 */

  /* USER CODE END CheckUpdate 6 */

  // check CRC32
  /* Reset CRC Calculation Unit (hcrc->Instance->INIT is
  *  written in hcrc->Instance->DR) */
  __HAL_CRC_DR_RESET(&hcrc);
  u32_Crc32 = 0;
  u32_FirmSize = 0;
  while (u32_FirmSize < oUpDate.u32_BinaryFileSize)
  {
    /* USER CODE BEGIN CheckUpdate 7 */

    /* USER CODE END CheckUpdate 7 */
    if ((oUpDate.u32_BinaryFileSize - u32_FirmSize) > SIZE_BUFF_UPDATE)
      u32_Buff4UpDatePt = SIZE_BUFF_UPDATE;
    else
      u32_Buff4UpDatePt = oUpDate.u32_BinaryFileSize - u32_FirmSize;
    /* USER CODE BEGIN CheckUpdate 8 */
    /* USER CODE END CheckUpdate 8 */
    u32_Crc32 = HAL_CRC_Accumulate(&hcrc, (uint32_t*)au8_Buff4UpDate, u32_Buff4UpDatePt);
    u32_FirmSize += u32_Buff4UpDatePt;
  }
  u32_Crc32 = (u32_Crc32 ^ 0xffffffffL);

  if (oUpDate.u32_BinaryFileCRC32 != u32_Crc32)
  {
    /* USER CODE BEGIN CheckUpdate 9 */
    /* USER CODE END CheckUpdate 9 */
    return -5;
  }

  /* USER CODE BEGIN CheckUpdate 10 */

  /* USER CODE END CheckUpdate 10 */

  return 0;
}

void ClearUpdate(void)
{
  /* USER CODE BEGIN UPDATE 14 */
  UpDateFileHeader_t oClearUpdate = { 0 };
  /* USER CODE END UPDATE 14 */
}

/* USER CODE BEGIN UPDATE 15 */

/* USER CODE END UPDATE 15 */

