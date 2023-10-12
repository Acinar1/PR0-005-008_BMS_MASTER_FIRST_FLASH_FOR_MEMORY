
#include "math.h"
#include "main.h"

#include "adc.h"
#include "SST25VF080.h"


static int rawAnaInput_LowCurrent = 0;
static int rawAnaInput_HighCurrent = 0;
static int MCC[7][5] = { 
	{ 0, 0, 70, 90, 100 },
	{ -20, 50, 50, 50, 0 },
	{ 0, 50, 50, 50, 0 },
	{ 5, 72, 72, 72, 50 },
	{ 10, 100, 100, 100, 50 },
	{ 50, 100, 100, 100, 50 },
	{ 55, 10, 10, 10, 10 },
};

static int MDC[7][6] = {
	{ 0, 0, 10, 20, 30, 100 },
	{ -20, 0, 100, 100, 100, 100 },
	{ 0, 0, 100, 100, 100, 100 }, 
	{ 5, 0, 100, 150, 300, 300 },
	{ 10, 0, 150, 300, 300, 300 },
	{ 50, 0, 150, 300, 300, 300 },
	{ 55, 0, 100, 100, 100, 100 },
};

static int TMDC[7][6] = {
	{ 0, 0, 10, 20, 30, 100 },
	{ -20, 0, 200, 200, 200, 200 },
	{ 0, 0, 200, 200, 200, 200 }, 
	{ 5, 0, 200, 300, 500, 600 },
	{ 10, 0, 300, 500, 500, 600 },
	{ 50, 0, 300, 500, 500, 600 },
	{ 55, 0, 200, 200, 200, 200 },
};

// Maximum Charge Current for SOC 0, 70, 90 and 100%


void computeSOC(int * BMS_SOC, uint16_t * BMS_SOC_percentage, int regenCurrent, int DCDC_Current, int * HallEffectCurrent, int * LowCurrentValue, int *HighCurrentValue, int Charger_Current, int BMS_State);
int computeTMDC(int BMS_SOC, int tempCellMin, int tempCellMax);
int computeMCC(int BMS_SOC, int tempCellMin, int tempCellMax);
int computeMDC(int BMS_SOC, int tempCellMin, int tempCellMax);
int driveCurrentSelection (int TMDCValue, int MDCValue, int CAN_InverterCurrent, int *tempoMaxCurrent, int driveCurrentValueZ1);
//void writeSOCFlash(uint16_t BMS_SOC_percentage);
//void writeCycleFlash(int cptCharge);
void readCycleFlash(int *cptCharge);
//void writeWhFlash(int cptChargeWh);
void readWhFlash(int *cptChargeWh);
void readSOCFlash(uint16_t * BMS_SOC_percentage,  int * BMS_SOC);
void readConsoFlash(int *AverageConsumption, int *AverageGenerative);
//void writeConsoFlash(int AverageConsumption, int AverageGenerative);
void readTimeChargingFlash(int *cptTimeCharging);
//void writeTimeChargingFlash(int cptTimeCharging);
void readTimeDrivingFlash(int *cptTimeDriving);
//void writeTimeDrivingFlash(int cptTimeDriving);
void WriteAllFlash(int cptCharge, int AverageConsumption, int AverageGenerative, int cptTimeCharging, int cptTimeDriving, int cptChargeWh, uint16_t BMS_SOC_percentage);
void ReadAllFlash(int *cptCharge, int *AverageConsumption, int *AverageGenerative, int *cptTimeCharging, int *cptTimeDriving, int *cptChargeWh, uint16_t *BMS_SOC_percentage, int * BMS_SOC);