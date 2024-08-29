#include "i2c.h"
#include "structure.h"

#include <avr/io.h>

void I2C_Init(I2C* i2c)
{
	uint8_t registerSetup = 1 << TWEN; // Enable I2C HW
	registerSetup |= 1 << TWIE;        // Enable I2C interrupt
	i2c->Registers->Control |= registerSetup;

	*i2c->PWR &= ~(1 << PRTWI); // Power on HW
}

void I2C_Deinit(I2C* i2c)
{
	uint8_t registerSetup = 1 << TWEN; // Disable I2C HW
	registerSetup |= 1 << TWIE;        // Disable I2C interrupt
	i2c->Registers->Control &= ~registerSetup;

	*i2c->PWR |= (1 << PRTWI); // Power off HW
}

void I2C_AssignTransactionBuffer(I2C* i2c, I2CTransaction* buff, size_t size) { FifoBuffer_Init(&i2c->TransBuffer, buff, sizeof(*buff), size); }

void I2C_AssignWriteBuffer(I2C* i2c, uint8_t* buff, size_t size) { FifoBuffer_Init(&i2c->TXBuffer, buff, sizeof(*buff), size); }

void I2C_AssignReadBuffer(I2C* i2c, uint8_t* buff, size_t size) { FifoBuffer_Init(&i2c->RXBuffer, buff, sizeof(*buff), size); }

void I2C_SetBaud(I2C* i2c, uint32_t baud)
{
	i2c->Registers->BitRate = ((F_CPU / baud) - 16) / 2;
	i2c->Registers->Status  = 0;
}
