#include "i2c.h"
#include "structure.h"

#include <assert.h>
#include <avr/interrupt.h>
#include <util/twi.h>

bool I2C_Request(I2C* i2c, const uint8_t addr, const size_t size, const I2C_Complete complete, const void* completeRef)
{
	assert(i2c != NULL);
	assert(size > 0);

	I2CTransaction transaction;
	if (FifoBuffer_Free(&i2c->TransBuffer) < sizeof(transaction))
		return false;

	transaction.Action      = TW_READ;
	transaction.Address     = addr;
	transaction.Size        = size;
	transaction.Complete    = complete;
	transaction.CompleteRef = completeRef;

	FifoBuffer_Add(&i2c->TransBuffer, &transaction, sizeof(transaction));

	if (!i2c->Active)
	{
		i2c->Active = true;
		i2c->Registers->Control |= 1 << TWSTA; // Issue start if not already active
	}

	return true;
}

size_t I2C_Read(I2C* i2c, void* data, const size_t size)
{
	assert(i2c != NULL);
	assert(data != NULL);

	return FifoBuffer_Remove(&i2c->RXBuffer, data, size);
}

size_t I2C_Write(const I2CDevice* device, const void* data, const size_t size, const I2C_Complete completeCallback)
{
	I2CTransaction transaction;

	if (FifoBuffer_Free(&device->Bus->TransBuffer) < sizeof(transaction))
		return 0;
	if (FifoBuffer_Free(&device->Bus->TXBuffer) < sizeof(size))
		return 0;

	/** TODO: Update transaction data population */
	transaction.Action      = TW_WRITE;
	transaction.Complete    = completeCallback;
	transaction.CompleteRef = device;
	transaction.Size        = FifoBuffer_Add(&device->Bus->TXBuffer, data, size);

	FifoBuffer_Add(&device->Bus->TransBuffer, &transaction, sizeof(transaction));

	if (!device->Bus->Active)
	{
		device->Bus->Active = true;
		device->Bus->Registers->Control |= 1 << TWSTA; // Issue start if not already active
	}

	return transaction.Size;
}

bool I2C_IsBusy(const I2C* i2c)
{
	assert(i2c != NULL);

	return i2c->Active;
}
