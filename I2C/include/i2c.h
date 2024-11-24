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
 * \param device I2C device on which transaction was done
 * \param success Was the transaction successfull
 */
typedef void (*I2C_CompleteHandler)(void* device, const bool success);

/**
 * I2C bus status
 */
typedef enum _I2CStatus_
{
	I2C_STATUS_OK,       /** Ok */
	I2C_STATUS_NACK,     /** Device did not acknowledge */
	I2C_STATUS_ARB_LOST, /** Bus arbitration lost */
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

/**
 * I2C device information on the bus
 */
typedef struct _I2CDevice_
{
	I2C*                Bus;        /** I2C bus to which the device is connected */
	void*               DeviceRef;  /** Device properties, this will be used as callback reference within transaction notifications */
	uint8_t             Address;    /** Device address */
	I2CDeviceAddressing Addressing; /** Addressing type the devices uses - 8bit/16bit registers */
} I2CDevice;

/** Retrieves the provided I2C instance
 * \param instance The I2C instance to return
 * \return Reference to I2C instance
 */
I2C* I2C_GetInstance(const I2CInstance instance);

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
 * Assigns a Read buffer to the I2C instance.
 * All transaction read data is stored in this buffer
 * \param i2c Instance to assign buffer too
 * \param buff Read buffer to use
 * \param size Size (Bytes) of the available buffer
 */
void I2C_AssignReadBuffer(I2C* i2c, void* buff, const size_t size);

/**
 * Assigns a Write buffer to the I2C instance.
 * All transaction write data is stored in this buffer
 * \param i2c Instance to assign buffer too
 * \param buff Write buffer to use
 * \param size Size (Bytes) of the available buffer
 */
void I2C_AssignWriteBuffer(I2C* i2c, void* buff, const size_t size);

/**
 * Assigns a Transaction buffer to the I2C instance.
 * All Requests and Writes are queued in this buffer, executing in order.
 * \param i2c Instance to assign buffer too
 * \param buff Transaction buffer to use
 * \param size Size (Bytes) of the transaction buffer
 */
void I2C_AssignTransactionBuffer(I2C* i2c, void* buff, const size_t size);

/** Configures the I2C instance bus speed/clock rate
 * \param i2c Instance clock rate to adjust
 * \param baud Bus clock rate (Hz)
 */
void I2C_SetBaud(I2C* i2c, const uint32_t baud);

/**
 * Binds/Links a device to a bus
 * \param deviceRef Device structure reference, may be NULL, passed as reference to callbacks
 * \param bus I2C instance to which the device is connected
 * \param deviceAddress I2C device address to initiate communications
 * \param addressing Device register addressing used (8bit/16bit)
 * \return Device for use with related I2C functions
 */
I2CDevice I2C_BindDevice(void* deviceRef, I2C* bus, const uint8_t deviceAddress, const I2CDeviceAddressing addressing);

/**
 * Read requested data. All data will only be available after the transaction is completed
 * \param i2c Bus to read buffered data from
 * \param data Destination where read data will be stored
 * \param size Number of bytes to read from buffered data (recommended to match requested transaction size)
 * \return Number of bytes read from buffered data
 */
size_t I2C_Read(const I2CDevice* device, void* data, const size_t size);

/**
 * Write data to a device on the bus
 * \param device Device to write to
 * \param data The data to write
 * \param size Number of bytes to write
 * \param complete_handler Called when transaction completes
 * \return True if transaction could be added to transaction list
 */
bool I2C_Write(const I2CDevice* device, const void* data, const size_t size, const I2C_CompleteHandler complete_handler);

/**
 * Write data to a specific memory address of a device on the bus
 * \param device Device to write to
 * \param address Device memory address to write data to
 * \param data The data to write
 * \param size Number of bytes to write
 * \param complete_handler Called when transaction completes
 * \return True if transaction could be added to transaction list
 */
bool I2C_WriteMem(const I2CDevice* device, const uint16_t address, const void* data, const size_t size, const I2C_CompleteHandler complete_handler);

/**
 * Issue a Request for data (Read) on the I2C bus
 * \param device Devices from which to request data
 * \param size Number of bytes to request/read
 * \param complete_handler Called when transaction completes
 * \return True if transaction could be queued on the bus, false otherwise
 */
bool I2C_Request(const I2CDevice* device, const size_t size, const I2C_CompleteHandler complete_handler);

/**
 * Issue a Request for data (Read) from a specific memory address
 * \param device Devices from which to request data
 * \param address Device memory address to read from
 * \param size Number of bytes to request/read
 * \param complete_handler Called when transaction completes
 * \return True if transaction could be queued on the bus, false otherwise
 */
bool I2C_RequestMem(const I2CDevice* device, const uint16_t address, const size_t size, const I2C_CompleteHandler complete_handler);

/**
 * Check if Device's I2C bus is busy
 * \param device Device's bus to check
 * \return True if the I2C instance is busy, False if no transactions are pending
 */
bool I2C_IsBusy(const I2CDevice* device);

/**
 * Retrieves the status of the I2C bus, updated after each transaction
 * \param device Device's bus status to retrieve
 * \return Current bus status
 */
I2CStatus I2C_GetStatus(const I2CDevice* device);

#endif
