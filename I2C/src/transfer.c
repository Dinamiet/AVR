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

size_t I2C_Read(const I2CDevice* device, void* data, const size_t size)
{
	assert(device != NULL);

	cli(); // Have to disable global interrupts - cant write I2C Control register to disable only I2C interrupt without messing with the I2C bus
	size_t retVal = FifoBuffer_Remove(&device->Bus->RXBuffer, data, size);
	sei(); // Re-enable global interrupts
	return retVal;
}

bool I2C_Write(const I2CDevice* device, const void* data, const size_t size, const I2C_CompleteHandler complete_handler)
{
	assert(device != NULL);

	cli(); // Have to disable global interrupts - cant write I2C Control register to disable only I2C interrupt without messing with the I2C bus

	I2CTransaction transaction;
	if (FifoBuffer_Free(&device->Bus->TransBuffer) < sizeof(transaction))
	{
		sei(); // Re-enable global interrupts
		return false;
	}
	if (FifoBuffer_Free(&device->Bus->TXBuffer) < size)
	{
		sei(); // Re-enable global interrupts
		return false;
	}

	transaction.Device      = device;
	transaction.Complete    = complete_handler;
	transaction.WriteSize   = FifoBuffer_Add(&device->Bus->TXBuffer, data, size);
	transaction.RequestSize = 0;

	FifoBuffer_Add(&device->Bus->TransBuffer, &transaction, sizeof(transaction));

	sei(); // Re-enable global interrupts

	if (!device->Bus->Active)
	{
		device->Bus->Active = true;
		device->Bus->Registers->Control |= 1 << TWSTA; // Issue start if not already active
	}

	return true;
}

bool I2C_WriteMem(const I2CDevice* device, const uint16_t address, const void* data, const size_t size, const I2C_CompleteHandler complete_handler)
{
	assert(device != NULL);

	cli(); // Have to disable global interrupts - cant write I2C Control register to disable only I2C interrupt without messing with the I2C bus

	I2CTransaction transaction;
	if (FifoBuffer_Free(&device->Bus->TransBuffer) < sizeof(transaction))
	{
		sei(); // Re-enable global interrupts
		return false;
	}
	if (FifoBuffer_Free(&device->Bus->TXBuffer) < size + device->Addressing)
	{
		sei(); // Re-enable global interrupts
		return false;
	}

	uint16_t memoryRegister = (device->Addressing == I2C_ADDRESSING_8BIT) ? address : BIG_ENDIAN_16(address);

	transaction.Device    = device;
	transaction.Complete  = complete_handler;
	transaction.WriteSize = FifoBuffer_Add(&device->Bus->TXBuffer, &memoryRegister, device->Addressing);
	transaction.WriteSize += FifoBuffer_Add(&device->Bus->TXBuffer, data, size);
	transaction.RequestSize = 0;

	FifoBuffer_Add(&device->Bus->TransBuffer, &transaction, sizeof(transaction));

	sei(); // Re-enable global interrupts

	if (!device->Bus->Active)
	{
		device->Bus->Active = true;
		device->Bus->Registers->Control |= 1 << TWSTA; // Issue start if not already active
	}

	return true;
}

bool I2C_Request(const I2CDevice* device, const size_t size, const I2C_CompleteHandler complete_handler)
{
	assert(device != NULL);
	assert(size > 0);

	cli(); // Have to disable global interrupts - cant write I2C Control register to disable only I2C interrupt without messing with the I2C bus

	I2CTransaction transaction;
	if (FifoBuffer_Free(&device->Bus->TransBuffer) < sizeof(transaction))
	{
		sei(); // Re-enable global interrupts
		return false;
	}

	transaction.Device      = device;
	transaction.Complete    = complete_handler;
	transaction.RequestSize = size;
	transaction.WriteSize   = 0;

	FifoBuffer_Add(&device->Bus->TransBuffer, &transaction, sizeof(transaction));

	sei(); // Re-enable global interrupts

	if (!device->Bus->Active)
	{
		device->Bus->Active = true;
		device->Bus->Registers->Control |= 1 << TWSTA; // Issue start if not already active
	}

	return true;
}

bool I2C_RequestMem(const I2CDevice* device, const uint16_t address, const size_t size, const I2C_CompleteHandler complete_handler)
{
	assert(device != NULL);
	assert(size > 0);

	cli(); // Have to disable global interrupts - cant write I2C Control register to disable only I2C interrupt without messing with the I2C bus

	I2CTransaction transaction;
	if (FifoBuffer_Free(&device->Bus->TransBuffer) < sizeof(transaction))
	{
		sei(); // Re-enable global interrupts
		return false;
	}
	if (FifoBuffer_Free(&device->Bus->TXBuffer) < device->Addressing)
	{
		sei(); // Re-enable global interrupts
		return false;
	}

	uint16_t memoryRegister = (device->Addressing == I2C_ADDRESSING_8BIT) ? address : BIG_ENDIAN_16(address);

	transaction.Device      = device;
	transaction.Complete    = complete_handler;
	transaction.RequestSize = size;
	transaction.WriteSize   = FifoBuffer_Add(&device->Bus->TXBuffer, &memoryRegister, device->Addressing);

	FifoBuffer_Add(&device->Bus->TransBuffer, &transaction, sizeof(transaction));

	sei(); // Re-enable global interrupts

	if (!device->Bus->Active)
	{
		device->Bus->Active = true;
		device->Bus->Registers->Control |= 1 << TWSTA; // Issue start if not already active
	}

	return true;
}

bool I2C_IsBusy(const I2CDevice* device)
{
	assert(device != NULL);

	return device->Bus->Active;
}
