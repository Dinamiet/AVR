#include "i2c.h"
#include "structure.h"

#include <avr/interrupt.h>
#include <util/twi.h>

static void newTransaction(I2C* i2c, I2CTransaction* newTrans);

static void newTransaction(I2C* i2c, I2CTransaction* newTrans)
{
	I2CTransaction* tmp = FifoBuffer_Remove(&i2c->TransBuffer);

	newTrans->Size = 0;
	if (tmp)
	{
		newTrans->ControlByte = tmp->ControlByte;
		newTrans->Size        = tmp->Size;
		newTrans->Handler     = tmp->Handler;
		newTrans->Data        = tmp->Data;
	}
}

ISR(TWI_vect)
{
	I2C*                  i2c          = I2C_GetInstance(I2C0);
	uint8_t               controlValue = i2c->Registers->Control;
	static uint8_t*       data;
	static I2CTransaction activeTransaction;

	switch (TW_STATUS)
	{
		case TW_START:
			newTransaction(i2c, &activeTransaction);
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

			data                 = activeTransaction.Data;
			i2c->Registers->Data = activeTransaction.ControlByte;
			break;

		case TW_MT_SLA_ACK: // Fall through
		case TW_MT_DATA_ACK:
			if (activeTransaction.Size > 0)
			{
				--activeTransaction.Size;
				data = FifoBuffer_Remove(&i2c->TXBuffer);
				if (data)
					i2c->Registers->Data = *data;
			}
			else // Done with current transaction no more data to write
			{
				if (activeTransaction.Handler)
					activeTransaction.Handler(activeTransaction.Data);
				newTransaction(i2c, &activeTransaction);
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
		case TW_MR_DATA_NACK:
			*data++ = i2c->Registers->Data;
			--activeTransaction.Size;
			if (activeTransaction.Size == 1)  // next receive is last one
				controlValue &= ~(1 << TWEA); // Disable ACK
			else if (activeTransaction.Size == 0)
			{
				if (activeTransaction.Handler)
					activeTransaction.Handler(activeTransaction.Data);
				newTransaction(i2c, &activeTransaction);
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
		case TW_MT_SLA_NACK:  // Fall through
		case TW_MT_DATA_NACK: // Fall through
		case TW_MT_ARB_LOST:  // Fall through
		default:
			activeTransaction.Size = 0;
			data                   = FifoBuffer_Remove(&i2c->TXBuffer);
			while (data) // Remove all data - device NACKED
				data = FifoBuffer_Remove(&i2c->TXBuffer);

			data = FifoBuffer_Remove(&i2c->TransBuffer);
			while (data) // Remove all pending transactions - device NACKED
				data = FifoBuffer_Remove(&i2c->TransBuffer);

			controlValue |= 1 << TWSTO; // release interface
			i2c->Active = false;
			break;
	}
	i2c->Registers->Control = controlValue;
}
