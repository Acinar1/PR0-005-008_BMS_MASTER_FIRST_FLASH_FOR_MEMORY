#pragma once


#define W25Q01JV_DEVID                    0x00EF7021
#define MT25QL01_DEVID                    0x0020BA21
#define SST25_DEVID                       0x00BF258E


#define W25_ADDR_NewFirmwareHeader        0x07F80000
#define MT25_ADDR_NewFirmwareHeader       0x07F80000
#define SST25_ADDR_NewFirmwareHeader      0x00080000


uint32_t FlashReadDevID(void);
uint32_t FlashRead(uint32_t u32_Addr, uint8_t* pu8_data, uint32_t u32_Len);
