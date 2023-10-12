#include "relay.h"


void openPowerRelay(void)
{
	HAL_GPIO_WritePin(CdeRelaisDePuissance_GPIO_Port, CdeRelaisDePuissance_Pin, GPIO_PIN_RESET);
	
  //digitalWrite(CdeRelaisDePuissance_Pin LOW);
}

void closePowerRelay(void)
{  //HAL_GPIO_WritePin(
	HAL_GPIO_WritePin(CdeRelaisDePuissance_GPIO_Port, CdeRelaisDePuissance_Pin, GPIO_PIN_SET);
  //digitalWrite(CdeRelaisDePuissance_Pin, HIGH);
}
