/*
 * AD7705.c
 *
 *  Created on: May 18, 2024
 *      Author: Pepe
 */

#include "AD7705.h"

eAD7705Registers eRegister;

/*********************************************************************
 * Actua sobre el pin de CS.
 * CS_ON / CS_OFF
 *********************************************************************/
void AD7705_CSSet(uint8_t state)
{
	HAL_GPIO_WritePin(CS_PIN_GPIO_Port, CS_PIN_Pin, state);
}


/*********************************************************************
 * Inicializa el AD7705.
 * Consulta el datasheet, en la carpeta "Docs"
 *********************************************************************/
void AD7705_Init(eChannel channel, eMode mode, eGain gain, eDataRate dataRate)
{
	uint8_t data[2];

	AD7705_CSSet(CS_ON);

	//El primer byte se escribe en el registro de comunicaciones.
	//1byte: Se indica cual va a ser el registro afectado, el canal leído, y si va a ser una acción de lectura o escritura.
	//2byte: Se inicializan los valores deseados en el registro afectado.

	//Seteo del clock Register
	data[0] = REG_CLOCK;
	data[0] |= channel;
	data[0] |= REG_WRITE;
	data[1] = 0;
	data[1] |= dataRate;
	if(HAL_SPI_Transmit(&hspi1, (uint8_t *)&data, 2, 100) != HAL_OK)
	{
		while(1)
			;
	}

	//Seteo del Setup register
	data[0] = REG_SETUP;
	data[0] |= channel;
	data[0] |= REG_WRITE;
	data[1] = 0;
	data[1] |= mode;
	data[1] |= gain;
	//data[1] |= 0x40; //Bipolar mode
	if(HAL_SPI_Transmit(&hspi1, (uint8_t *)&data, 2, 100) != HAL_OK)
	{
		while(1)
			;
	}

	AD7705_CSSet(CS_OFF);
}


/*********************************************************************
 * Devuelve el estado (negado) del DRDY, que nos indica si hay una nueva lectura disponible.
 * Devuelve "1" si hay nueva lectura.
 * Devuelve "0" si no hay una nueva lectura.
 *********************************************************************/
uint8_t AD7705_GetDRDY(void)
{
	return(!HAL_GPIO_ReadPin(DRDY_PIN_GPIO_PORT, DRDY_PIN_PIN));
}


/*********************************************************************
 * Devuelve el último valor de ADC generado por el AD7705.
 *********************************************************************/
uint16_t AD7705_ReadADC(void)
{
	uint8_t data;
	uint16_t readValue;

	AD7705_CSSet(CS_ON);

	//Seteo del Data Register para obtener una lectura del ADC
	data = REG_DATA;
	data |= REG_READ;
	if(HAL_SPI_Transmit(&hspi1, &data, 1, 100) != HAL_OK)
	{
		while(1)
			;
	}

	//Se obtiene la lectura del último valor de conversión del ADC.
	if(HAL_SPI_Receive(&hspi1, (uint8_t *)&readValue, 2, 100) != HAL_OK)
	{
		while(1)
			;
	}

	AD7705_CSSet(CS_OFF);

	return(readValue);
}
