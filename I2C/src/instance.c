#include "i2c.h"
#include "structure.h"

#include <assert.h>
#include <avr/interrupt.h>
#include <util/twi.h>

static uint8_t nextTransaction(I2C* i2c, I2CTransaction* transaction, size_t* transfer);

static I2C i2c0 = {.Registers = (I2CMemory*)&TWBR, .PWR = &PRR};

static uint8_t nextTransaction(I2C* i2c, I2CTransaction* transaction, size_t* transfer)
{
	transaction->Size = *transfer = 0;
	if (FifoBuffer_Remove(&i2c->TransBuffer, transaction, sizeof(*transaction))) // Next transaction available
	{
		i2c->Active = true;
		return 1 << TWSTA; // Issue start/restart
	}
	else
	{
		i2c->Active = false;
		return 1 << TWSTO; // Release bus, done
	}
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
	static size_t         transfered = 0;
	static I2CTransaction activeTransaction;

	switch (TW_STATUS)
	{
		case TW_START:
			controlValue |= nextTransaction(i2c, &activeTransaction, &transfered);
			// Fall through
		case TW_REP_START:
			controlValue &= ~(1 << TWSTA); // Clear start
			i2c->Registers->Data = activeTransaction.ControlByte;
			break;

		case TW_MT_SLA_ACK: // Fall through
		case TW_MT_DATA_ACK:
			if (transfered < activeTransaction.Size)
			{
				transfered += FifoBuffer_Remove(&i2c->TXBuffer, &data, sizeof(data));
				i2c->Registers->Data = data;
			}
			else // Done with current transaction no more data to write
			{
				i2c->Status = I2C_STATUS_OK;
				if (activeTransaction.Complete)
					activeTransaction.Complete(i2c->Status == I2C_STATUS_OK, activeTransaction.CompleteRef, transfered);

				controlValue |= nextTransaction(i2c, &activeTransaction, &transfered);
			}
			break;

		case TW_MR_SLA_ACK:
			if (activeTransaction.Size > 1) // Enable ACK when receiving more than 1 byte
				controlValue |= 1 << TWEA;
			break;

		case TW_MR_DATA_ACK: // Fall through
		case TW_MR_DATA_NACK:
			data = i2c->Registers->Data; // Force read
			FifoBuffer_Add(&i2c->RXBuffer, &data, sizeof(data));
			transfered++;
			if (activeTransaction.Size - transfered == 1) // next receive is last one
				controlValue &= ~(1 << TWEA);             // NACK to indicate end
			else if (activeTransaction.Size == transfered)
			{
				i2c->Status = I2C_STATUS_OK;
				if (activeTransaction.Complete)
					activeTransaction.Complete(i2c->Status == I2C_STATUS_OK, activeTransaction.CompleteRef, transfered);
				controlValue |= nextTransaction(i2c, &activeTransaction, &transfered);
			}
			break;

		case TW_MR_SLA_NACK:
			i2c->Status = I2C_STATUS_BUS_ERROR;
			if (activeTransaction.Complete)
				activeTransaction.Complete(i2c->Status == I2C_STATUS_OK, activeTransaction.CompleteRef, transfered);
			controlValue |= nextTransaction(i2c, &activeTransaction, &transfered);
			break;

		case TW_MT_SLA_NACK:  // Fall through
		case TW_MT_DATA_NACK: // Fall through
		case TW_MT_ARB_LOST:  // Fall through
			i2c->Status = I2C_STATUS_BUS_ERROR;
			if (activeTransaction.Complete)
				activeTransaction.Complete(i2c->Status == I2C_STATUS_OK, activeTransaction.CompleteRef, transfered);
			while (transfered++ < activeTransaction.Size) // Remove this transactions write data still in buffer
				FifoBuffer_Remove(&i2c->TXBuffer, &data, sizeof(data));

			controlValue |= nextTransaction(i2c, &activeTransaction, &transfered);
			break;

		default: // Should never enter here, stop I2C should this happen
			assert(false);
			controlValue |= 1 << TWSTO; // release interface
			i2c->Active = false;
			break;
	}
	i2c->Registers->Control = controlValue;
}
