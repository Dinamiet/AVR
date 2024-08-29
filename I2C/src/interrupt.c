#include "i2c.h"
#include "structure.h"

#include <avr/interrupt.h>
#include <util/twi.h>

static size_t newTransaction(I2C* i2c, I2CTransaction* newTrans);

static size_t newTransaction(I2C* i2c, I2CTransaction* newTrans)
{
	I2CTransaction* tmp = FifoBuffer_Remove(&i2c->TransBuffer);

	newTrans->Size = 0;
	if (tmp)
	{
		*newTrans = *tmp;
	}

	return 0;
}

ISR(TWI_vect)
{
	I2C*                  i2c          = I2C_GetInstance(I2C0);
	uint8_t               controlValue = i2c->Registers->Control;
	uint8_t*              bufferElement;
	uint8_t               data;
	static size_t         transfered = 0;
	static I2CTransaction activeTransaction;

	switch (TW_STATUS)
	{
		case TW_START:
			transfered = newTransaction(i2c, &activeTransaction);
			// Fall through
		case TW_REP_START:
			i2c->Active = true;
			controlValue &= ~(1 << TWSTA); // Clear start

			if (!activeTransaction.Size)
			{
				controlValue |= 1 << TWSTO; // Release bus - no data waiting
				i2c->Active = false;
				break;
			}
			i2c->Registers->Data = activeTransaction.ControlByte;
			break;

		case TW_MT_SLA_ACK: // Fall through
		case TW_MT_DATA_ACK:
			if (transfered++ < activeTransaction.Size)
			{
				bufferElement = FifoBuffer_Remove(&i2c->TXBuffer);
				if (bufferElement)
					i2c->Registers->Data = *bufferElement;
			}
			else // Done with current transaction no more data to write
			{
				if (activeTransaction.Complete)
					activeTransaction.Complete(activeTransaction.Address, transfered);
				transfered = newTransaction(i2c, &activeTransaction);
				if (activeTransaction.Size)     // Next transaction available
					controlValue |= 1 << TWSTA; // Send repeated start
				else                            // No next transaction available
				{
					controlValue |= 1 << TWSTO; // Send stop
					i2c->Active = false;
				}
			}
			break;

		case TW_MR_SLA_ACK:
			if (activeTransaction.Size > 1) // Enable ACK when receiving more than 1 byte
				controlValue |= 1 << TWEA;
			break;

		case TW_MR_DATA_ACK: // Fall through
			data          = i2c->Registers->Data; // Force read
			bufferElement = FifoBuffer_Add(&i2c->RXBuffer);
			if (bufferElement)
				*bufferElement = data;
			transfered++;
			if (activeTransaction.Size - transfered == 1) // next receive is last one
				controlValue &= ~(1 << TWEA); // Disable ACK
			else if (activeTransaction.Size == transfered)
			{
				if (activeTransaction.Complete)
					activeTransaction.Complete(activeTransaction.Address, transfered);
				transfered = newTransaction(i2c, &activeTransaction);
				if (activeTransaction.Size)     // Next transaction available
					controlValue |= 1 << TWSTA; // Send repeated start
				else                            // No next transaction available
				{
					controlValue |= 1 << TWSTO; // Send stop
					i2c->Active = false;
				}
			}
			break;

		case TW_MR_SLA_NACK:  // Fall through
		case TW_MR_DATA_NACK:
			if (activeTransaction.Complete)
				activeTransaction.Complete(activeTransaction.Address, transfered);
			transfered = newTransaction(i2c, &activeTransaction);
			if (activeTransaction.Size)     // Next transaction available
				controlValue |= 1 << TWSTA; // Send repeated start
			else                            // No next transaction available
			{
				controlValue |= 1 << TWSTO; // Send stop
				i2c->Active = false;
			}
			break;

		case TW_MT_SLA_NACK:  // Fall through
		case TW_MT_DATA_NACK: // Fall through
		case TW_MT_ARB_LOST:  // Fall through
			if (activeTransaction.Complete)
				activeTransaction.Complete(activeTransaction.Address, transfered);
			while (transfered++ < activeTransaction.Size) // Remove this transactions write data still in buffer
				bufferElement = FifoBuffer_Remove(&i2c->TXBuffer);

			transfered = newTransaction(i2c, &activeTransaction);
			if (activeTransaction.Size)     // Next transaction available
				controlValue |= 1 << TWSTA; // Send repeated start
			else                            // No next transaction available
			{
				controlValue |= 1 << TWSTO; // Send stop
				i2c->Active = false;
			}
			break;

		default:
			/** TODO: Assert */
			controlValue |= 1 << TWSTO; // release interface
			i2c->Active = false;
			break;
	}
	i2c->Registers->Control = controlValue;
}
