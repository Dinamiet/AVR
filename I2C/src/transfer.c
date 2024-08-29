#include "i2c.h"
#include "structure.h"

#include <avr/io.h>
#include <util/twi.h>

bool I2C_Request(I2C* i2c, uint8_t addr, size_t size, I2CRequestComplete complete)
{
	// Disable Interrupt?
	I2CTransaction* transaction = FifoBuffer_Add(&i2c->TransBuffer);
	if (!transaction)
		return false;

	transaction->Action   = TW_READ;
	transaction->Address  = addr;
	transaction->Size     = size;
	transaction->Complete = complete;

	// Enable Interrupt?

	if (!i2c->Active)
	{
		i2c->Registers->Control |= 1 << TWSTA; // Issue start if not already active
	}

	return true;
}

size_t I2C_Read(I2C* i2c, void* _data, size_t size)
{
	uint8_t* data = _data;
	size_t   read;

	// Disable Interrupt?

	uint8_t* bufferElement = FifoBuffer_Remove(&i2c->RXBuffer);
	while (bufferElement)
	{
		data[read++] = *bufferElement;
		if (read >= size)
			break;

		bufferElement = FifoBuffer_Remove(&i2c->RXBuffer);
	}

	// Enable Interrupt?

	return read;
}

size_t I2C_Write(I2C* i2c, uint8_t addr, void* _data, size_t size)
{
	uint8_t*        data        = _data;
	I2CTransaction* transaction = FifoBuffer_Add(&i2c->TransBuffer);
	if (!transaction)
		return 0;

	// Disable Interrupt?

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

	// Enable Interrupt?

	if (!i2c->Active)
	{
		i2c->Registers->Control |= 1 << TWSTA; // Issue start if not already active
	}

	return written;
}

bool I2C_IsBusy(I2C* i2c) { return i2c->Active; }
