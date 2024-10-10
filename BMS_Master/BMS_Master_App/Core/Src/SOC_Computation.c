#include "SOC_Computation.h"




void computeSOC(int * BMS_SOC, uint16_t * BMS_SOC_percentage, int regenCurrent, int DCDC_Current, int *HallEffectCurrent, int * LowCurrentValue, int *HighCurrentValue, int Charger_Current, int BMS_State)
{
  int testValeurTemp = 0;
  //Datasheet : Offset (moyenne pour retourner à 0A à l'arret), dépend aussi de la température
  int OffsetLow = 0; //x10
  int OffsetHigh = 0; //x10
  
  //valeurs x10 : en 0.1A
	rawAnaInput_LowCurrent = au16_AdcFilteredValues[1];
	*LowCurrentValue = (((au16_AdcFilteredValues[1] * 3.3 / 4096 / 0.5833) - 2.5) / 0.04) * 10 - OffsetLow;
	rawAnaInput_HighCurrent = au16_AdcFilteredValues[2];
	*HighCurrentValue = (((au16_AdcFilteredValues[2] * 3.3 / 4096 / 0.5833) - 2.5) / 0.0025) * 10 - OffsetHigh; 
	
  
  //anciens calculs
  //*LowCurrentValue = ((rawAnaInput_LowCurrent*3.3)/(1024*0.6875)-2.5)*30; 
  //*HighCurrentValue = ((rawAnaInput_HighCurrent*3.3)/(1024*0.6875)-2.5)*300; 
  
  testValeurTemp = *LowCurrentValue/10;
  if(testValeurTemp <= -40)
  {
    *HallEffectCurrent = *HighCurrentValue;
  }
  else if(testValeurTemp > -41 && testValeurTemp <= -35)
  {
    *HallEffectCurrent = (*HighCurrentValue+*LowCurrentValue)/2;
  }
  else if(testValeurTemp > -35 && testValeurTemp <= 40)
  {
    *HallEffectCurrent = *LowCurrentValue;
  }
  else if (testValeurTemp > 40 && testValeurTemp <=48)
  {
    *HallEffectCurrent = (*LowCurrentValue + *HighCurrentValue)/2;
  }
  else if (testValeurTemp > 47)
  {
    *HallEffectCurrent = *HighCurrentValue;
  }

  if (BMS_State == 1) // if we are on driving mode
  {
    
    if (regenCurrent == 0)
    {
      if (* BMS_SOC >= 0)
      {
        *BMS_SOC = *BMS_SOC - (*HallEffectCurrent/10) - (DCDC_Current*0.16475);//0.1318 is the ratio between high voltage and low voltage and 0.8 is the DCDC efficiency estimation
      }
    }
    else
    {
      if (* BMS_SOC <= 37422000)
      {
        *BMS_SOC = *BMS_SOC - (*HallEffectCurrent/10) - (DCDC_Current*0.16475);
      }
    }
  }
  else if (BMS_State == 2) // charging state
  {
    if(*BMS_SOC <= 37422000)
    {
    *BMS_SOC = *BMS_SOC + Charger_Current;
    }
  }
  
  *BMS_SOC_percentage = (*BMS_SOC / 378000) + 1;

}

