/**
  ******************************************************************************
  * File Name          : SST25VF080.c
  * Version            : 1.0
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "spi.h"
#include "SST25VF080.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/**
  * Definir le port et le numero de pin du ChipSelect de la flash
  * Definir le handler de la SPI utilise par la flash
  */
/* USER CODE BEGIN Private defines */
#define CS_PORT                         Flash_CS_GPIO_Port
#define CS_PIN                          Flash_CS_Pin
#define FLASH_SPI_HANDLER               &hspi1
/* USER CODE END Private defines */

/* USER CODE BEGIN Private variables */

/* USER CODE END Private variables */

/* USER CODE BEGIN FLASH_Transmit 0 */
HAL_StatusTypeDef FlashSST25SPI_Transmit(SPI_HandleTypeDef* hspi, uint8_t* pData, uint16_t Size, uint32_t Timeout);
/* USER CODE END FLASH_Transmit 0 */

/* USER CODE BEGIN FLASH_Receive 0 */
HAL_StatusTypeDef FlashSST25SPI_Receive(SPI_HandleTypeDef* hspi, uint8_t* pData, uint16_t Size, uint32_t Timeout);
/* USER CODE END FLASH_Receive 0 */

/* USER CODE BEGIN FLASH_TransmitReceive 0 */
HAL_StatusTypeDef FlashSST25SPI_TransmitReceive(SPI_HandleTypeDef* hspi, uint8_t* pTxData, uint8_t* pRxData, uint16_t Size, uint32_t Timeout);
/* USER CODE END FLASH_TransmitReceive 0 */

/* USER CODE BEGIN Private function prototypes */

/* USER CODE END Private function prototypes */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* USER CODE BEGIN FLASH_Transmit 1 */
HAL_StatusTypeDef FlashSST25SPI_Transmit(SPI_HandleTypeDef* hspi, uint8_t* pData, uint16_t Size, uint32_t Timeout)
{
  uint32_t u32_TickStart = HAL_GetTick();
  bUserSPITxNotEnded = true;
  HAL_StatusTypeDef Ret = HAL_SPI_Transmit_DMA(hspi, pData, Size);
  while (bUserSPITxNotEnded)
  {
    if ((HAL_GetTick() - u32_TickStart) > Timeout)
    {
      Ret = HAL_TIMEOUT;
      break;
    }
  }

  HAL_SPI_DMAStop(hspi);
  bUserSPITxNotEnded = false;

  HAL_SPIEx_FlushRxFifo(hspi);

  return Ret;
}
/* USER CODE END FLASH_Transmit 1 */

/* USER CODE BEGIN FLASH_Receive 1 */
HAL_StatusTypeDef FlashSST25SPI_Receive(SPI_HandleTypeDef* hspi, uint8_t* pData, uint16_t Size, uint32_t Timeout)
{
  uint32_t u32_TickStart = HAL_GetTick();
  bUserSPIRxNotEnded = true;
  HAL_StatusTypeDef Ret = HAL_SPI_Receive_DMA(hspi, pData, Size);
  while (bUserSPIRxNotEnded)
  {
    if ((HAL_GetTick() - u32_TickStart) > Timeout)
    {
      Ret = HAL_TIMEOUT;
      break;
    }
  }

  HAL_SPI_DMAStop(hspi);
  bUserSPIRxNotEnded = false;

  return Ret;
}
/* USER CODE END FLASH_Receive 1 */

/* USER CODE BEGIN FLASH_TransmitReceive 1 */
HAL_StatusTypeDef FlashSST25SPI_TransmitReceive(SPI_HandleTypeDef* hspi, uint8_t* pTxData, uint8_t* pRxData, uint16_t Size, uint32_t Timeout)
{
  uint32_t u32_TickStart = HAL_GetTick();
  bUserSPITxNotEnded = true;
  bUserSPIRxNotEnded = true;
  HAL_StatusTypeDef Ret = HAL_SPI_TransmitReceive_DMA(hspi, pTxData, pRxData, Size);
  while (bUserSPITxNotEnded || bUserSPIRxNotEnded)
  {
    if ((HAL_GetTick() - u32_TickStart) > Timeout)
    {
      Ret = HAL_TIMEOUT;
      break;
    }
  }

  HAL_SPI_DMAStop(hspi);
  bUserSPITxNotEnded = false;
  bUserSPIRxNotEnded = false;

  return Ret;
}
/* USER CODE END FLASH_TransmitReceive 1 */

uint8_t SST25_Read_Status_Register()
{
  uint8_t au8_Cmd[2] = { 0x05, 0 };/* send RDSR command */
  uint8_t au8_Resp[2] = { 0, 0 };

  CS_PORT->BSRR = CS_PIN << 16;        /* enable device */
  FlashSST25SPI_TransmitReceive(FLASH_SPI_HANDLER, au8_Cmd, au8_Resp, 2, 10);
  CS_PORT->BSRR = CS_PIN;        /* disable device */
  return au8_Resp[1];
}

