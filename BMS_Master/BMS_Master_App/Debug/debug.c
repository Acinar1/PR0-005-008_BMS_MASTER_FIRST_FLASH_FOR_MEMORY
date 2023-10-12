/**
  ******************************************************************************
  * File Name          : debug.c
  * Version            : 1.0
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */

/* Includes */
#include "main.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#include "usart.h"
#include "usart_ex.h"
#include "stdarg.h"	// va_list

/* USER CODE BEGIN LOG include */

/* USER CODE END LOG include */

#if (EN_DBG_UART == 1)
/* definitions */
#if (USE_TS_OR_TIME > 0)
#define SIZE_TIMESTAMP			15		//<! \def time stamp with uint32_t type print on 15 digit
#else
#define SIZE_TIMESTAMP			10		//<! \def time stamp with uint32_t type print on 10 digit
#endif

#if (USE_DBG_COLOR == 1)
//<! \def size of debug message header
#define HEADER_MSG_LEN			(sizeof((char *)LOG_COLOR_ERROR) + SIZE_TIMESTAMP + sizeof((char *)LIBELLE_MESSAGE_TAB) + \
														sizeof((char *)LOG_LIBELLE_INFO) + sizeof((char *)LIBELLE_MESSAGE_TAB) + 2)
//<! \def size of debug message footer
#define END_MSG_LEN					(sizeof((char *)LOG_RESET_COLOR) + sizeof((char *)LIBELLE_MESSAGE_RETURN))

#define DBG_COLOR_STR					"%s"
#define DBG_COLOR_VAL					(u8_message_type == DEBUG_INFO ? (char *)LOG_COLOR_INFO : (u8_message_type == DEBUG_WARNING ? (char *)LOG_COLOR_WARNING : (char *)LOG_COLOR_ERROR)),
#else
//<! \def size of debug message header
#define HEADER_MSG_LEN			(SIZE_TIMESTAMP + sizeof((char *)LIBELLE_MESSAGE_TAB) + \
														sizeof((char *)LOG_LIBELLE_INFO) + sizeof((char *)LIBELLE_MESSAGE_TAB))
//<! \def size of debug message footer
#define END_MSG_LEN					(sizeof((char *)LIBELLE_MESSAGE_RETURN))

#define DBG_COLOR_STR					""
#define DBG_COLOR_VAL
#endif

/* USER CODE BEGIN LOG Private defines */

/* USER CODE END LOG Private defines */

/* variables */
char ac_EndOfMsg[] = LOG_RESET_COLOR "\r\n";
bool bDisableDebugLog = false;

/* USER CODE BEGIN LOG Private variables */

/* USER CODE END LOG Private variables */

/* local functions prototypes */

/* USER CODE BEGIN LOG Private function prototypes */

/* USER CODE END LOG Private function prototypes */

/* functions */

/* USER CODE BEGIN LOG 0 */

/* USER CODE END LOG 0 */

void LOG_MOVE_CURSOR_UP(uint32_t up)
{
  /* USER CODE BEGIN CURSOR_UP 1 */

  /* USER CODE END CURSOR_UP 1 */
	if (bDisableDebugLog == false)
	{
		char acMoveCursorUp[8];
		sprintf(acMoveCursorUp, STR_MV_CUR_UP, up);
		LOG_UART.TX_put(&LOG_UART, (uint8_t*)acMoveCursorUp, strlen(acMoveCursorUp));
	}
  /* USER CODE BEGIN CURSOR_UP 2 */

  /* USER CODE END CURSOR_UP 2 */
}

void LOG_MOVE_CURSOR_DOWN(uint32_t dw)
{
  /* USER CODE BEGIN CURSOR_DOWN 1 */

  /* USER CODE END CURSOR_DOWN 1 */
	if (bDisableDebugLog == false)
	{
		char acMoveCursorDown[8];
		sprintf(acMoveCursorDown, STR_MV_CUR_DOWN, dw);
		LOG_UART.TX_put(&LOG_UART, (uint8_t*)acMoveCursorDown, strlen(acMoveCursorDown));
	}
  /* USER CODE BEGIN CURSOR_DOWN 2 */

  /* USER CODE END CURSOR_DOWN 2 */
}

/** ************************************************************
 * \fn void debug_printf(uint8_t u8_message_type, const char *pc_format...)
 *
 * \brief printf like function, with string format support
 *
 * \param u8_message_type : type of debug message : INFO, WARNING, ERROR
 * \param pc_format : string format definition
 *
 * \return void
 *
 * \author JR-Innovel
 ***************************************************************/
