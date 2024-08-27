#include "i2c.h"
#include "structure.h"

#include <avr/io.h>

static I2C i2c0 = {.Registers = (I2CMemory*)&TWBR, .PWR = &PRR};

I2C* I2C_GetInstance(I2CInstance instance)
{
	switch (instance)
	{
		case I2C0:
			return &i2c0;

		default:
			return NULL;
	}
}
