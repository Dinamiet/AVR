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

	transaction.Action   = TW_READ;
	transaction.Address  = addr;
	transaction.Size     = size;
	transaction.Complete = complete;
	transaction.CompleteRef = completeRef;

	FifoBuffer_Add(&i2c->TransBuffer, &transaction, sizeof(transaction));

	if (!i2c->Active)
		i2c->Registers->Control |= 1 << TWSTA; // Issue start if not already active

	return true;
}

size_t I2C_Read(I2C* i2c, void* data, const size_t size)
{
	assert(i2c != NULL);
	assert(data != NULL);

	return FifoBuffer_Remove(&i2c->RXBuffer, data, size);
}

size_t I2C_Write(I2C* i2c, const uint8_t addr, const void* data, const size_t size, const I2C_Complete complete, const void* completeRef)
{
	assert(i2c != NULL);
	assert(data != NULL);

	I2CTransaction transaction;

	if (FifoBuffer_Free(&i2c->TransBuffer) < sizeof(transaction))
		return 0;

	transaction.Action   = TW_WRITE;
	transaction.Address  = addr;
	transaction.Complete = complete;
	transaction.CompleteRef = completeRef;
	transaction.Size     = FifoBuffer_Add(&i2c->TXBuffer, data, size);

	FifoBuffer_Add(&i2c->TransBuffer, &transaction, sizeof(transaction));

	if (!i2c->Active)
		i2c->Registers->Control |= 1 << TWSTA; // Issue start if not already active

	return transaction.Size;
}

bool I2C_IsBusy(const I2C* i2c)
{
	assert(i2c != NULL);

	return i2c->Active;
}