#if (USE_OPTIMZE_LOG != 0)
void debug_printf(uint8_t u8_message_type, const char *pc_format, ...)
{
	char ac_debugMessage[SIZE_DEBUG_MESSAGE];
	va_list ap;

/* USER CODE BEGIN LOG 1 */

/* USER CODE END LOG 1 */
	if (bDisableDebugLog != false)
		return;

#if (USE_TS_OR_TIME > 0)
  div_t divresult;
  uint8_t u8_j, u8_h, u8_m, u8_s;
  uint16_t u16_ms;
  uint32_t u32_Tick = HAL_GetTick();
  divresult = div(u32_Tick, 86400000);
  u8_j = divresult.quot;
  divresult = div(divresult.rem, 3600000);
  u8_h = divresult.quot;
  divresult = div(divresult.rem, 60000);
  u8_m = divresult.quot;
  divresult = div(divresult.rem, 1000);
  u8_s = divresult.quot;
  u16_ms = divresult.rem;

  sprintf(ac_debugMessage, DBG_COLOR_STR"%02dj%02d:%02d:%02d'%03d%s%s%s", DBG_COLOR_VAL
						u8_j, u8_h, u8_m, u8_s, u16_ms, (char *)LIBELLE_MESSAGE_TAB,
						(u8_message_type == DEBUG_INFO ? (char *)LOG_LIBELLE_INFO : (u8_message_type == DEBUG_WARNING ? (char *)LOG_LIBELLE_WARNING : (char *)LOG_LIBELLE_ERROR)),
		(char *)LIBELLE_MESSAGE_TAB);
#else
	sprintf(ac_debugMessage, DBG_COLOR_STR"%010lu%s%s%s", DBG_COLOR_VAL
						(unsigned long int)HAL_GetTick(),
						(char *)LIBELLE_MESSAGE_TAB, (u8_message_type == DEBUG_INFO ? (char *)LOG_LIBELLE_INFO : (u8_message_type == DEBUG_WARNING ? (char *)LOG_LIBELLE_WARNING : (char *)LOG_LIBELLE_ERROR)),
		(char *)LIBELLE_MESSAGE_TAB);
#endif

	va_start(ap, pc_format);	// init stack ptr from pc_format parameter
	vsnprintf(&ac_debugMessage[HEADER_MSG_LEN], (SIZE_DEBUG_MESSAGE - HEADER_MSG_LEN - END_MSG_LEN), pc_format, ap);	// format the string
	va_end(ap);				// release ptr

	// add end of message
#if (USE_DBG_COLOR == 1)
	strcat(ac_debugMessage, (char *)LOG_RESET_COLOR);
#endif
	strcat(ac_debugMessage, (char *)LIBELLE_MESSAGE_RETURN);

/* USER CODE BEGIN LOG 2 */

/* USER CODE END LOG 2 */

	// put on uart
  LOG_UART.TX_put(&LOG_UART, (uint8_t*)ac_debugMessage, strlen(ac_debugMessage));

/* USER CODE BEGIN LOG 3 */

/* USER CODE END LOG 3 */
}
#else
void debug_printf(const char *pc_format, ...)
{
	va_list ap;
	char ac_debugMessage[SIZE_DEBUG_MESSAGE];
	int iRealLenOfMsg;

/* USER CODE BEGIN LOG 4 */

/* USER CODE END LOG 4 */
	if (bDisableDebugLog != false)
		return;

	va_start( ap, pc_format );	// init stack ptr from pc_format parameter
	iRealLenOfMsg = vsnprintf( ac_debugMessage, SIZE_DEBUG_MESSAGE, pc_format, ap );	// format the string
	va_end( ap );				// release ptr

	// put on uart
  LOG_UART.TX_put(&LOG_UART, (uint8_t*)ac_debugMessage, strlen(ac_debugMessage));

/* USER CODE BEGIN LOG 5 */

/* USER CODE END LOG 5 */
	if (iRealLenOfMsg >= SIZE_DEBUG_MESSAGE)
	{
		// put on uart
		LOG_UART.TX_put(&LOG_UART, (uint8_t*)ac_EndOfMsg, strlen(ac_EndOfMsg));
/* USER CODE BEGIN LOG 6 */

/* USER CODE END LOG 6 */
	}

/* USER CODE BEGIN LOG 7 */

/* USER CODE END LOG 7 */
}
#endif

#endif

