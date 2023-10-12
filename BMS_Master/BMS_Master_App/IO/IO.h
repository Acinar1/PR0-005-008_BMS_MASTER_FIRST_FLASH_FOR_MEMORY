/**
  ******************************************************************************
  * File Name          : IO.h
  * Version            : 1.0
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */

#pragma once

/* Includes */

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

/* USER CODE BEGIN include */

/* USER CODE END include */

/* definitions */

/* USER CODE BEGIN Exported defines */
#define NumFilteredInput          9

#define EtatVerrouPrise_FilterID  0
#define ContactClef_FilterID      1
#define HVIL1_FilterID            2
#define HVIL2_FilterID            3
#define HVIL3_FilterID            4
#define TOR1_FilterID             5
#define TOR2_FilterID             6
#define TOR3_FilterID             7
#define TOR4_FilterID             8

/* USER CODE END Exported defines */

/* exported typedef */
typedef struct {
  GPIO_TypeDef* Port;
  uint16_t Pin;
  bool bReverse;
}I_O_Def;

typedef struct {
  I_O_Def oIO;
  uint32_t u32_Tick;
  uint32_t u32_Val;
  bool bInPrevVal;
  bool bRisingEdgeDetect;
  bool bFallingEdgeDetect;
}InFilter;

/* USER CODE BEGIN Exported typedef */

/* USER CODE END Exported typedef */

/* exported variables */
extern InFilter aoInPutFilter[NumFilteredInput];

/* USER CODE BEGIN Exported variables */

/* USER CODE END Exported variables */

/* exported Macros */
#define MakePortPin(x,y)  x ## _ ## y
#define GetIn(IOName)   ((MakePortPin(IOName,GPIO_Port)->IDR & MakePortPin(IOName,Pin)) == 0 ? false : true)
#define GetOut(IOName)  ((MakePortPin(IOName,GPIO_Port)->ODR & MakePortPin(IOName,Pin)) == 0 ? false : true)
#define SetOut(IOName)    MakePortPin(IOName,GPIO_Port)->BSRR = MakePortPin(IOName,Pin)
#define ResetOut(IOName)  MakePortPin(IOName,GPIO_Port)->BSRR = MakePortPin(IOName,Pin) << 16
#define ToggleOut(IOName) do{if(GetOut(IOName) == false) SetOut(IOName); else ResetOut(IOName);}while(0)

#define InPutFilteredGetValue(FilterID)   aoInPutFilter[FilterID].bInPrevVal

/* USER CODE BEGIN Exported Macros */

/* USER CODE END Exported Macros */

/* exported functions */
void InPutFilter(void);
bool InPutGetRisingEdge(uint32_t u32_FilterID);
bool InPutGetFallingEdge(uint32_t u32_FilterID);
void InPutClearEdge(uint32_t u32_FilterID);

/* USER CODE BEGIN Exported functions */

/* USER CODE END Exported functions */

