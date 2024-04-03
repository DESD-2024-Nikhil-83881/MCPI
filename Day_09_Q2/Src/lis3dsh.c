/*
 * lis3dsh.c
 *
 *  Created on: Apr 3, 2024
 *      Author: nikhil
 */

#include "lis3dsh.h"

void LIS_WriteIO(uint8_t reg, uint8_t data[], uint8_t size) {

	SPI_CS_Enable();
	SPI_Transmit(reg);
	for(int i=0; i<size; i++)
		SPI_Transmit(data[i]);
	SPI_CS_Disable();
}

void LIS_ReadIO(uint8_t reg, uint8_t data[], uint8_t size) {
	reg |= BV(7);
	SPI_CS_Enable();
	SPI_Transmit(reg);
	for(int i=0; i<size; i++)
		data[i] = SPI_Receive();
	SPI_CS_Disable();
}

void LIS_Init(void) {
	SPI_Init();
	DelayMs(3);
	uint8_t config = (LIS_DATARATE_25 | LIS_XYZ_ENABLE);
	LIS_WriteIO(LIS_CTRL_REG4_ADDR, &config, 1);
	DelayMs(3);
}

LIS_Data LIS_GetData(void) {
	uint8_t data[2];
	uint16_t x, y, z;
	LIS_ReadIO(LIS_OUT_X_L_ADDR, data, 2);
	x = ((uint16_t)data[1] << 8) | data[0];
	LIS_ReadIO(LIS_OUT_Y_L_ADDR, data, 2);
	y = ((uint16_t)data[1] << 8) | data[0];
	LIS_ReadIO(LIS_OUT_Z_L_ADDR, data, 2);
	z = ((uint16_t)data[1] << 8) | data[0];
	LIS_Data val;
	val.x = x;
	val.y = y;
	val.z = z;
	return val;
}

int LIS_DRdy(void) {
	uint8_t status;
	do {
		LIS_ReadIO(LIS_STATUS_ADDR, &status, 1);
	} while( (status & LIS_STATUS_XYZ_Msk) == 0 );
	return 1;
}





