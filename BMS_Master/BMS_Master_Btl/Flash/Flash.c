
#include "main.h"
#include "spi.h"
#include "Flash.h"

#define RDY_BSY_BIT                     0x01

uint32_t FlashReadDevID(void)
{
  uint8_t au8_Cmd[4] = { 0x9F, 0, 0, 0 };
  uint8_t au8_Rsp[4] = { 0, 0, 0, 0 };
  uint32_t u32DevID = 0;

  // select flash
  Flash_CS_GPIO_Port->BRR = Flash_CS_Pin;
  // transmit the command
  HAL_SPI_TransmitReceive(&hspi1, au8_Cmd, au8_Rsp, 4, 10);
  // unselect flash
  Flash_CS_GPIO_Port->BSRR = Flash_CS_Pin;

  u32DevID = ((uint32_t)au8_Rsp[1] << 16) | ((uint32_t)au8_Rsp[2] << 8) | (uint32_t)au8_Rsp[3];
  return u32DevID;
}

uint8_t FlashReadStatusReg(void)
{
  uint8_t au8_Cmd[2] = { 0x05, 0 };
  uint8_t au8_Resp[2] = { 0,0 };

  // select flash
  Flash_CS_GPIO_Port->BRR = Flash_CS_Pin;
  // transmit the command
  HAL_SPI_TransmitReceive(&hspi1, au8_Cmd, au8_Resp, 2, 10);
  // unselect flash
  Flash_CS_GPIO_Port->BSRR = Flash_CS_Pin;

  return au8_Resp[1];
}

void FlashWaitBusy(void)
{
  while ((FlashReadStatusReg() & RDY_BSY_BIT) == RDY_BSY_BIT);
}

uint32_t FlashRead(uint32_t u32_Addr, uint8_t* pu8_data, uint32_t u32_Len)
{
  uint32_t u32_ByteRead = 0;
  uint8_t au8_Cmd[5] = { 0x03 , 0, 0, 0, 0 };
  uint8_t u8_WrIdx = 1;

  au8_Cmd[u8_WrIdx++] = (u32_Addr & 0x00FF0000) >> 16;
  au8_Cmd[u8_WrIdx++] = (u32_Addr & 0x0000FF00) >> 8;
  au8_Cmd[u8_WrIdx++] = (u32_Addr & 0x000000FF);

  FlashWaitBusy();

  // select flash
  Flash_CS_GPIO_Port->BRR = Flash_CS_Pin;
  // transmit the command
  HAL_SPI_Transmit(&hspi1, au8_Cmd, u8_WrIdx, 10);
  HAL_SPIEx_FlushRxFifo(&hspi1);
  // read all data
  if (HAL_SPI_Receive(&hspi1, pu8_data, u32_Len, 10) == HAL_OK)
    u32_ByteRead = u32_Len;
  // unselect flash
  Flash_CS_GPIO_Port->BSRR = Flash_CS_Pin;

  return u32_ByteRead;
}
