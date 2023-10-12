/**
  ******************************************************************************
  * File Name          : debug.h
  * Version            : 1.0
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */

#pragma once

#include <stdbool.h>
#include "usart_ex.h"

/* Example d'activation/desactivation de debug, a mettre en haut du fichier module :
 *
 * Debug desactive :
 * #define TEST_DEBUG(fmt, ...)       // LOG_INFO(fmt, ##__VA_ARGS__)
 *
 * Debug active :
 * #define TEST_DEBUG(fmt, ...)        LOG_INFO(fmt, ##__VA_ARGS__)
 *
 **/

/**
  * Activer ou desactiver les log optimise
  * Activer ou desactiver les logs
  * Definir le handler de l'UART utilise pour le log
  */
  /* USER CODE BEGIN LOG 0 */

#define USE_OPTIMZE_LOG			1
#define EN_DBG_UART				1
#define LOG_UART            s_usart1

  /* USER CODE END LOG 0 */

#define LIBELLE_MESSAGE_RETURN	"\r\n"
#define LIBELLE_MESSAGE_TAB			" "

#define LOG_LIBELLE_INFO				"INFO     :"
#define LOG_LIBELLE_WARNING			"WARN /!\\ :"
#define LOG_LIBELLE_ERROR				"ERR  *** :"

#define LOG_COLOR_BLACK     "0"
#define LOG_COLOR_RED       "1"
#define LOG_COLOR_GREEN     "2"
#define LOG_COLOR_YELLOW    "3"
#define LOG_COLOR_BLUE      "4"
#define LOG_COLOR_MAGENTA   "5"
#define LOG_COLOR_CYAN      "6"
#define LOG_COLOR_WHITE     "7"

#define LOG_NORMAL_COLOR    "3"
#define LOG_BRIGTH_COLOR    "9"
#define LOG_NORM_BKG_COLOR  "4"
#define LOG_BRIG_BKG_COLOR  "10"

#define LOG_STYLE_NORMAL      "0"
#define LOG_STYLE_BOLD        "1"     // Not Supported by TeraTerm
#define LOG_STYLE_FAINT       "2"     // Not Supported by TeraTerm
#define LOG_STYLE_ITALIC      "3"     // Not Supported by TeraTerm
#define LOG_STYLE_UNDERLINE   "4"     // Not Supported by TeraTerm
#define LOG_STYLE_REVERSE     "7"
#define LOG_STYLE_CROSSED_OUT "9"     // Not Supported by TeraTerm

#define LOG(STYLE, COLOR_STYLE, COLOR)  "\x1B["STYLE";" COLOR_STYLE COLOR "m"

#define CLR_SCREEN				  "\x1B[2J"
#define MOVE_CURSOR_UP(up)	"\x1B["#up"F"
#define MOVE_CURSOR_DOWN(dw)"\x1B["#dw"E"
#define HIDE_CURSOR				  "\x1B[?25l"

#define STR_MV_CUR_UP       "\x1B[%luF"
#define STR_MV_CUR_DOWN     "\x1B[%luE"

/**
  * Activer ou desactiver l'utilisation de la couleur pour les log
  * Definir pour chaque type de log la couleur associee
  */
  /* USER CODE BEGIN LOG 1 */

#define USE_TS_OR_TIME      0

#define USE_DBG_COLOR				1

#define LOG_COLOR_INFO					LOG(LOG_STYLE_NORMAL, LOG_NORMAL_COLOR, LOG_COLOR_WHITE)
#define LOG_COLOR_WARNING				LOG(LOG_STYLE_NORMAL, LOG_NORMAL_COLOR, LOG_COLOR_YELLOW)
#define LOG_COLOR_ERROR					LOG(LOG_STYLE_NORMAL, LOG_NORMAL_COLOR, LOG_COLOR_RED)
#define LOG_RESET_COLOR					"\x1B[0m"

#define MACSTR												"%02X:%02X:%02X:%02X:%02X:%02X"
#define MACSTR_NODOT									"%02X%02X%02X%02X%02X%02X"
#define MAC2STR(a)										(a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]

#define IPSTR                         "%d.%d.%d.%d"
#define IP2STR(a)                     (a)[0], (a)[1], (a)[2], (a)[3]

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

  /* USER CODE END LOG 1 */

#define SIZE_DEBUG_MESSAGE		200		//<! \def size of debug message buffer

#define CASE_PRINT(EVT, format, ...)	case EVT : LOG_INFO(format " : " #EVT, ##__VA_ARGS__);
#define CASE_WARN(EVT, format, ...)	case EVT : LOG_WARNING(format " : " #EVT, ##__VA_ARGS__);

#if (EN_DBG_UART == 1)

#define LOG_SND_CMD(cmd)        do{	if (bDisableDebugLog == false) LOG_UART.TX_put(&LOG_UART, (uint8_t*)cmd, strlen(cmd));}while(0)
#define LOG_NEW_LINE()  		    do{	if (bDisableDebugLog == false) LOG_UART.TX_put(&LOG_UART, (uint8_t*)LIBELLE_MESSAGE_RETURN, sizeof(LIBELLE_MESSAGE_RETURN));}while(0)

void LOG_MOVE_CURSOR_UP(uint32_t up);
void LOG_MOVE_CURSOR_DOWN(uint32_t dw);

#if (USE_OPTIMZE_LOG != 0)
#define DEBUG_INFO					3
#define DEBUG_WARNING				2
#define DEBUG_ERROR					1

#define LOG_INFO(format, ...)		debug_printf(DEBUG_INFO, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...)	debug_printf(DEBUG_WARNING, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...)		debug_printf(DEBUG_ERROR, format, ##__VA_ARGS__)

void debug_printf(uint8_t u8_message_type, const char *pc_format, ...);

#else

#define LOG_FORMAT(level, format)  LOG_COLOR_ ## level "%010lu" LIBELLE_MESSAGE_TAB LOG_LIBELLE_ ## level LIBELLE_MESSAGE_TAB format LOG_RESET_COLOR LIBELLE_MESSAGE_RETURN

#define LOG_INFO(format, ...)		debug_printf(LOG_FORMAT(INFO, format),			(unsigned long int)HAL_GetTick(), ##__VA_ARGS__)
#define LOG_WARNING(format, ...)	debug_printf(LOG_FORMAT(WARNING, format),	(unsigned long int)HAL_GetTick(), ##__VA_ARGS__)
#define LOG_ERROR(format, ...)		debug_printf(LOG_FORMAT(ERROR, format),		(unsigned long int)HAL_GetTick(), ##__VA_ARGS__)

void debug_printf(const char *pc_format, ...);

#endif
#else
#define LOG_MOVE_CURSOR_UP(up)
#define LOG_INFO(format, ...)
#define LOG_WARNING(format, ...)
#define LOG_ERROR(format, ...)
#endif

extern bool bDisableDebugLog;

/* USER CODE BEGIN LOG 2 */

/* USER CODE END LOG 2 */

