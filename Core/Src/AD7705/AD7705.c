/*
 * AD7705.c
 *
 *  Created on: May 18, 2024
 *      Author: Pepe
 */

#include "AD7705.h"

eAD7705Registers eRegister;

void AD7705_CSSet(uint8_t state)
{
	HAL_GPIO_WritePin(CS_PIN_GPIO_Port, CS_PIN_Pin, state);
}

void AD7705_Init(eChannel channel, eMode mode, eGain gain, eDataRate dataRate)
{
	uint8_t data;

	AD7705_CSSet(CS_ON);

	data = 0;
	data |= REG_CLOCK;
	data |= channel;
	data |= REG_WRITE;
	if(HAL_SPI_Transmit(&hspi1, &data, 1, 100) != HAL_OK)
	{
		while(1)
			;
	}

	data = 0;
	data |= dataRate;
	//data |= 0x08; //Clock register CLKDIV = 1 (because the 4.9152MHz crystyal??)
	if(HAL_SPI_Transmit(&hspi1, &data, 1, 100) != HAL_OK)
	{
		while(1)
			;
	}

	data = 0;
	data |= REG_SETUP;
	data |= channel;
	data |= REG_WRITE;
	if(HAL_SPI_Transmit(&hspi1, &data, 1, 100) != HAL_OK)
	{
		while(1)
			;
	}

	data = 0;
	data |= mode;
	data |= gain;
	data |= 0x40; //Bipolar mode
	if(HAL_SPI_Transmit(&hspi1, &data, 1, 100) != HAL_OK)
	{
		while(1)
			;
	}

	AD7705_CSSet(CS_OFF);
}

uint8_t AD7705_GetDRDY(void)
{
	return(!HAL_GPIO_ReadPin(DRDY_PIN_GPIO_PORT, DRDY_PIN_PIN));
}

uint16_t AD7705_ReadADC(void)
{
	uint8_t data;
	uint16_t readValue;

	AD7705_CSSet(CS_ON);

	data = 0;
	data |= REG_DATA;
	data |= REG_READ;
	if(HAL_SPI_Transmit(&hspi1, &data, 1, 100) != HAL_OK)
	{
		while(1)
			;
	}
	if(HAL_SPI_Receive(&hspi1, (uint8_t *)&readValue, 2, 100) != HAL_OK)
	{
		while(1)
			;
	}
	AD7705_CSSet(CS_OFF);
	return(readValue);
}