void SST25_Sector_Erase(uint32_t u32_sector_addr)
{
  uint8_t au8_Cmd[4] = { 0x20, 0, 0, 0 }; /* send Sector Erase command */

  au8_Cmd[1] = (((u32_sector_addr & 0xFFFFFF) >> 16));   /* send 3 address bytes */
  au8_Cmd[2] = (((u32_sector_addr & 0xFFFF) >> 8));
  au8_Cmd[3] = (u32_sector_addr & 0xFF);

  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  FlashSST25SPI_Transmit(FLASH_SPI_HANDLER, au8_Cmd, 4, 10);
  CS_PORT->BSRR = CS_PIN;          /* disable device */
}

void SST25_Byte_Program(uint32_t Dst, uint8_t byte)
{
  uint8_t au8_Cmd[5] = { 0x02, 0, 0, 0, 0 }; /* send Byte Program command */

  au8_Cmd[1] = (((Dst & 0xFFFFFF) >> 16));  /* send 3 address bytes */
  au8_Cmd[2] = (((Dst & 0xFFFF) >> 8));
  au8_Cmd[3] = (Dst & 0xFF);
  au8_Cmd[4] = (byte);      /* send byte to be programmed */

  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  FlashSST25SPI_Transmit(FLASH_SPI_HANDLER, au8_Cmd, 5, 10);
  CS_PORT->BSRR = CS_PIN;          /* disable device */
}

void SST25_Wait_Busy()
{
  while ((SST25_Read_Status_Register() & 0x03) == 0x03)  /* waste time until not busy */
    SST25_Read_Status_Register();
}

void SST25_Wait_Busy_AAI()
{
  while ((SST25_Read_Status_Register() & 0x43) == 0x43)  /* waste time until not busy */
    SST25_Read_Status_Register();
}

void SST25_EWSR()
{
  uint8_t u8_Cmd = 0x50;    /* enable writing to the status register */

  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  FlashSST25SPI_Transmit(FLASH_SPI_HANDLER, &u8_Cmd, 1, 10);
  CS_PORT->BSRR = CS_PIN;        /* disable device */
}

void SST25_WRSR(uint8_t byte)
{
  uint8_t au8_Cmd[2] = { 0x01, 0, }; /* select write to status register */

  au8_Cmd[1] = byte;

  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  FlashSST25SPI_Transmit(FLASH_SPI_HANDLER, au8_Cmd, 2, 10);
  CS_PORT->BSRR = CS_PIN;        /* disable the device */
}

void SST25_WREN()
{
  uint8_t u8_Cmd = 0x06;    /* send WREN command */

  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  FlashSST25SPI_Transmit(FLASH_SPI_HANDLER, &u8_Cmd, 1, 10);
  CS_PORT->BSRR = CS_PIN;        /* disable device */
}

void SST25_WRDI()
{
  uint8_t u8_Cmd = 0x04;    /* send WRDI command */

  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  FlashSST25SPI_Transmit(FLASH_SPI_HANDLER, &u8_Cmd, 1, 10);
  CS_PORT->BSRR = CS_PIN;        /* disable device */
}

void SST25_Auto_Add_IncA(uint32_t Dst, uint8_t byte1, uint8_t byte2)
{
  uint8_t au8_Cmd[6] = { 0xAD, 0, 0, 0, 0 , 0}; /* send AAI command */

  au8_Cmd[1] = (((Dst & 0xFFFFFF) >> 16));   /* send 3 address bytes */
  au8_Cmd[2] = (((Dst & 0xFFFF) >> 8));
  au8_Cmd[3] = (Dst & 0xFF);
  au8_Cmd[4] = (byte1);      /* send 1st byte to be programmed */
  au8_Cmd[5] = (byte2);      /* send 2nd byte to be programmed */

  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  FlashSST25SPI_Transmit(FLASH_SPI_HANDLER, au8_Cmd, 6, 10);
  CS_PORT->BSRR = CS_PIN;          /* disable device */
}

void SST25_Auto_Add_IncB(uint8_t byte1, uint8_t byte2)
{
  uint8_t au8_Cmd[3] = { 0xAD, 0, 0 }; /* send AAI command */

  au8_Cmd[1] = (byte1);      /* send 1st byte to be programmed */
  au8_Cmd[2] = (byte2);      /* send 2nd byte to be programmed */

  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  FlashSST25SPI_Transmit(FLASH_SPI_HANDLER, au8_Cmd, 3, 10);
  CS_PORT->BSRR = CS_PIN;          /* disable device */
}

void FlashSST25Init(void)
{
  uint8_t u8_Status;
  CS_PORT->BSRR = CS_PIN;
  u8_Status = SST25_Read_Status_Register();
  SST25_EWSR();
  SST25_WRSR(u8_Status & 0x03);
}

