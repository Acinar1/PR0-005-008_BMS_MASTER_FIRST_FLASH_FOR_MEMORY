/**
  ******************************************************************************
  * File Name          : UpDate.h
  * Version            : 1.0
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */
#pragma once

#include <stdint.h>

/* USER CODE BEGIN UPDATE Include */

/* USER CODE END UPDATE Include */

typedef struct __attribute__((packed))
{
  uint32_t u32_ValidityKey;
  uint32_t u32_BinaryFileSize;
  uint32_t u32_BinaryFileCRC32;
  union {
    uint32_t u32_SoftwareVersion;
    uint8_t au8_SoftwareVersion[4];
  };
  uint32_t u32_ProductID;
  uint32_t u32_HardwareVersion;
  uint32_t u32_CpuID;
  uint32_t u32_Spare;
}UpDateFileHeader_t;

/* USER CODE BEGIN UPDATE Variables */

/* USER CODE END UPDATE Variables */

void UpDatePrepare(void);
int8_t Updating(
	uint32_t u32_SizeOfUpdate
  /* USER CODE BEGIN UPDATE Prototype */

  /* USER CODE END UPDATE Prototype */
	);

int32_t CheckUpDateHeader(UpDateFileHeader_t oUpDate, uint32_t u32_FirmSize
  /* USER CODE BEGIN CheckUpdate Prototype */

  /* USER CODE END CheckUpdate Prototype */
);

void ClearUpdate(void);

/* USER CODE BEGIN UPDATE 1 */

/* USER CODE END UPDATE 1 */

