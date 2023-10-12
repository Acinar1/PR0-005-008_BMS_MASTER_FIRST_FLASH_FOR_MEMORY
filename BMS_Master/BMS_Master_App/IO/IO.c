/**
  ******************************************************************************
  * File Name          : IO.c
  * Version            : 1.0
  * Description        :
  ******************************************************************************
  ******************************************************************************
  */

/* Includes */
#include "IO.h"

/* USER CODE BEGIN include */

/* USER CODE END include */

/* definitions */
#define InitFilter(IOName, reverse)     {.oIO.Port=MakePortPin(IOName,GPIO_Port),.oIO.Pin=MakePortPin(IOName,Pin),.oIO.bReverse=reverse,.u32_Tick=0,.u32_Val=0,.bInPrevVal=false}

/* USER CODE BEGIN Private defines */
#define InputFilterTime_ms      50
/* USER CODE END Private defines */

/* variables */
InFilter aoInPutFilter[NumFilteredInput] = {
/* USER CODE BEGIN FilterInit */
  InitFilter(EtatVerrouPrise, false),
  InitFilter(ContactClef, false),
  InitFilter(HVIL1, false),
  InitFilter(HVIL2, false),
  InitFilter(HVIL3, false),
  InitFilter(Ana_Tor1, false),
  InitFilter(Ana_Tor2, false),
  InitFilter(Ana_Tor3, false),
  InitFilter(Tor4, false),
/* USER CODE END FilterInit */
};
/* USER CODE BEGIN Private variables */

/* USER CODE END Private variables */

/* local functions prototypes */

/* USER CODE BEGIN Private function prototypes */

/* USER CODE END Private function prototypes */

/* functions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void InPutFilter(void)
{
  uint32_t i;
  bool bNewVal = false;

  for (i = 0; i < NumFilteredInput; i++)
  {
    bNewVal = aoInPutFilter[i].bInPrevVal;
    if ((aoInPutFilter[i].oIO.Port->IDR & aoInPutFilter[i].oIO.Pin) != aoInPutFilter[i].u32_Val)
    {
      aoInPutFilter[i].u32_Val = aoInPutFilter[i].oIO.Port->IDR & aoInPutFilter[i].oIO.Pin;
      aoInPutFilter[i].u32_Tick = HAL_GetTick();
    }
    else if ((HAL_GetTick() - aoInPutFilter[i].u32_Tick) > InputFilterTime_ms)
    {
      if (aoInPutFilter[i].u32_Val != 0)
      {
        if (aoInPutFilter[i].oIO.bReverse == false)
          bNewVal = true;
        else
          bNewVal = false;
      }
      else
      {
        if (aoInPutFilter[i].oIO.bReverse == false)
          bNewVal = false;
        else
          bNewVal = true;
      }
    }

    if ((bNewVal == false) && (aoInPutFilter[i].bInPrevVal != false))
      aoInPutFilter[i].bFallingEdgeDetect = true;
    else if ((bNewVal != false) && (aoInPutFilter[i].bInPrevVal == false))
      aoInPutFilter[i].bRisingEdgeDetect = true;

    aoInPutFilter[i].bInPrevVal = bNewVal;
  }
}

bool InPutGetRisingEdge(uint32_t u32_FilterID)
{
  if (u32_FilterID > NumFilteredInput)
    return false;

  if (aoInPutFilter[u32_FilterID].bRisingEdgeDetect != false)
  {
    aoInPutFilter[u32_FilterID].bRisingEdgeDetect = false;
    return true;
  }
  return false;
}

bool InPutGetFallingEdge(uint32_t u32_FilterID)
{
  if (u32_FilterID > NumFilteredInput)
    return false;

  if (aoInPutFilter[u32_FilterID].bFallingEdgeDetect != false)
  {
    aoInPutFilter[u32_FilterID].bFallingEdgeDetect = false;
    return true;
  }
  return false;
}

void InPutClearEdge(uint32_t u32_FilterID)
{
  if (u32_FilterID <= NumFilteredInput)
  {
    aoInPutFilter[u32_FilterID].bFallingEdgeDetect = false;
    aoInPutFilter[u32_FilterID].bRisingEdgeDetect = false;
  }
}

