/*
 * AD7705.h
 *
 *  Created on: May 18, 2024
 *      Author: Pepe
 */

#ifndef SRC_AD7705_AD7705_H_
#define SRC_AD7705_AD7705_H_

#include "main.h"

#define CS_PIN_GPIO_Port   AD7705_CS_GPIO_Port
#define CS_PIN_Pin         AD7705_CS_Pin
#define DRDY_PIN_GPIO_PORT AD7705_DRDY_GPIO_Port
#define DRDY_PIN_PIN       AD7705_DRDY_Pin

#define CS_ON  0
#define CS_OFF 1
#define REG_WRITE  0x00
#define REG_READ   0x08

enum
{
	REG_COMM   = 0x00,
	REG_SETUP  = 0x10,
	REG_CLOCK  = 0x20,
	REG_DATA   = 0x30,
	REG_TEST   = 0x40,
	REG_NOP    = 0x50,
	REG_OFFSET = 0x60,
	REG_GAIN   = 0x70
}typedef eAD7705Registers;

enum
{
	GAINx1   = 0x00,
	GAINx2   = 0x08,
	GAINx4   = 0x10,
	GAINx8   = 0x18,
	GAINx16  = 0x20,
	GAINx32  = 0x28,
	GAINx64  = 0x30,
	GAINx128 = 0x38
}typedef eGain;

enum
{
	MODE_NORMAL     = 0x00,
	MODE_SELF_CALIB = 0x40,
	MODE_ZERO_SCALE = 0x80,
	MODE_FULL_SCALE = 0xB0
}typedef eMode;

enum
{
	CH_AIN1_1 = 0x00,
	CH_AIN2_1 = 0x01,
	CH_AIN1_2 = 0x02,
	CH_AIN12  = 0x03
}typedef eChannel;

enum
{
	HZ_20  = 0x00,
	HZ_25  = 0x01,
	HZ_50  = 0x04,
	HZ_60  = 0x05,
	HZ_100 = 0x02,
	HZ_200 = 0x03,
	HZ_250 = 0x06,
	HZ_500 = 0x07
}typedef eDataRate;

extern SPI_HandleTypeDef hspi1;

void AD7705_Init(eChannel channel, eMode mode, eGain gain, eDataRate dataRate);
void AD7705_CSSet(uint8_t state);
uint8_t AD7705_GetDRDY(void);
uint16_t AD7705_ReadADC(void);

#endif /* SRC_AD7705_AD7705_H_ */
