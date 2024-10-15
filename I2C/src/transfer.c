#include "i2c.h"
#include "structure.h"

#include <assert.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define BIG_ENDIAN_16(x)    __builtin_bswap16(x)
#define LITTLE_ENDIAN_16(x) (x)
#define BIG_ENDIAN_32(x)    __builtin_bswap32(x)
#define LITTLE_ENDIAN_32(x) (x)
#else
#define BIG_ENDIAN_16(x)    (x)
#define LITTLE_ENDIAN_16(x) __builtin_bswap16(x)
#define BIG_ENDIAN_32(x)    (x)
#define LITTLE_ENDIAN_32(x) __builtin_bswap32(x)
#endif

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
	if (FifoBuffer_Free(&device->Bus->TXBuffer) < size)
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

size_t I2C_WriteMem(const I2CDevice* device, const uint16_t address, void* data, const size_t size, const I2C_Complete completeCallback)
{
	I2CTransaction transaction;
	if (FifoBuffer_Free(&device->Bus->TransBuffer) < sizeof(transaction))
		return 0;
	if (FifoBuffer_Free(&device->Bus->TXBuffer) < size + device->Addressing)
		return 0;

	uint16_t memoryRegister = (device->Addressing == I2C_ADDRESSING_8BIT) ? address : BIG_ENDIAN_16(address);

	/** TODO: Update transaction data population */
	transaction.Action      = TW_WRITE;
	transaction.Complete    = completeCallback;
	transaction.CompleteRef = device;
	transaction.Size        = FifoBuffer_Add(&device->Bus->TXBuffer, &memoryRegister, device->Addressing);
	transaction.Size += FifoBuffer_Add(&device->Bus->TXBuffer, data, size);

	FifoBuffer_Add(&device->Bus->TransBuffer, &transaction, sizeof(transaction));
	if (!device->Bus->Active)
	{
		device->Bus->Active = true;
		device->Bus->Registers->Control |= 1 << TWSTA; // Issue start if not already active
	}

	return transaction.Size;
}

bool I2C_Request(const I2CDevice* device, const size_t size, const I2C_Complete completeCallback)
{
	assert(size > 0);

	I2CTransaction transaction;
	if (FifoBuffer_Free(&device->Bus->TransBuffer) < sizeof(transaction))
		return false;

	/** TODO: Update transaction data population */
	transaction.Action      = TW_READ;
	transaction.Size        = size;
	transaction.Complete    = completeCallback;
	transaction.CompleteRef = device;

	FifoBuffer_Add(&device->Bus->TransBuffer, &transaction, sizeof(transaction));

	if (!device->Bus->Active)
	{
		device->Bus->Active = true;
		device->Bus->Registers->Control |= 1 << TWSTA; // Issue start if not already active
	}

	return true;
}

bool I2C_IsBusy(const I2C* i2c)
{
	assert(i2c != NULL);

	return i2c->Active;
}