uint32_t FlashSST25ReadDevID(void)
{
  uint32_t u32_temp = 0;
  uint8_t au8_Cmd[4] = { 0x9F, 0, 0, 0 };/* send JEDEC ID command (9Fh) */
  uint8_t au8_Resp[4] = { 0, 0, 0, 0 };

  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  FlashSST25SPI_TransmitReceive(FLASH_SPI_HANDLER, au8_Cmd, au8_Resp, 4, 10);
  CS_PORT->BSRR = CS_PIN;          /* disable device */

  u32_temp = ((uint32_t)au8_Resp[1] << 16) + ((uint32_t)au8_Resp[2] << 8) + au8_Resp[3]; /* temp value = 0xBF258E */

  return u32_temp;
}

void FlashSST25EraseAll(void)
{
  uint8_t u8_Cmd = 0x60;      /* send Sector Erase command */

  SST25_Wait_Busy();
  SST25_WREN();

  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  FlashSST25SPI_Transmit(FLASH_SPI_HANDLER, &u8_Cmd, 1, 10);
  CS_PORT->BSRR = CS_PIN;          /* disable device */
}

void FlashSST25EraseBlock64K(uint32_t u32_Addr)
{
  uint8_t au8_Cmd[4] = { 0xD8, 0, 0, 0 }; /* send Sector Erase command */

  au8_Cmd[1] = (((u32_Addr & 0xFFFFFF) >> 16));   /* send 3 address bytes */
  au8_Cmd[2] = (((u32_Addr & 0xFFFF) >> 8));
  au8_Cmd[3] = (u32_Addr & 0xFF);

  SST25_Wait_Busy();
  SST25_WREN();

  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  FlashSST25SPI_Transmit(FLASH_SPI_HANDLER, au8_Cmd, 4, 10);
  CS_PORT->BSRR = CS_PIN;          /* disable device */
}

void FlashSST25EraseBlock32K(uint32_t u32_Addr)
{
  uint8_t au8_Cmd[4] = { 0x52, 0, 0, 0 }; /* send Sector Erase command */

  au8_Cmd[1] = (((u32_Addr & 0xFFFFFF) >> 16));   /* send 3 address bytes */
  au8_Cmd[2] = (((u32_Addr & 0xFFFF) >> 8));
  au8_Cmd[3] = (u32_Addr & 0xFF);

  SST25_Wait_Busy();
  SST25_WREN();

  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  FlashSST25SPI_Transmit(FLASH_SPI_HANDLER, au8_Cmd, 4, 10);
  CS_PORT->BSRR = CS_PIN;          /* disable device */
}

void FlashSST25EraseBlock4K(uint32_t u32_Addr)
{
  SST25_Wait_Busy();
  SST25_WREN();
  SST25_Sector_Erase(u32_Addr);
}

uint32_t FlashSST25Read(uint32_t u32_Addr, uint8_t* pu8_data, uint32_t u32_Len)
{
  uint32_t u32_ByteRead = 0;
  uint8_t au8_Cmd[4] = { 0x03, 0, 0, 0 }; /* read command */

  au8_Cmd[1] = (((u32_Addr & 0xFFFFFF) >> 16));   /* send 3 address bytes */
  au8_Cmd[2] = (((u32_Addr & 0xFFFF) >> 8));
  au8_Cmd[3] = (u32_Addr & 0xFF);

  if (u32_Len == 0)
    return 0;

  memset(pu8_data, 0, u32_Len);

  SST25_Wait_Busy();
  CS_PORT->BSRR = CS_PIN << 16; /* enable device */
  // transmit the command
  FlashSST25SPI_Transmit(FLASH_SPI_HANDLER, au8_Cmd, 4, 10);
  // read all data
  if (FlashSST25SPI_Receive(FLASH_SPI_HANDLER, pu8_data, u32_Len, 1000) == HAL_OK)
    u32_ByteRead = u32_Len;
  CS_PORT->BSRR = CS_PIN;          /* disable device */

  return u32_ByteRead;
}

