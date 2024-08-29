#ifndef _I2C_H_
#define _I2C_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*I2CRequestComplete)(void*);

typedef struct _I2CTransaction_
{
	union
	{
		uint8_t ControlByte;
		struct
		{
			uint8_t Action  : 1;
			uint8_t Address : 7;
		};
	};
	size_t             Size;
	I2CRequestComplete Handler;
	void*              Data;
} I2CTransaction;

typedef enum _I2CInstance_
{
	I2C0,
} I2CInstance;

typedef struct _I2C_ I2C;

I2C* I2C_GetInstance(I2CInstance instance);

void I2C_Init(I2C* i2c);
void I2C_Deinit(I2C* i2c);

void I2C_AssignTransactionBuffer(I2C* i2c, I2CTransaction* buff, size_t size);
void I2C_AssignWriteBuffer(I2C* i2c, uint8_t* buff, size_t size);
void I2C_AssignReadBuffer(I2C* i2c, uint8_t* buff, size_t size);

void I2C_SetBaud(I2C* i2c, uint32_t baud);

bool   I2C_Request(I2C* i2c, uint8_t addr, size_t size, I2CRequestComplete complete);
size_t I2C_Read(I2C* i2c, void* data, size_t size);
size_t I2C_Write(I2C* i2c, uint8_t addr, void* data, size_t size);

bool I2C_IsBusy(I2C* i2c);

#endif