int computeMDC(int BMS_SOC, int tempCellMin, int tempCellMax)
{
  int MDCValue = 0;
  int dx = 0;
  int dy = 0;
  int deltax = 0;
  int deltay = 0;
  int deltafx = 0;
  int deltafy = 0;
  int deltafxy = 0;
  int x1 = 1;
  int y1 = 0;
  int x2 = 0;
  int y2 = 0;
  int indexx = 1;
  int indexy = 1;
  bool temperatureFound = false;
  bool BMS_SOCFound = false;
  int temperatureCell = 0;

  // Selection of cell temperature
  if (tempCellMin > 0 && tempCellMax < 50)
  {
    temperatureCell = (tempCellMin + tempCellMax)/2;
  }
  else if(tempCellMax > 54)
  {
    temperatureCell = 54;
  }
  else if(tempCellMin < -19)
  {
    temperatureCell = -19;
  }
  else if(tempCellMin < 0)
  {
    temperatureCell = tempCellMin;
  }
  else if(tempCellMax > 50)
  {
    temperatureCell = tempCellMax;
  }

  // Range of BMS
  if (BMS_SOC < 1)
  {
    BMS_SOC = 1;
  }
  else if(BMS_SOC>99)
  {
    BMS_SOC = 99;
  }

  while (BMS_SOCFound == false)
  {
    if (BMS_SOC > MDC[0][indexx])
    {
      BMS_SOCFound = true;
      x1 =  MDC[0][indexx];
      x2 =  MDC[0][indexx+1]; // should be necessary to verify that it is not the end of the table
    }
    else
    {
      indexx = indexx + 1;
    }
  }

  while (temperatureFound == false)
  {
    if (temperatureCell > MDC[indexy][0])
    {
      temperatureFound = true;
      y1 =  MDC[indexy][0];
      y2 =  MDC[indexy+1][0]; // should be necessary to verify that it is not the end of the table
    }
    else
    {
      indexy = indexy + 1;
    }
  }

  dx = BMS_SOC - x1;
  dy = temperatureCell - y1;

  deltax = x2 - x1;
  deltay = y2 - y1;

  deltafx = MDC[indexy][indexx+1] - MDC[indexy][indexx];
  deltafy = MDC[indexy+1][indexx] - MDC[indexy][indexx];
  deltafxy = MDC[indexy][indexx] + MDC[indexy+1][indexx+1] - MDC[indexy][indexx+1] - MDC[indexy+1][indexx] ;

  MDCValue = deltafx * dx / deltax + deltafy * dy / deltay + deltafxy * (dx / deltax) * (dy/deltay) + MDC[indexy][indexx]; 

  return MDCValue;
}

int computeTMDC(int BMS_SOC, int tempCellMin, int tempCellMax)
{
  int TMDCValue = 0;
  int dx = 0;
  int dy = 0;
  int deltax = 0;
  int deltay = 0;
  int deltafx = 0;
  int deltafy = 0;
  int deltafxy = 0;
  int x1 = 1;
  int y1 = 0;
  int x2 = 0;
  int y2 = 0;
  int indexx = 1;
  int indexy = 1;
  bool temperatureFound = false;
  bool BMS_SOCFound = false;
  int temperatureCell = 0;

  // Selection of cell temperature
  if (tempCellMin > 0 && tempCellMax < 50)
  {
    temperatureCell = (tempCellMin + tempCellMax)/2;
  }
  else if(tempCellMax > 54)
  {
    temperatureCell = 54;
  }
  else if(tempCellMin < -19)
  {
    temperatureCell = -19;
  }
  else if(tempCellMin < 0)
  {
    temperatureCell = tempCellMin;
  }
  else if(tempCellMax > 50)
  {
    temperatureCell = tempCellMax;
  }

  // Range of BMS
  if (BMS_SOC < 1)
  {
    BMS_SOC = 1;
  }
  else if(BMS_SOC>99)
  {
    BMS_SOC = 99;
  }

  while (BMS_SOCFound == false)
  {
    if (BMS_SOC > TMDC[0][indexx])
    {
      BMS_SOCFound = true;
      x1 =  TMDC[0][indexx];
      x2 =  TMDC[0][indexx+1]; // should be necessary to verify that it is not the end of the table
    }
    else
    {
      indexx = indexx + 1;
    }
  }

  while (temperatureFound == false)
  {
    if (temperatureCell > TMDC[indexy][0])
    {
      temperatureFound = true;
      y1 =  TMDC[indexy][0];
      y2 =  TMDC[indexy+1][0]; // should be necessary to verify that it is not the end of the table
    }
    else
    {
      indexy = indexy + 1;
    }
  }

  dx = BMS_SOC - x1;
  dy = temperatureCell - y1;

  deltax = x2 - x1;
  deltay = y2 - y1;

  deltafx = TMDC[indexy][indexx+1] - TMDC[indexy][indexx];
  deltafy = TMDC[indexy+1][indexx] - TMDC[indexy][indexx];
  deltafxy = TMDC[indexy][indexx] + TMDC[indexy+1][indexx+1] - TMDC[indexy][indexx+1] - TMDC[indexy+1][indexx] ;

  TMDCValue = deltafx * dx / deltax + deltafy * dy / deltay + deltafxy * (dx / deltax) * (dy/deltay) + TMDC[indexy][indexx]; 

  return TMDCValue;
}

