#ifndef _I2C_H_
#define _I2C_H_

void I2C_GetInstance();

void I2C_Init();
void I2C_Deinit();

void I2C_AssignTransactionBuffer();
void I2C_AssignWriteBuffer();

void I2C_SetBaud();

void I2C_Read();  // addr, data, size, stop
void I2C_Write(); // addr, callback, size, stop

void I2C_IsBusy();

#endif
