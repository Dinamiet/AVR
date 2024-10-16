#include "i2c.h"
#include "structure.h"

#include <assert.h>
#include <avr/interrupt.h>
#include <util/twi.h>

static I2C i2c0 = {.Registers = (I2CMemory*)&TWBR, .PWR = &PRR};

static uint8_t finalizeTransaction(I2CTransaction* transaction, I2CStatus status);

static uint8_t finalizeTransaction(I2CTransaction* transaction, I2CStatus status)
{
	uint8_t controlValue             = (1 << TWSTO);
	transaction->Device->Bus->Status = status;
	if (transaction->Complete)
		transaction->Complete(transaction->Device, status == I2C_STATUS_OK);
	if (FifoBuffer_Empty(&transaction->Device->Bus->TransBuffer))
		transaction->Device->Bus->Active = false;
	else
		controlValue |= (1 << TWSTA);

	return controlValue;
}

I2C* I2C_GetInstance(const I2CInstance instance)
{
	switch (instance)
	{
		case I2C0:
			return &i2c0;

		default:
			return NULL;
	}
}

I2CDevice I2C_BindDevice(const void* const deviceRef, I2C* const bus, const uint8_t deviceAddress, const I2CDeviceAddressing addressing)
{
	I2CDevice device = {.Device = deviceRef, .Bus = bus, .Address = deviceAddress, .Addressing = addressing};
	return device;
}

I2CStatus I2C_GetStatus(const I2C* i2c) { return i2c->Status; }

ISR(TWI_vect)
{
	I2C*                  i2c          = I2C_GetInstance(I2C0);
	uint8_t               controlValue = i2c->Registers->Control;
	uint8_t               data;
	static I2CTransaction activeTransaction;

	switch (TW_STATUS)
	{
		case TW_START:
			FifoBuffer_Remove(&i2c->TransBuffer, &activeTransaction, sizeof(activeTransaction));
			// Fall through
		case TW_REP_START:
			controlValue &= ~(1 << TWSTA); // Clear start
			data = activeTransaction.Device->Address << 1;
			if (activeTransaction.RequestSize && !activeTransaction.WriteSize)
				data |= TW_READ;
			i2c->Registers->Data = data;
			break;

		case TW_MT_SLA_ACK: // Fall through
		case TW_MT_DATA_ACK:
			if (activeTransaction.WriteSize)
			{
				activeTransaction.WriteSize -= FifoBuffer_Remove(&i2c->TXBuffer, &data, sizeof(data));
				i2c->Registers->Data = data;
			}
			else if (activeTransaction.RequestSize) // Repeated start for request
			{
				controlValue |= (1 << TWSTA);
			}
			else // Done with transaction
			{
				controlValue |= finalizeTransaction(&activeTransaction, I2C_STATUS_OK);
			}
			break;

		case TW_MR_SLA_ACK:
			if (activeTransaction.RequestSize > 1) // Enable ACK when receiving more than 1 byte
				controlValue |= 1 << TWEA;
			break;

		case TW_MR_DATA_ACK: // Fall through
		case TW_MR_DATA_NACK:
			data = i2c->Registers->Data; // Force read
			FifoBuffer_Add(&i2c->RXBuffer, &data, sizeof(data));
			--activeTransaction.RequestSize;
			if (activeTransaction.RequestSize == 1)       // next receive is last one
				controlValue &= ~(1 << TWEA);             // NACK to indicate end
			else if (!activeTransaction.RequestSize)
			{
				controlValue |= finalizeTransaction(&activeTransaction, I2C_STATUS_OK);
			}
			break;

		case TW_MR_SLA_NACK:
			controlValue |= finalizeTransaction(&activeTransaction, I2C_STATUS_NACK);
			break;

		case TW_MT_SLA_NACK:  // Fall through
		case TW_MT_DATA_NACK:
			FifoBuffer_Remove(&activeTransaction.Device->Bus->TXBuffer, NULL, activeTransaction.WriteSize);
			controlValue |= finalizeTransaction(&activeTransaction, I2C_STATUS_NACK);
			break;

		case TW_MT_ARB_LOST:
			FifoBuffer_Remove(&activeTransaction.Device->Bus->TXBuffer, NULL, activeTransaction.WriteSize);
			controlValue |= finalizeTransaction(&activeTransaction, I2C_STATUS_ARB_LOST);
			controlValue &= ~(1 << TWSTO); // Bus released when arbitration is lost, no need to stop
			break;

		default: // Should never enter here, stop I2C should this happen
			assert(false);
			i2c->Active = false;
			break;
	}
	i2c->Registers->Control = controlValue;
}
