#include "main.h"

bool checkMCUPresence(void);

void CAN_send_periodic_Consumption(int AverageConsumption, int AverageGenerative, int Range, int Conso);
	
void CAN_send_periodic_Charge(int cptCharge, int cptChargekWh, int cptTimeCharging);
	
void CAN_sendCptTimeDriving(int cptTimeDriving);
	
void CAN_send_periodic_Speed(int Vehicle_Speed);
	
void CAN_send_periodic_BMS_temp_voltage(int tempCellMax, int idTempCellMax, int tempCellMin, int idTempCellMin, unsigned short voltageCellMax, int idVoltageCellMax, unsigned short voltageCellMin, int idVoltageCellMin);
	
void CAN_send_periodic_BMS_status(int BMS_ErrorId, int BMS_State, int BMS_SOC_percentage, int * HallEffectCurrent, int batteryVoltage);
	
void CAN_send_periodic_charger(int voltage, int current, bool ChargerEnable);
	
void CAN_sendVersionBMS(int VERSION_BMS_MASTER_MAJ, int VERSION_BMS_MASTER_MIN);
	
void CAN_send_periodic_inverter(int currentLimitDrive, int currentLimitRegen);

//////////////////////////////////////////////////////

void gotInverterFrameStatus(uint8_t u8CanNum);
void gotFrameInverter(uint8_t u8CanNum);
void  gotMCUStatusFrame(uint8_t u8CanNum);
void gotWheelSpeedFrame(uint8_t u8CanNum);
void gotFrameDCDC(uint8_t u8CanNum);
void gotChargerFrame(uint8_t u8CanNum);
void gotChargerState(uint8_t u8CanNum);
void gotMCUFrameStatus(uint8_t u8CanNum);