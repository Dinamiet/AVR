#include "i2c.h"
#include "structure.h"

#include <avr/io.h>
#include <util/twi.h>

size_t I2C_Read(I2C* i2c, uint8_t addr, I2CRequestComplete handler, void* data, size_t size)
{
	I2CTransaction* transaction = FifoBuffer_Add(&i2c->TransBuffer);
	if (!transaction)
		return 0;

	transaction->Action  = TW_READ;
	transaction->Address = addr;
	transaction->Size    = size;
	transaction->Handler = handler;
	transaction->Data    = data;

	if (!i2c->Active)
	{
		i2c->Registers->Control |= 1 << TWSTA; // Issue start if not already active
	}

	return size;
}

size_t I2C_Write(I2C* i2c, uint8_t addr, void* _data, size_t size)
{
	uint8_t*        data        = _data;
	I2CTransaction* transaction = FifoBuffer_Add(&i2c->TransBuffer);
	if (!transaction)
		return 0;

	uint8_t* bufferElement = FifoBuffer_Add(&i2c->TXBuffer);
	size_t   written;

	while (bufferElement)
	{
		*bufferElement = data[written++];
		if (written >= size)
			break;
		bufferElement = FifoBuffer_Add(&i2c->TXBuffer);
	}

	transaction->Action  = TW_WRITE;
	transaction->Address = addr;
	transaction->Size    = written;

	if (!i2c->Active)
	{
		i2c->Registers->Control |= 1 << TWSTA; // Issue start if not already active
	}

	return written;
}

bool I2C_IsBusy(I2C* i2c) { return i2c->Active; }
