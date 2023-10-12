#include "main.h"
#include "CAN_Retrofit.h"
#include "adc.h"



#define BalancingCellVoltage 34500
#define BalancingPhaseVoltage 35000

bool chargingCableDetected(int Charger_PlugPresence, int Charger_SectorPresence);
void chargingVehicle(bool activation, bool cellBalancingStatus, int voltageCellMax, int chargerCurrent, int chargerVoltage);
int checkchargingPPValue(void);
int voltageRequestComput(int voltageCellMax);