int computeMCC(int BMS_SOC, int tempCellMin, int tempCellMax)
{
  int MCCValue = 0;
  int dx = 0;
  int dy = 0;
  int deltax = 0;
  int deltay = 0;
  int deltafx = 0;
  int deltafy = 0;
  int deltafxy = 0;
  int x1 = 1;
  int y1 = 0;
  int x2 = 0;
  int y2 = 0;
  int indexx = 1;
  int indexy = 1;
  bool temperatureFound = false;
  bool BMS_SOCFound = false;
  int temperatureCell = 0;

  // Selection of cell temperature
  if (tempCellMin > 0 && tempCellMax < 50)
  {
    temperatureCell = (tempCellMin + tempCellMax)/2;
  }
  else if(tempCellMax > 54)
  {
    temperatureCell = 54;
  }
  else if(tempCellMin < -19)
  {
    temperatureCell = -19;
  }
  else if(tempCellMin < 0)
  {
    temperatureCell = tempCellMin;
  }
  else if(tempCellMax > 50)
  {
    temperatureCell = tempCellMax;
  }

  // Range of BMS
  if (BMS_SOC < 1)
  {
    BMS_SOC = 1;
  }
  else if(BMS_SOC>99)
  {
    BMS_SOC = 99;
  }

  while (BMS_SOCFound == false)
  {
    if (BMS_SOC > MCC[0][indexx])
    {
      BMS_SOCFound = true;
      x1 =  MCC[0][indexx];
      x2 =  MCC[0][indexx+1]; // should be necessary to verify that it is not the end of the table
    }
    else
    {
      indexx = indexx + 1;
    }
  }

  while (temperatureFound == false)
  {
    if (temperatureCell > MCC[indexy][0])
    {
      temperatureFound = true;
      y1 =  MCC[indexy][0];
      y2 =  MCC[indexy+1][0]; // should be necessary to verify that it is not the end of the table
    }
    else
    {
      indexy = indexy + 1;
    }
  }

  dx = BMS_SOC - x1;
  dy = temperatureCell - y1;

  deltax = x2 - x1;
  deltay = y2 - y1;

  deltafx = MCC[indexy][indexx+1] - MCC[indexy][indexx];
  deltafy = MCC[indexy+1][indexx] - MCC[indexy][indexx];
  deltafxy = MCC[indexy][indexx] + MCC[indexy+1][indexx+1] - MCC[indexy][indexx+1] - MCC[indexy+1][indexx] ;

  MCCValue = deltafx * dx / deltax + deltafy * dy / deltay + deltafxy * (dx / deltax) * (dy/deltay) + MCC[indexy][indexx]; 

  return MCCValue;
}

int driveCurrentSelection (int TMDCValue, int MDCValue, int CAN_InverterCurrent, int *tempoMaxCurrent, int driveCurrentValueZ1)
{
  int driveCurrentValue = 0;

  //Incrementation of the temporisation
  if (CAN_InverterCurrent > MDCValue)
  {
    *tempoMaxCurrent = *tempoMaxCurrent +1;
  }
  else if (CAN_InverterCurrent < MDCValue && *tempoMaxCurrent > 0) //Decrementation of the temporisation
  {
    *tempoMaxCurrent = *tempoMaxCurrent -1;
  }

  if (*tempoMaxCurrent > 300) // Selection of MDC value as the tempo is above 300
  {
    driveCurrentValue = MDCValue;
  }
  else if(*tempoMaxCurrent == 0) // Selection of TMDC value as the tempo equals 0
  {
    driveCurrentValue = TMDCValue;
  }
  else // No modification of driveCurrentValue
  {
    driveCurrentValue = driveCurrentValueZ1;
  }

  return driveCurrentValue;
}




//////////////////////////////////////////////////////////


