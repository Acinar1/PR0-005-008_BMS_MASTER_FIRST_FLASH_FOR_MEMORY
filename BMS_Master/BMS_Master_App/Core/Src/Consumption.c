#include "Consumption.h"

void computeConsumption(uint16_t BMS_SOC_percentage, int *InstantConsumption, int *AverageConsumption, int averageVoltage, int averageSpeed, int averageCurrent, int *Vehicle_Speed, int *Range, int *InstantGenerative, int *AverageGenerative, int averageRegenCurrent, int *Conso)
{               
  int DeltaConsumption = 0;
  int DeltaGenerative = 0;
  int coeff = 500; //plus il est haut, moins la conso et le range varient

  *Vehicle_Speed = averageSpeed*10 / 157; //speed computation (x10)

  /////////////////////////////////// Consuming ////////////////////////////////////////////////////

    if (*Vehicle_Speed != 0 && averageCurrent > 0) //to compute only when we're not in regen case
    { 
      if(*Vehicle_Speed > 10) //to not consider cases where we're really slow which would "fake" the averaging (> 2km/h only)
      {
        *InstantConsumption = 100*averageCurrent * averageVoltage / *Vehicle_Speed; //instant consumption computation kWh/m (x100)
      }
      else { *InstantConsumption = 0; }
      
    } // = 10 U*I/v(km/h) //real computation of InstantConsumption (with no x100)
    else { *InstantConsumption = 0; }
    
    if(*Vehicle_Speed > 10)
    { 
      
      if(*InstantConsumption == 0) //if regen state -> average consumption should stay the same => delta = 0
      {
        DeltaConsumption = 0;
      }
      else
      {
        DeltaConsumption = *AverageConsumption - *InstantConsumption;
      }
      *AverageConsumption = ((*AverageConsumption*10)-((DeltaConsumption*10)/coeff))/10; //average consumption computation kWh (x100) 
      
    }

    ///////////////////////////////////////////////// Regen /////////////////////////////////////////////:

    if (*Vehicle_Speed != 0 && averageRegenCurrent > 0) //to compute in regen case only
    {
      if(*Vehicle_Speed > 10)
      {
      *InstantGenerative = (100*averageRegenCurrent * averageVoltage)/ *Vehicle_Speed; //consumption generative computation (x100) same as InstantConsumption
      }
      else { *InstantGenerative = 0; }
    }
    else { *InstantGenerative = 0; }
      
    if(*Vehicle_Speed > 10)
    {
      if(*InstantGenerative == 0)//if we are on consumption mode (regenCurrent = 0), averageGenerative shouldn't be lowering
      {
        DeltaGenerative = 0;
      }
      else 
      {
        DeltaGenerative = *AverageGenerative - *InstantGenerative;
      }

      *AverageGenerative = ((*AverageGenerative*10) - ((DeltaGenerative*10)/coeff))/10; //average generation consumption computation (x100)
    }

    *Conso = *AverageConsumption - *AverageGenerative;

  if(*Conso != 0)
    {
      *Range = BMS_SOC_percentage * 20000 / *Conso; //range computation (x10)
    }  //Range = 20*SOC / Conso_Moyenne (Conso - Regen) -> on a *20000 car Range(x10) et les Conso (x100)
    else
    {
      *Range = 0;
    }

}//this function returns instant consumption, average one and range. It also computes and returns regen ones if we are in regen mode
