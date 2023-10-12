#include "Can_BMS.h"
#include "fdcan.h"


void cellCommand_send_CAN(bool activation, volatile int BMS_Balancing[][8])
{
	int indexModule = 0;
	int indexCell = 0;
	uint8_t u8CanNum = 1; 
	uint32_t u32_ID = 0;
	uint8_t data[2];

	for (indexModule = 0; indexModule < 12; indexModule++)
	{
		u32_ID = 0x100 | indexModule;
		data[0] = 0;
		data[1] = 0;
		if (activation == true)
		{
			for (indexCell = 0; indexCell < 8; indexCell++)
			{
				if (BMS_Balancing[indexModule][indexCell] == 1)
				{
					data[1] = data[1] | (1 << indexCell);
				}
			}
		}
		else
		{
			data[1] = 0;
		}
		CAN_IT_put(u8CanNum, u32_ID, data, sizeof(data));
	}
}
void gotBMSFrameVoltage(uint8_t u8CanNum) {
	int indexTab = 0;
	int FirstCellVoltageIndex = 0;
	int indexDataFrame = 0;
	int CellVoltage = 0;
  
	// Processing frame regarding voltage
	if (aoLastCanMsg[u8CanNum].u29_ID >= 0x300 && aoLastCanMsg[u8CanNum].u29_ID <= 0x320) // Frame containig voltage cell
	{
		indexTab = ((aoLastCanMsg[u8CanNum].u29_ID & 0x00F) % 0x0C); // Retrieve the number of the module
		//Serial.println(indexTab);
		if ((aoLastCanMsg[u8CanNum].u29_ID & 0x10) == 0x10)
		{
			FirstCellVoltageIndex = 4;
		}
		if (indexTab < 12 && aoLastCanMsg[u8CanNum].u8_DataLen == 8) // Frame addresse should not be higher than 12 and the frame length should be equal to 8 bytes.
		{
			for (indexDataFrame = 0; indexDataFrame < 4; indexDataFrame++)
			{
				CellVoltage = (aoLastCanMsg[u8CanNum].au8_Data[indexDataFrame * 2 + 1] * 256 + aoLastCanMsg[u8CanNum].au8_Data[indexDataFrame * 2]) * 6.1035; // Voltage is computed to be 1 unit = 0.1mV
				//Serial.println(CellVoltage);
				if (CellVoltage > 23000 && CellVoltage < 40000)
				{
					if (BMS_Voltage[indexTab][FirstCellVoltageIndex + indexDataFrame] != 0)
					{
						BMS_Voltage[indexTab][FirstCellVoltageIndex + indexDataFrame] = (BMS_Voltage[indexTab][FirstCellVoltageIndex + indexDataFrame] + CellVoltage) / 2;
					}
					else
					{
						BMS_Voltage[indexTab][FirstCellVoltageIndex + indexDataFrame] = CellVoltage;
					}
             

				}
 
			}
		}
	}
}
void gotBMSFrameTemperature(uint8_t u8CanNum) {
	
	int indexTab = 0;
	int indexDataFrame = 0;
	int TempVoltage = 0;
	int CellTemperature = 0;
	int CellTempIndex = 0;
	bool tempFound = false;
  
  
	// Processing frame regarding temperature
	if (aoLastCanMsg[u8CanNum].u29_ID >= 0x400 && aoLastCanMsg[u8CanNum].u29_ID <= 0x40B)
	{
		indexTab = ((aoLastCanMsg[u8CanNum].u29_ID & 0x00F) % 0x0C); // Retrieve the number of the module
		if (aoLastCanMsg[u8CanNum].u8_DataLen == 8) // Frame length should be equal to 8 bytes.
		{
			for (indexDataFrame = 0; indexDataFrame < 4; indexDataFrame++)
			{
				CellTempIndex = 0;
				tempFound = false;
          
				TempVoltage = (aoLastCanMsg[u8CanNum].au8_Data[indexDataFrame * 2 + 1] * 256 + aoLastCanMsg[u8CanNum].au8_Data[indexDataFrame * 2]) * 1.52589; // Computation of corresponding temperature
				while (tempFound == false)
				{
					if (TempVoltage > tempVoltageRelation[CellTempIndex][1] || CellTempIndex == 80)
					{
						tempFound = true;
						CellTemperature = tempVoltageRelation[CellTempIndex][0];
					}
					else
					{
						CellTempIndex++;
					}
				}
				if (indexTab == 4 && indexDataFrame == 2)
				{
					BMS_Temperature[indexTab][indexDataFrame] = BMS_Temperature[4][1]; 
				}
				else if (indexTab == 4 && indexDataFrame == 3)
				{
					BMS_Temperature[indexTab][indexDataFrame] = BMS_Temperature[4][1];             
				}
				else if (CellTemperature > -20 || CellTemperature < 60) // if temperature is equal -20°C or 60°C, it means there is a measurement failure
				{
					BMS_Temperature[indexTab][indexDataFrame] = CellTemperature; 
				}
          
				if (CellTemperature > 55)
				{
					CellOverheat = true;
				}
			}
		}
	}
  
}