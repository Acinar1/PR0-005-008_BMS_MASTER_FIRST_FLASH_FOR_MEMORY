#include "charger.h"
#include "math.h"
#include "fdcan.h"

static int chargingVoltageRequest = 540;
static int periodicityCharging = 0;

int max(int a, int b) {
	// Si a est plus grand que b, renvoyer a
	if (a > b) {
		return a;
	}
	// Sinon, renvoyer b
	else {
		return b;
	}
}
bool chargingCableDetected(int Charger_PlugPresence, int Charger_SectorPresence)
{
  bool chargingCableStatus = false;
 
	if (Charger_PlugPresence==1 || Charger_SectorPresence==1 ){
		
    chargingCableStatus = true;
  }
  else
  {
    chargingCableStatus = false;
  }
  
  return chargingCableStatus;
}

void chargingVehicle(bool activation, bool cellBalancingStatus, int voltageCellMax, int chargerCurrent, int chargerVoltage)
{
  int currentRqst = 0;
	
    currentRqst = 300; //Max input current RMS : 16A(230V) 

  if (activation == true && cellBalancingStatus == false)
  {  
    
	  //HAL_GPIO_WritePin(ModifImpedanceCP_GPIO_Port, ModifImpedanceCP_Pin, GPIO_PIN_SET);

    if (periodicityCharging > 60)
    {
      periodicityCharging = 0;
      if ((currentRqst - chargerCurrent*10) > 12 && (chargingVoltageRequest - chargerVoltage) < 4 && chargerCurrent > 1)
      {
        chargingVoltageRequest = max(chargingVoltageRequest+voltageRequestComput(voltageCellMax),530);
      }
      else if (voltageCellMax > 34850)
      {
        chargingVoltageRequest = max(chargingVoltageRequest+voltageRequestComput(voltageCellMax),530);
      }
      else if (chargerCurrent < 2 )
      {
        chargingVoltageRequest = chargingVoltageRequest+7;        
      }
    }
    CAN_send_periodic_charger(chargingVoltageRequest, currentRqst, true);

    periodicityCharging = periodicityCharging + 1;
  }
  else if(activation == true && cellBalancingStatus == true)
  {
	 // HAL_GPIO_WritePin(ModifImpedanceCP_GPIO_Port, ModifImpedanceCP_Pin, GPIO_PIN_SET);

    CAN_send_periodic_charger(584, 7, true);
  }
  else if(activation == false && cellBalancingStatus == true)
  {
	 // HAL_GPIO_WritePin(ModifImpedanceCP_GPIO_Port, ModifImpedanceCP_Pin, GPIO_PIN_SET);
	  
    CAN_send_periodic_charger(584, 0, false);
    chargingVoltageRequest = 584;
  }
  else 
  {
	  //HAL_GPIO_WritePin(ModifImpedanceCP_GPIO_Port, ModifImpedanceCP_Pin, GPIO_PIN_RESET);

    CAN_send_periodic_charger(584, 0, false);
    chargingVoltageRequest = 584;
  }
}


int voltageRequestComput(int voltageCellMax)
{
  int incrementVoltageRequest = 0;

  if (voltageCellMax < 34500)
  {
    incrementVoltageRequest = 1;
  }
  else if (voltageCellMax > 34850)
  {
    incrementVoltageRequest = -5;
  }
  else
  {
    incrementVoltageRequest = 0;
  }

  return incrementVoltageRequest;
}