void readCycleFlash(int *cptCharge)
{  
	uint32_t b0address_to_read = 0x000000; // Adresse de départ dans la mémoire flash
	uint32_t b1address_to_read = 0x000001; // Adresse de départ dans la mémoire flash
	
	uint8_t b0;
	uint8_t b1;
	
	FlashSST25Read(b0address_to_read, &b0, sizeof(b0));
	FlashSST25Read(b1address_to_read, &b1, sizeof(b1));
	
	

    *cptCharge = b0*256 + b1;
  
}
void readConsoFlash(int *AverageConsumption, int *AverageGenerative)
{
  
	uint32_t b0address_to_read = 0x000002; 
	uint32_t b1address_to_read = 0x000003; 
	uint32_t b2address_to_read = 0x000004; 
	uint32_t b3address_to_read = 0x000005;
	
	uint8_t b0;
	uint8_t b1;
	uint8_t b2;
	uint8_t b3;
	
	FlashSST25Read(b0address_to_read, &b0, sizeof(b0));
	FlashSST25Read(b1address_to_read, &b1, sizeof(b1));
	FlashSST25Read(b2address_to_read, &b2, sizeof(b2));
	FlashSST25Read(b3address_to_read, &b3, sizeof(b3));
	
	*AverageGenerative = b0 * 256 + b1;
	
}
void readTimeDrivingFlash(int *cptTimeDriving)
{
	uint32_t b0address_to_read = 0x000006; 
	uint32_t b1address_to_read = 0x000007; 
	uint8_t b0;
	uint8_t b1;
	
	FlashSST25Read(b0address_to_read, &b0, sizeof(b0));
	FlashSST25Read(b1address_to_read, &b1, sizeof(b1));
	
	*cptTimeDriving = b0 * 256 + b1;
}
void readTimeChargingFlash(int *cptTimeCharging)
{
	uint32_t b0address_to_read = 0x000008; 
	uint32_t b1address_to_read = 0x000009; 
	uint8_t b0;
	uint8_t b1;
	
	FlashSST25Read(b0address_to_read, &b0, sizeof(b0));
	FlashSST25Read(b1address_to_read, &b1, sizeof(b1));
	
	*cptTimeCharging = b0 * 256 + b1;
}
void readWhFlash(int *cptChargeWh) 
{
	uint32_t b0address_to_read = 0x0000A; 
	uint32_t b1address_to_read = 0x0000B; 
	uint32_t b2address_to_read = 0x0000C; 
	uint32_t b3address_to_read = 0x0000D;
	
	uint8_t b0;
	uint8_t b1;
	uint8_t b2;
	uint8_t b3;
	
	FlashSST25Read(b0address_to_read, &b0, sizeof(b0));
	FlashSST25Read(b1address_to_read, &b1, sizeof(b1));
	FlashSST25Read(b2address_to_read, &b2, sizeof(b2));
	FlashSST25Read(b3address_to_read, &b3, sizeof(b3));
	
	*cptChargeWh = b0 * 16777216 + b1 * 65536 + b2 * 256 + b3;
}
void readSOCFlash(uint16_t * BMS_SOC_percentage, int * BMS_SOC)
{
  
	uint32_t b0address_to_read = 0x00000E; 
	uint32_t b1address_to_read = 0x00000F; 
	uint8_t b0;
	uint8_t b1;
	
	FlashSST25Read(b0address_to_read, &b0, sizeof(b0));
	FlashSST25Read(b1address_to_read, &b1, sizeof(b1));
	
	* BMS_SOC_percentage = b0 * 16 + b1;
	* BMS_SOC = * BMS_SOC_percentage * 378000 - 189000;
}
void ReadAllFlash(int *cptCharge, int *AverageConsumption, int *AverageGenerative, int *cptTimeCharging, int *cptTimeDriving, int *cptChargeWh, uint16_t *BMS_SOC_percentage, int * BMS_SOC)
{
	uint8_t data_to_read[16];
	uint32_t address_to_read = 0x000000; // Adresse de départ dans la mémoire flash
	uint32_t data_length = sizeof(data_to_read); // Longueur des données à écrire
	
	
	FlashSST25Read(address_to_read, data_to_read, data_length);
	*cptCharge = data_to_read[0] * 256 + data_to_read[1];
	*AverageGenerative = data_to_read[2] * 256 + data_to_read[3];
	*cptTimeDriving = data_to_read[6] * 256 + data_to_read[7];
	*cptTimeCharging = data_to_read[8] * 256 + data_to_read[9];
	*cptChargeWh = data_to_read[10] * 16777216 + data_to_read[11] * 65536 + data_to_read[12] * 256 + data_to_read[13];
	* BMS_SOC_percentage = data_to_read[14] * 16 + data_to_read[15];
	* BMS_SOC = * BMS_SOC_percentage * 378000 - 189000;
}
	
	
	



