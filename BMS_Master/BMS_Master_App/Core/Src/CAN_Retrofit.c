#include "CAN_Retrofit.h"
#include "fdcan.h"


bool checkMCUPresence(void)
{
  return true;
}
///////////////////////////////////////////////////
void CAN_send_periodic_Consumption(int AverageConsumption, int AverageGenerative, int Range, int Conso) 
{
	uint8_t u8CanNum = 2; 
	uint32_t u32_ID = 0x706; 
	
	// Créez un tableau pour stocker les données à envoyer
	uint8_t data[8];

	// Remplissez le tableau avec les valeurs à envoyer
	data[0] = AverageConsumption % 256;
	data[1] = AverageConsumption / 256;
	data[2] = AverageGenerative % 256;
	data[3] = AverageGenerative / 256;
	data[4] = Range % 256;
	data[5] = Range / 256;
	data[6] = Conso % 256;
	data[7] = Conso / 256;

	// Appelez la fonction CAN_IT_put pour envoyer les données
	CAN_IT_put(u8CanNum, u32_ID, data, sizeof(data));
}
void CAN_send_periodic_Charge(int cptCharge, int cptChargekWh, int cptTimeCharging) 
{
	uint8_t u8CanNum = 2; 
	uint32_t u32_ID = 0x709; 
	
	// Créez un tableau pour stocker les données à envoyer
	uint8_t data[6];

	// Remplissez le tableau avec les valeurs à envoyer
	data[0] = cptCharge % 256;
	data[1] = cptCharge / 256;
	data[2] = cptChargekWh % 256;
	data[3] = cptChargekWh / 256;
	data[4] = (cptTimeCharging / 60) % 256;
	data[5] = (cptTimeCharging / 60) / 256;
	

	// Appelez la fonction CAN_IT_put pour envoyer les données
	CAN_IT_put(u8CanNum, u32_ID, data, sizeof(data));
}
void CAN_sendCptTimeDriving(int cptTimeDriving)
{
	uint8_t u8CanNum = 2; 
	uint32_t u32_ID = 0x710; 
	
	// Créez un tableau pour stocker les données à envoyer
	uint8_t data[2];

	// Remplissez le tableau avec les valeurs à envoyer
	data[0] = (cptTimeDriving / 60) % 256;
	data[1] = (cptTimeDriving / 60) / 256;
	
	

	// Appelez la fonction CAN_IT_put pour envoyer les données
	CAN_IT_put(u8CanNum, u32_ID, data, sizeof(data));
}
void CAN_send_periodic_Speed(int Vehicle_Speed)
{
	uint8_t u8CanNum = 2; 
	uint32_t u32_ID = 0x707; 
	
	// Créez un tableau pour stocker les données à envoyer
	uint8_t data[1];

	// Remplissez le tableau avec les valeurs à envoyer
	data[0] = Vehicle_Speed;
	
	
	

	// Appelez la fonction CAN_IT_put pour envoyer les données
	CAN_IT_put(u8CanNum, u32_ID, data, sizeof(data));
}
void CAN_send_periodic_BMS_temp_voltage(int tempCellMax, int idTempCellMax, int tempCellMin, int idTempCellMin, unsigned short voltageCellMax, int idVoltageCellMax, unsigned short voltageCellMin, int idVoltageCellMin)
{
	uint8_t u8CanNum = 2; 
	uint32_t u32_ID = 0x701; 
	uint32_t u32_ID2 = 0x702;
	
	// Créez un tableau pour stocker les données à envoyer
	uint8_t data[4];
	uint8_t data1[6];

	// Remplissez le tableau avec les valeurs à envoyer
	data[0] = tempCellMax;
	data[1] = idTempCellMax;
	data[2] = tempCellMin;
	data[3] = idTempCellMin;
	
	data1[0] = voltageCellMax % 256;
	data1[1] = voltageCellMax / 256;
	data1[2] = voltageCellMin % 256;
	data1[3] = voltageCellMin / 256;
	data1[4] = idVoltageCellMax;
	data1[5] = idVoltageCellMin;

	// Appelez la fonction CAN_IT_put pour envoyer les données
	CAN_IT_put(u8CanNum, u32_ID, data, sizeof(data));
	CAN_IT_put(u8CanNum, u32_ID2, data1, sizeof(data));
}
void CAN_send_periodic_BMS_status(int BMS_ErrorId, int BMS_State, int BMS_SOC_percentage, int * HallEffectCurrent, int batteryVoltage)
{
	uint8_t u8CanNum = 2; 
	uint32_t u32_ID = 0x700; 
	
	// Créez un tableau pour stocker les données à envoyer
	uint8_t data[7];
	
	int batteryVoltage_temp = 0;

	batteryVoltage_temp = batteryVoltage / 100;
	data[0] = *HallEffectCurrent % 256;
	if (*HallEffectCurrent < 0)
	{
		data[1] = (*HallEffectCurrent / 256) - 1;
	}
	else
	{
		data[1] = (*HallEffectCurrent / 256);
	} 
	// Remplissez le tableau avec les valeurs à envoyer
	data[2] = BMS_SOC_percentage; // SOC in %
	data[3] = BMS_ErrorId;
	data[4] = BMS_State; 
	data[5] = batteryVoltage_temp % 256;
	data[6] = batteryVoltage_temp / 256;
	

	// Appelez la fonction CAN_IT_put pour envoyer les données
	CAN_IT_put(u8CanNum, u32_ID, data, sizeof(data));
	
}
void CAN_send_periodic_charger(int voltage0, int current0, bool ChargerEnable)
{
	uint8_t u8CanNum = 3; 
	uint32_t u32_ID = 630; 
	
	// Créez un tableau pour stocker les données à envoyer
	uint8_t data[8];
	int current1 = current0 * 1.6;
	int voltage1 = voltage0 * 1.6;
	// Remplissez le tableau avec les valeurs à envoyer
	data[0] = voltage1 * current1; //Puissance 
	data[1] = (current1  & 0xFF00) >> 8;
	data[2] = (current1  & 0x00FF);
	data[3] = ((voltage1 & 0xFF00) >> 8);
	data[4] = (voltage1 & 0x00FF);
	
	if (ChargerEnable == true) {
		
		data[5] = 2;
		
	}
	else {
		data[5] = 0;
	}
	data[6] =  BMS_SOC_percentage;
	data[7] =  0x09; 
	

	// Appelez la fonction CAN_IT_put pour envoyer les données
	CAN_IT_put(u8CanNum, u32_ID, data, sizeof(data));
}
void CAN_sendVersionBMS(int VERSION_BMS_MASTER_MAJ, int VERSION_BMS_MASTER_MIN)
{
	uint8_t u8CanNum = 2; 
	uint32_t u32_ID = 0x021; 
	
	// Créez un tableau pour stocker les données à envoyer
	uint8_t data[2];

	// Remplissez le tableau avec les valeurs à envoyer
	data[0] = VERSION_BMS_MASTER_MAJ;
	data[0] = VERSION_BMS_MASTER_MIN; 
	
	
	

	// Appelez la fonction CAN_IT_put pour envoyer les données
	CAN_IT_put(u8CanNum, u32_ID, data, sizeof(data));
}
void CAN_send_periodic_inverter(int currentLimitDrive, int currentLimitRegen)
{
	uint8_t u8CanNum = 2; 
	uint32_t u32_ID = 0x326; 
	
	// Créez un tableau pour stocker les données à envoyer
	uint8_t data[8];

	// Remplissez le tableau avec les valeurs à envoyer
	data[0] = (200 * 10) % 256;
	data[1] = (200 * 10) / 256;
	data[2] = (105 * 10) % 256;
	data[3] = (105 * 10) / 256;
	data[4] = 0x00 % 256;
	data[5] =  0x00 / 256;
	data[6] = 0xFF % 256;
	data[7] = 0xFF / 256;

	// Appelez la fonction CAN_IT_put pour envoyer les données
	CAN_IT_put(u8CanNum, u32_ID, data, sizeof(data));
}
///////////////////////////////////////////////////
void gotInverterFrameStatus(uint8_t u8CanNum) {
	if (aoLastCanMsg[u8CanNum].u29_ID == 0x726) {
		
			
		inverterStatus = 1;

		// Utilisez chargerVoltage et chargerCurrent comme nécessaire ici
		
	}
}
void gotFrameInverter(uint8_t u8CanNum) {
	
	if (aoLastCanMsg[u8CanNum].u29_ID == 0x1A6)
	{
		engineSpeedRawCAN = ((aoLastCanMsg[u8CanNum].au8_Data[5] & 0x7F) * 256) + aoLastCanMsg[u8CanNum].au8_Data[4];
 
		if (engineSpeedRawCAN > 32000) // cas où la vitesse est vue négative par le controleur
		{
			engineSpeedRawCAN = 0;
		}
	}

	if (aoLastCanMsg[u8CanNum].u29_ID == 0x2A6)
	{
		CAN_InverterCurrent = (aoLastCanMsg[u8CanNum].au8_Data[7] * 256 + aoLastCanMsg[u8CanNum].au8_Data[6]) / 10;

		CAN_CapaVoltage = (aoLastCanMsg[u8CanNum].au8_Data[3] * 4 + aoLastCanMsg[u8CanNum].au8_Data[2] * 0.015625);
		cptVolt++;
		averageVoltageTemp += CAN_CapaVoltage;

		if ((aoLastCanMsg[u8CanNum].au8_Data[0] & 0x04) == 0x04)
		{
			regenCurrent = 1;
		}
		else
		{
			regenCurrent = 0;
		}
	}
}
void  gotMCUStatusFrame(uint8_t u8CanNum) {
	if (aoLastCanMsg[u8CanNum].u29_ID == 0x704) {
		
		CAN_Speedometer = ((aoLastCanMsg[u8CanNum].au8_Data[5]) + (aoLastCanMsg[u8CanNum].au8_Data[6]) * 256 + (aoLastCanMsg[u8CanNum].au8_Data[7]) * 65536);

		// Utilisez chargerVoltage et chargerCurrent comme nécessaire ici
		
	}
}
void gotWheelSpeedFrame(uint8_t u8CanNum) {
	if (aoLastCanMsg[u8CanNum].u29_ID == 0x705) {
		
			
		CAN_WheelSpeed = ((aoLastCanMsg[u8CanNum].au8_Data[0]) + (aoLastCanMsg[u8CanNum].au8_Data[1] * 256));
		averageSpeedTemp += CAN_WheelSpeed;
		cptSpeed++;

		// Utilisez chargerVoltage et chargerCurrent comme nécessaire ici
		
	}
}
void gotFrameDCDC(uint8_t u8CanNum) {
	if (aoLastCanMsg[u8CanNum].u29_ID == 0x670) {
		
		DCDC_Current = ((aoLastCanMsg[u8CanNum].au8_Data[1] * 10) + aoLastCanMsg[u8CanNum].au8_Data[0]/25.6);

		// Utilisez chargerVoltage et chargerCurrent comme nécessaire ici
		
	}
}
void gotChargerFrame(uint8_t u8CanNum) {
	if (aoLastCanMsg[u8CanNum].u29_ID == 0x620) {
		
		chargerVoltage = (aoLastCanMsg[u8CanNum].au8_Data[2]*10) + (aoLastCanMsg[u8CanNum].au8_Data[3]/25.6);
		chargerCurrent = ((aoLastCanMsg[u8CanNum].au8_Data[0]*10) + (aoLastCanMsg[u8CanNum].au8_Data[1] / 25.6));

		// Utilisez chargerVoltage et chargerCurrent comme nécessaire ici
		
	}
}
void gotChargerState(uint8_t u8CanNum) {
	if (aoLastCanMsg[u8CanNum].u29_ID == 0x610) {
		
		//chargerVoltage = (aoLastCanMsg[u8CanNum].au8_Data[2]) + aoLastCanMsg[u8CanNum].au8_Data[3] * 256;
		//chargerCurrent = ((aoLastCanMsg[u8CanNum].au8_Data[0]) + aoLastCanMsg[u8CanNum].au8_Data[1] * 256);
		Charger_State = aoLastCanMsg[u8CanNum].au8_Data[0];
		Charger_SectorPresence = ((aoLastCanMsg[u8CanNum].au8_Data[1] & 0x80) >> 7);
		Charger_PlugPresence = ((aoLastCanMsg[u8CanNum].au8_Data[1] & 0x40) >> 6);
		Charger_Power = ((aoLastCanMsg[u8CanNum].au8_Data[1] & 0x30) >> 4);
		//Failure=
		// 
		
	}
}
void gotMCUFrameStatus(uint8_t u8CanNum) {
	if (aoLastCanMsg[u8CanNum].u29_ID == 0x10) {
		
		MCUStatus = 1;
	}
}