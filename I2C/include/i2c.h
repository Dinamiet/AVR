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

/**
 * Transaction completion callback.
 * This callback is called from the interrupt routine.
 * \param success Was the transaction complete
 * \param ref Reference provided with completion callback
 * \param size The number of bytes the transaction transfered
 */
typedef void (*I2C_Complete)(const bool success, const void* ref, const size_t size);

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
	I2C_Complete       Complete; /** Called when transaction finishes */
	const void*        CompleteRef; /** Transaction complete reference */
} I2CTransaction;

/**
 * I2C device information on the bus
 */
typedef struct _I2CDevice_
{
	void*               Device;     /** Device properties, this will be used as callback reference within transaction notifications */
	I2C*                Bus;        /** I2C bus to which the device is connected */
	uint8_t             Address;    /** Device address */
	I2CDeviceAddressing Addressing; /** Addressing type the devices uses - 8bit/16bit registers */
} I2CDevice;

/**
 * I2C bus status
 */
typedef enum _I2CStatus_
{
	I2C_STATUS_OK,        /** Ok */
	I2C_STATUS_BUS_ERROR, /** Bus error */
} I2CStatus;

/**
 * Different I2C device addressing modes
 */
typedef enum _I2CDeviceAddressing_
{
	I2C_ADDRESSING_8BIT  = 1, /** 8bit device registers */
	I2C_ADDRESSING_16BIT = 2  /** 16bit device registers */
} I2CDeviceAddressing;

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
I2C* I2C_GetInstance(const I2CInstance instance);

/**
 * Binds/Links a device to a bus
 * \param deviceRef Device structure reference, may be NULL, passed as reference to callbacks
 * \param bus I2C instance to which the device is connected
 * \param deviceAddress I2C device address to initiate communications
 * \param addressing Device register addressing used (8bit/16bit)
 * \return Device for use with related I2C functions
 */
I2CDevice I2C_BindDevice(const void* deviceRef, I2C* bus, uint8_t deviceAddress, I2CDeviceAddressing addressing);

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
 * \param size Size (Bytes) of the transaction buffer
 */
void I2C_AssignTransactionBuffer(I2C* i2c, I2CTransaction* buff, const size_t size);

/**
 * Assigns a Write buffer to the I2C instance.
 * All transaction write data is stored in this buffer
 * \param i2c Instance to assign buffer too
 * \param buff Write buffer to use
 * \param size Size (Bytes) of the available buffer
 */
void I2C_AssignWriteBuffer(I2C* i2c, void* buff, const size_t size);

/**
 * Assigns a Read buffer to the I2C instance.
 * All transaction read data is stored in this buffer
 * \param i2c Instance to assign buffer too
 * \param buff Read buffer to use
 * \param size Size (Bytes) of the available buffer
 */
void I2C_AssignReadBuffer(I2C* i2c, void* buff, const size_t size);

/** Configures the I2C instance bus speed/clock rate
 * \param i2c Instance clock rate to adjust
 * \param baud Bus clock rate (Hz)
 */
void I2C_SetBaud(I2C* i2c, const uint32_t baud);

/**
 * Issue a Request for data (Read) on the I2C bus
 * \param i2c Bus to request data on
 * \param addr Device from which to request data
 * \param size Number of bytes to request from the device
 * \param complete Transaction done callback
 * \param completeRef Callback reference
 * \return True if transaction could be queued on the bus, false otherwise
 */
bool I2C_Request(I2C* i2c, const uint8_t addr, const size_t size, const I2C_Complete complete, const void* completeRef);

/**
 * Read requested data. All data will only be available after the transaction is completed
 * \param i2c Bus to read buffered data from
 * \param data Destination where read data will be stored
 * \param size Number of bytes to read from buffered data (recommended to match requested transaction size)
 * \return Number of bytes read from buffered data
 */
size_t I2C_Read(I2C* i2c, void* data, const size_t size);

/**
 * Write data to a device on the bus
 * \param device Device to write to
 * \param data The data to write
 * \param size Number of bytes to write
 * \param completeCallback Called when transaction completes
 * \return Number of bytes added to buffer to write
 */
size_t I2C_Write(const I2CDevice* device, const void* data, const size_t size, const I2C_Complete completeCallback);

/**
 * Check if I2C instance is busy
 * \param i2c Instance
 * \return True if the I2C instance is busy, False if no transactions are pending
 */
bool I2C_IsBusy(const I2C* i2c);

/**
 * Retrieves the status of the I2C bus, updated after each transaction
 * \param i2c The bus
 * \return Current bus status
 */
I2CStatus I2C_GetStatus(const I2C* i2c);

#endif
