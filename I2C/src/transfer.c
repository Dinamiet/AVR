#include "i2c.h"
#include "structure.h"

#include <assert.h>
#include <avr/io.h>
#include <util/twi.h>

bool I2C_Request(I2C* i2c, uint8_t addr, size_t size, I2CRequestComplete complete)
{
	assert(i2c != NULL);
	assert(size > 0);

	// Disable Interrupt?
	I2CTransaction transaction;
	if (FifoBuffer_Free(&i2c->TransBuffer) < sizeof(transaction))
		return false;

	transaction.Action   = TW_READ;
	transaction.Address  = addr;
	transaction.Size     = size;
	transaction.Complete = complete;

	FifoBuffer_Add(&i2c->TransBuffer, &transaction, sizeof(transaction));

	// Enable Interrupt?

	if (!i2c->Active)
		i2c->Registers->Control |= 1 << TWSTA; // Issue start if not already active

	return true;
}

size_t I2C_Read(I2C* i2c, void* data, size_t size)
{
	assert(i2c != NULL);
	assert(data != NULL);

	// Disable Interrupt?
	// Enable Interrupt?

	return FifoBuffer_Remove(&i2c->RXBuffer, data, size);
}

size_t I2C_Write(I2C* i2c, uint8_t addr, void* data, size_t size)
{
	assert(i2c != NULL);
	assert(data != NULL);

	// Disable Interrupt?
	I2CTransaction transaction;

	if (FifoBuffer_Free(&i2c->TransBuffer) < sizeof(transaction))
		return 0;

	transaction.Action  = TW_WRITE;
	transaction.Address = addr;
	transaction.Size    = FifoBuffer_Add(&i2c->TXBuffer, data, size);

	// Enable Interrupt?

	if (!i2c->Active)
		i2c->Registers->Control |= 1 << TWSTA; // Issue start if not already active

	return transaction.Size;
}

bool I2C_IsBusy(I2C* i2c)
{
	assert(i2c != NULL);

	return i2c->Active;
}