void FlashSST25Write(uint32_t u32_Addr, uint8_t* pu8_data, uint32_t u32_Len)
{
  uint32_t length;

  if (u32_Len == 0)
    return;

/* USER CODE BEGIN FLASH_Write 0 */

/* USER CODE END FLASH_Write 0 */

  // If starting at an odd address, write a single byte
  if ((u32_Addr & 0x01))
  {
    SST25_Wait_Busy();
    SST25_WREN();
    SST25_Byte_Program(u32_Addr, *pu8_data);
    pu8_data++;
    u32_Len--;
    u32_Addr++;
  }

  // If address is a sector boundary
  if((u32_Addr & (SST25_SIZE_SECTOR - 1)) == 0)
  {
    SST25_Wait_Busy();
    SST25_WREN();
    SST25_Sector_Erase(u32_Addr);
  }

  // if only one byte to be programmed
  if (u32_Len == 1)
  {
    SST25_Wait_Busy();
    SST25_WREN();
    SST25_Byte_Program(u32_Addr, *pu8_data);
    pu8_data++;
    u32_Len--;
    u32_Addr++;
  }
  else if (u32_Len > 0) // program all bytes
  {
    if ((u32_Len % 2) == 0)
      length = u32_Len;
    else
      length = u32_Len - 1;

    SST25_Wait_Busy();
    SST25_WREN();
    SST25_Auto_Add_IncA(u32_Addr, *pu8_data, *(pu8_data + 1));
    length -= 2;
    u32_Len -= 2;
    u32_Addr += 2;
    pu8_data += 2;
    SST25_Wait_Busy_AAI();

    while (length > 0)
    {
      SST25_Auto_Add_IncB(*pu8_data, *(pu8_data + 1));
      length -= 2;
      u32_Len -= 2;
      u32_Addr += 2;
      pu8_data += 2;
      SST25_Wait_Busy_AAI();

      // If address is a sector boundary
      if (((u32_Addr & (SST25_SIZE_SECTOR - 1)) == 0) && (length > 0))
      {
        SST25_WRDI(); // exit AAI mode
        SST25_Wait_Busy();
        SST25_WREN();
        SST25_Sector_Erase(u32_Addr);

        SST25_Wait_Busy();
        SST25_WREN();
        SST25_Auto_Add_IncA(u32_Addr, *pu8_data, *(pu8_data + 1));
        length -= 2;
        u32_Len -= 2;
        u32_Addr += 2;
        pu8_data += 2;
        SST25_Wait_Busy_AAI();
      }
    }

    SST25_WRDI(); // exit AAI mode

    // if a last byte to write
    if (u32_Len == 1)
    {
      // If address is a sector boundary
      if((u32_Addr & (SST25_SIZE_SECTOR - 1)) == 0)
      {
        SST25_Wait_Busy();
        SST25_WREN();
        SST25_Sector_Erase(u32_Addr);
      }

      SST25_Wait_Busy();
      SST25_WREN();
      SST25_Byte_Program(u32_Addr, *pu8_data);
      pu8_data++;
      u32_Len--;
      u32_Addr++;
    }
  }
}

 // Fonction qui écrit plusieurs octets de données dans la mémoire flash du SST25VF080B en utilisant la commande Page Program


void FlashSST25WriteWOErase(uint32_t u32_Addr, uint8_t* pu8_data, uint32_t u32_Len)
{
  uint32_t length;

  if (u32_Len == 0)
    return;

/* USER CODE BEGIN FLASH_WriteWoErase 0 */

/* USER CODE END FLASH_WriteWoErase 0 */

  // If starting at an odd address, write a single byte
  if ((u32_Addr & 0x01))
  {
    SST25_Wait_Busy();
    SST25_WREN();
    SST25_Byte_Program(u32_Addr, *pu8_data);
    pu8_data++;
    u32_Len--;
    u32_Addr++;
  }

  // if only one byte to be programmed
  if (u32_Len == 1)
  {
    SST25_Wait_Busy();
    SST25_WREN();
    SST25_Byte_Program(u32_Addr, *pu8_data);
    pu8_data++;
    u32_Len--;
    u32_Addr++;
  }
  else if (u32_Len > 0) // program all bytes
  {
    if ((u32_Len % 2) == 0)
      length = u32_Len;
    else
      length = u32_Len - 1;

    SST25_Wait_Busy();
    SST25_WREN();
    SST25_Auto_Add_IncA(u32_Addr, *pu8_data, *(pu8_data + 1));
    length -= 2;
    u32_Len -= 2;
    u32_Addr += 2;
    pu8_data += 2;
    SST25_Wait_Busy_AAI();

    while (length > 0)
    {
      SST25_Auto_Add_IncB(*pu8_data, *(pu8_data + 1));
      length -= 2;
      u32_Len -= 2;
      u32_Addr += 2;
      pu8_data += 2;
      SST25_Wait_Busy_AAI();
    }

    SST25_WRDI(); // exit AAI mode

    // if a last byte to write
    if (u32_Len == 1)
    {
      SST25_Wait_Busy();
      SST25_WREN();
      SST25_Byte_Program(u32_Addr, *pu8_data);
      pu8_data++;
      u32_Len--;
      u32_Addr++;
    }
  }
}

bool FlashSST25IsBusy(void)
{
  bool bRet = false;
  uint8_t u8_SR = SST25_Read_Status_Register();

  if (((u8_SR & 0x03) == 0x03) || ((u8_SR & 0x43) == 0x43))
    bRet = true;

  return bRet;
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

