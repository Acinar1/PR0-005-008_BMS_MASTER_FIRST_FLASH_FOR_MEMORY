#pragma once

#define BOOT_MODE_UPDATE_DONE							0x1DA7E000
#define BOOT_MODE_UPDATE_TO_DO						0x1DA7E055

#define BOOT_MODE_ERROR_CRC								0xFE000C8C
#define BOOT_MODE_ERROR_SIZE							0xFE00512E
#define BOOT_MODE_ERROR_FLASH_LOCK				0xFE0010C8
#define BOOT_MODE_ERROR_BLANK							0xFE0B1A08
#define BOOT_MODE_ERROR_VERIFY						0xFEAE81F1
#define BOOT_MODE_ERROR_HEADER            0xFE013E17
#define BOOT_MODE_ERROR_PRODUCT           0xFE00091D
#define BOOT_MODE_ERROR_CPU               0xFEC9171D
#define BOOT_MODE_ERROR_NULL							0xFE000000
#define BOOT_MODE_ERROR_WATCHDOG  				0xFE50F73D
#define BOOT_MODE_ERROR_HARD_FAULT				0xFEAADFA7

#define BOOT_MODE_ERROR_MEMORY            0xFE0003E3

#define CpuID	                            0
#define ProductID				                  0         // TODO From InnoSAV

#define FlashAddrOfNewFirmware            0x00080000
