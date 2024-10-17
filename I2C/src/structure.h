#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#include "fifobuffer.h"

#include <stdint.h>

typedef struct _I2CMemory_
{
	volatile uint8_t BitRate;
	volatile uint8_t Status;
	volatile uint8_t SlaveAddress;
	volatile uint8_t Data;
	volatile uint8_t Control;
	volatile uint8_t SlaveMask;
} I2CMemory;

typedef struct _I2C_
{
	I2CMemory* const        Registers;
	volatile uint8_t* const PWR;
	volatile bool           Active;
	FifoBuffer              TransBuffer;
	FifoBuffer              RXBuffer;
	FifoBuffer              TXBuffer;
	I2CStatus               Status;
} I2C;

typedef struct _I2CTransaction_
{
	const I2CDevice* Device;
	I2C_Complete     Complete;
	size_t           WriteSize;
	size_t           RequestSize;
} I2CTransaction;

#endif
