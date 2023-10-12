/**
  ******************************************************************************
  * File Name          : SST25VF080.h
  * Version            : 1.0
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define SST25_DEVID       0x00BF258E

#define SST25_SIZE_SECTOR      				(1024u*4u)    // secteur de 4ko

/* USER CODE BEGIN define */
#include "main.h"
// Default generic Flash size redefinition
#define SIZE_SECTOR							SST25_SIZE_SECTOR


// les premiers 512ko sont utilisable librement
// les derniers 512ko sont utilises pour l'enregistrement du nouveau firmware
#define NEW_FIRMWARE_ADDR       FlashAddrOfNewFirmware  // Nouveau firmware telecharge enregistre dans la seconde partie de la flash



/* USER CODE END define */

// initialize flash
void FlashSST25Init(void);
// get the JDEC value
uint32_t FlashSST25ReadDevID(void);
// erase a sector
void FlashSST25EraseAll(void);
void FlashSST25EraseBlock64K(uint32_t u32_Addr);
void FlashSST25EraseBlock32K(uint32_t u32_Addr);
void FlashSST25EraseBlock4K(uint32_t u32_Addr);
// Read data from flash at a specified address
uint32_t FlashSST25Read(uint32_t u32_Addr, uint8_t* pu8_data, uint32_t u32_Len);
// write data to flash at a specified address and erase sector if address is a boundary sector address
void FlashSST25Write(uint32_t u32_Addr, uint8_t* pu8_data, uint32_t u32_Len);
// write data to flash at a specified address without erase of sector
void FlashSST25WriteWOErase(uint32_t u32_Addr, uint8_t* pu8_data, uint32_t u32_Len);

bool FlashSST25IsBusy(void);

/* USER CODE BEGIN 1 */

// Default generic Flash call function redefinition
#define FlashInit               FlashSST25Init
#define FlashReadDevID          FlashSST25ReadDevID
#define FlashEraseAll           FlashSST25EraseAll
#define FlashEraseBlock64K      FlashSST25EraseBlock64K
#define FlashEraseBlock32K      FlashSST25EraseBlock32K
#define FlashEraseBlock4K       FlashSST25EraseBlock4K
#define FlashRead               FlashSST25Read
#define FlashWrite              FlashSST25Write
#define FlashWriteWOErase       FlashSST25WriteWOErase
#define FlashIsBusy             FlashSST25IsBusy

/* USER CODE END 1 */

