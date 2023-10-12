
_flash_full_size = 512K;
_sram_full_size = 128K;

_ram_start_addr = 0x20000000;
_shared_memory_length = 16;
_no_init_memory_length = 16;
_ram_length = _sram_full_size - _shared_memory_length - _no_init_memory_length;

/* Shared memory start address symbol */
_no_init_memory_address = _ram_start_addr + _ram_length;
_shared_memory_address = _no_init_memory_address + _no_init_memory_length;

_bootloader_start_addr = 0x08000000;
_bootloader_length = 16K;

/* Main app start address symbol */
_main_app_start_address = _bootloader_start_addr + _bootloader_length;

/* Firmware length */
_firmware_length = _flash_full_size - _bootloader_length;


/* Highest address of the user mode stack */
_estack = _no_init_memory_address;
