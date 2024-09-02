#ifndef _I2C_H_
#define _I2C_H_

/**
 * \file
 * I2C
 *
 * Non blocking I2C Master mode
 *
 * TODO: Implement I2C Slave mode
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** Transaction completion status */
typedef enum _I2CCompleteStatus_
{
	I2C_COMPLETE_OK,    /** Completed without any errors/issues */
	I2C_COMPLETE_ERROR, /** Completed/Interrupted with errors/issues (NACK) */
} I2CCompleteStatus;

/**
 * Transaction completion callback.
 * This callback is called from the interrupt routine.
 * \param status The status of the transaction
 * \param addr The device the transaction was intended for
 * \param size The number of bytes the transaction transfered
 */
typedef void (*I2CRequestComplete)(I2CCompleteStatus status, uint8_t addr, size_t size);

/**
 * Transaction storage
 */
typedef struct _I2CTransaction_
{
	union
	{
		uint8_t ControlByte; /** Combination of device address and action (R/W) */
		struct
		{
			uint8_t Action  : 1; /** Indicates if the transaction is R/W */
			uint8_t Address : 7; /** Device address of the transaction */
		};
	};
	size_t             Size;     /** Number of bytes this transaction needs to transfer */
	I2CRequestComplete Complete; /** Called when transaction finishes */
} I2CTransaction;

/**
 * Available I2C instances
 */
typedef enum _I2CInstance_
{
	I2C0, /** I2C */
} I2CInstance;

/**
 * I2C
 */
typedef struct _I2C_ I2C;

/** Retrieves the provided I2C instance
 * \param instance The I2C instance to return
 * \return Reference to I2C instance
 */
I2C* I2C_GetInstance(I2CInstance instance);

/**
 * Initialize and power-on I2C hardware
 * \param i2c Hardware to initialize and power-on
 */
void I2C_Init(I2C* i2c);

/**
 * Deinitialize and power-off I2C hardware
 * \param i2c Hardware to deinitialize and power-off
 */
void I2C_Deinit(I2C* i2c);

/**
 * Assigns a Transaction buffer to the I2C instance.
 * All Requests and Writes are queued in this buffer, executing in order.
 * \param i2c Instance to assign buffer too
 * \param buff Transaction buffer to use
 * \param num Number of transactions the buffer can hold
 */
void I2C_AssignTransactionBuffer(I2C* i2c, I2CTransaction* buff, size_t num);

/**
 * Assigns a Write buffer to the I2C instance.
 * All transaction write data is stored in this buffer
 * \param i2c Instance to assign buffer too
 * \param buff Write buffer to use
 * \param size Size (Bytes) of the available buffer
 */
void I2C_AssignWriteBuffer(I2C* i2c, uint8_t* buff, size_t size);

/**
 * Assigns a Read buffer to the I2C instance.
 * All transaction read data is stored in this buffer
 * \param i2c Instance to assign buffer too
 * \param buff Read buffer to use
 * \param size Size (Bytes) of the available buffer
 */
void I2C_AssignReadBuffer(I2C* i2c, uint8_t* buff, size_t size);

/** Configures the I2C instance bus speed/clock rate
 * \param i2c Instance clock rate to adjust
 * \param baud Bus clock rate (Hz)
 */
void I2C_SetBaud(I2C* i2c, uint32_t baud);

/**
 * Issue a Request for data (Read) on the I2C bus
 * \param i2c Bus to request data on
 * \param addr Device from which to request data
 * \param size Number of bytes to request from the device
 * \param complete Transaction done callback
 * \return True if transaction could be queued on the bus, false otherwise
 */
bool I2C_Request(I2C* i2c, uint8_t addr, size_t size, I2CRequestComplete complete);

/**
 * Read requested data. All data will only be available after the transaction is completed
 * \param i2c Bus to read buffered data from
 * \param data Destination where read data will be stored
 * \param size Number of bytes to read from buffered data (recommended to match requested transaction size)
 * \return Number of bytes read from buffered data
 */
size_t I2C_Read(I2C* i2c, void* data, size_t size);

/**
 * Write data to a device on the bus
 * \param i2c Bus to write on
 * \param addr Device to write to
 * \param data The data to write
 * \param size Number of bytes to write to the device
 * \return Number of bytes added to buffer to write
 */
size_t I2C_Write(I2C* i2c, uint8_t addr, void* data, size_t size);

/**
 * Check if I2C instance is busy
 * \param i2c Instance
 * \return True if the I2C instance is busy, False if no transactions are pending
 */
bool I2C_IsBusy(I2C* i2c);

#endif
