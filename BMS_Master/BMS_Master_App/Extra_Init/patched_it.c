/**
  ******************************************************************************
  * File Name          : patched_it.c
  * Version            : 1.0
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "patched_it.h"
#include "usart_ex.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/* USER CODE BEGIN Private variables */

/* USER CODE END Private variables */

/* USER CODE BEGIN Private function prototypes */

/* USER CODE END Private function prototypes */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
* HardFaultHandler_C:
*This is called from the HardFault_HandlerAsm with a pointer the Fault stack
* as the parameter.We can then read the values from the stack and place them
* into local variables for ease of reading.
* We then read the various Fault Status and Address Registers to help decode
* cause of the fault.
* The function ends with a BKPT instruction to force control back into the debugger
*/
void HardFault_HandlerC(unsigned long *hardfault_args)
{
	volatile unsigned long stacked_r0;
	volatile unsigned long stacked_r1;
	volatile unsigned long stacked_r2;
	volatile unsigned long stacked_r3;
	volatile unsigned long stacked_r12;
	volatile unsigned long stacked_lr;
	volatile unsigned long stacked_pc;
	volatile unsigned long stacked_psr;
	volatile unsigned long _CFSR;
	volatile unsigned long _HFSR;
	volatile unsigned long _DFSR;
	volatile unsigned long _AFSR;
	volatile unsigned long _BFAR;
	volatile unsigned long _MMFAR;

	stacked_r0 = ((unsigned long)hardfault_args[0]);
	stacked_r1 = ((unsigned long)hardfault_args[1]);
	stacked_r2 = ((unsigned long)hardfault_args[2]);
	stacked_r3 = ((unsigned long)hardfault_args[3]);
	stacked_r12 = ((unsigned long)hardfault_args[4]);
	stacked_lr = ((unsigned long)hardfault_args[5]);
	stacked_pc = ((unsigned long)hardfault_args[6]);
	stacked_psr = ((unsigned long)hardfault_args[7]);

	// Configurable Fault Status Register
	// Consists of MMSR, BFSR and UFSR
	_CFSR = (*((volatile unsigned long *)(0xE000ED28)));

	// Hard Fault Status Register
	_HFSR = (*((volatile unsigned long *)(0xE000ED2C)));

	// Debug Fault Status Register
	_DFSR = (*((volatile unsigned long *)(0xE000ED30)));

	// Auxiliary Fault Status Register
	_AFSR = (*((volatile unsigned long *)(0xE000ED3C)));

	// Read the Fault Address Registers. These may not contain valid values.
	// Check BFARVALID/MMARVALID to see if they are valid values
	// MemManage Fault Address Register
	_MMFAR = (*((volatile unsigned long *)(0xE000ED34)));
	// Bus Fault Address Register
	_BFAR = (*((volatile unsigned long *)(0xE000ED38)));

/* USER CODE BEGIN HardFault_HandlerC 0 */

  u32_HardFault_PC = stacked_pc;
  u32_HardFault_LR = stacked_lr;
  u32_BootMode = BOOT_MODE_ERROR_HARD_FAULT;

/* USER CODE END HardFault_HandlerC 0 */

	while (~USART1->ISR & UART_FLAG_TXE);
	USART1->TDR = 'H';
	while (~USART1->ISR & UART_FLAG_TXE);
	USART1->TDR = 'a';
	while (~USART1->ISR & UART_FLAG_TXE);
	USART1->TDR = 'r';
	while (~USART1->ISR & UART_FLAG_TXE);
	USART1->TDR = 'd';
	while (~USART1->ISR & UART_FLAG_TXE);
	USART1->TDR = ' ';
	while (~USART1->ISR & UART_FLAG_TXE);
	USART1->TDR = 'F';
	while (~USART1->ISR & UART_FLAG_TXE);
	USART1->TDR = 'a';
	while (~USART1->ISR & UART_FLAG_TXE);
	USART1->TDR = 'u';
	while (~USART1->ISR & UART_FLAG_TXE);
	USART1->TDR = 'l';
	while (~USART1->ISR & UART_FLAG_TXE);
	USART1->TDR = 't';
	while (~USART1->ISR & UART_FLAG_TXE);
	USART1->TDR = '\r';
	while (~USART1->ISR & UART_FLAG_TXE);
	USART1->TDR = '\n';
	while (~USART1->ISR & UART_FLAG_TXE);

/* USER CODE BEGIN HardFault_HandlerC 1 */

#ifdef DEBUG
	HAL_NVIC_SystemReset();
	//__asm("BKPT #0\n"); // Break into the debugger
#else
	//HAL_NVIC_SystemReset();
 	__DSB();                                                          /* Ensure all outstanding memory accesses included
																																		 buffered write are completed before reset */
	SCB->AIRCR = ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
		SCB_AIRCR_SYSRESETREQ_Msk);
	__DSB();                                                          /* Ensure completion of memory access */

	for (;;)                                                           /* wait until reset */
	{
		__NOP();
 	}
#endif

/* USER CODE END HardFault_HandlerC 1 */

	UNUSED(stacked_r0);
	UNUSED(stacked_r1);
	UNUSED(stacked_r2);
	UNUSED(stacked_r3);
	UNUSED(stacked_r12);
	UNUSED(stacked_lr);
	UNUSED(stacked_pc);
	UNUSED(stacked_psr);
	UNUSED(_CFSR);
	UNUSED(_HFSR);
	UNUSED(_DFSR);
	UNUSED(_AFSR);
	UNUSED(_BFAR);
	UNUSED(_MMFAR);
}

/******************************************************************************/
/*                Processor Interruption and Exception Handlers               */
/******************************************************************************/

/**
* @brief This function handles Hard fault interrupt.
*/
void __attribute__((naked, noreturn)) HardFault_Handler(void)
{
	/* USER CODE BEGIN HardFault_IRQn 0 */

	/* USER CODE END HardFault_IRQn 0 */

	/*
	* Get the appropriate stack pointer, depending on our mode,
	* and use it as the parameter to the C handler. This function
	* will never return
	*/
	__asm("MOVS   R0, #4  \n"
	"MOV    R1, LR  \n"
		"TST    R0, R1  \n"
		"BEQ    _MSP    \n"
		"MRS    R0, PSP \n"
		"B			_HALT		\n"
		"_MSP:  \n"
		"MRS    R0, MSP \n"
		"_HALT:  \n"
		"LDR R1,[R0,#20] \n"
		"B      HardFault_HandlerC      \n");

	/* USER CODE BEGIN HardFault_IRQn 1 */

	/* USER CODE END HardFault_IRQn 1 */
	while (1)
	{
	}
	/* USER CODE BEGIN HardFault_IRQn 2 */

	/* USER CODE END HardFault_IRQn 2 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

void UART5_IRQHandler(void)
{
  /* USER CODE BEGIN UART5_IRQn 0 */

  /* USER CODE END UART5_IRQn 0 */
  UART_IrqHandler(&s_uart5);
  /* USER CODE BEGIN UART5_IRQn 1 */

  /* USER CODE END UART5_IRQn 1 */
}

void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  UART_IrqHandler(&s_usart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