/*
void writeCycleFlash(int cptCharge)
{
	uint8_t data_to_write[] = { cptCharge / 256, cptCharge % 256 };
	uint32_t address_to_write = 0x000000; // Adresse de départ dans la mémoire flash
	uint32_t data_length = sizeof(data_to_write); // Longueur des données à écrire
	
	FlashSST25WriteWOErase(address_to_write, data_to_write, data_length);

}
void writeConsoFlash(int AverageConsumption, int AverageGenerative)
{

	uint8_t data_to_write[] = { AverageGenerative / 256, AverageGenerative % 256, AverageConsumption / 256, AverageConsumption % 256 };
	uint32_t address_to_write = 0x000005; // Adresse de départ dans la mémoire flash
	uint32_t data_length = sizeof(data_to_write); // Longueur des données à écrire
	
	FlashSST25WriteWOErase(address_to_write, data_to_write, data_length);
}
void writeTimeDrivingFlash(int cptTimeDriving)
{
 
	
	uint8_t data_to_write[] = { cptTimeDriving / 256, cptTimeDriving % 256 };
	uint32_t address_to_write = 0x000010; // Adresse de départ dans la mémoire flash
	uint32_t data_length = sizeof(data_to_write); // Longueur des données à écrire
	
	FlashSST25WriteWOErase(address_to_write, data_to_write, data_length);
}
void writeTimeChargingFlash(int cptTimeCharging)
{
	
	uint8_t data_to_write[] = { cptTimeCharging / 256, cptTimeCharging % 256 };
	uint32_t address_to_write = 0x000012; // Adresse de départ dans la mémoire flash
	uint32_t data_length = sizeof(data_to_write); // Longueur des données à écrire
	
	FlashSST25WriteWOErase(address_to_write, data_to_write, data_length);
  
}
void writeWhFlash(int cptChargeWh)
{	
	uint8_t data_to_write[] = { cptChargeWh / 16777216, cptChargeWh / 65536, cptChargeWh / 256, cptChargeWh % 256 };
	uint32_t address_to_write = 0x000015; // Adresse de départ dans la mémoire flash
	uint32_t data_length = sizeof(data_to_write); // Longueur des données à écrire
	
	FlashSST25WriteWOErase(address_to_write, data_to_write, data_length);
}
void writeSOCFlash(uint16_t BMS_SOC_percentage)
{
	
	uint8_t data_to_write[] = { BMS_SOC_percentage / 16, BMS_SOC_percentage % 16 };
	uint32_t address_to_write = 0x000020; // Adresse de départ dans la mémoire flash
	uint32_t data_length = sizeof(data_to_write); // Longueur des données à écrire
	
	FlashSST25WriteWOErase(address_to_write, data_to_write, data_length);
}


 

*/


void WriteAllFlash(int cptCharge, int AverageConsumption, int AverageGenerative ,int cptTimeCharging, int cptTimeDriving, int cptChargeWh, uint16_t BMS_SOC_percentage)
{
	uint8_t data_to_write[] = { cptCharge / 256, cptCharge % 256, AverageGenerative / 256, AverageGenerative % 256, AverageConsumption / 256, AverageConsumption % 256, cptTimeDriving / 256, cptTimeDriving % 256, cptTimeCharging / 256, cptTimeCharging % 256, cptChargeWh / 16777216, cptChargeWh / 65536, cptChargeWh / 256, cptChargeWh % 256, BMS_SOC_percentage / 256, BMS_SOC_percentage % 256 };
	uint32_t address_to_write = 0x000000; // Adresse de départ dans la mémoire flash
	uint32_t data_length = sizeof(data_to_write); // Longueur des données à écrire
	
	FlashSST25Write(address_to_write, data_to_write, data_length);
	}
