#ifndef _UART_H_
#define _UART_H_

/**
 * \file
 * UART
 *
 * Non blocking UART
 */

#include <stdbool.h>
#include <stddef.h>

/**
 * Data parity options
 */
typedef enum _UARTParity_
{
	UART_PARITY_NONE = 0, /** No parity */
	UART_PARITY_EVEN = 2, /** Even parity */
	UART_PARITY_ODD  = 3, /** Odd parity */
} UARTParity;

/**
 * Available hardware UART instances
 */
typedef enum _UARTInstance_
{
	UART0, /** UART */
} UARTInstance;

/**
 * UART
 */
typedef struct _UART_ UART;

/**
 * Retrieves the provided hardware UART instance
 * \param instance The UART instance to return
 * \return Reference to UART instance
 */
UART* UART_GetInstance(const UARTInstance instance);

/**
 * Initialize and power-on UART hardware
 * \param uart UART to initialize
 */
void UART_Init(UART* uart);

/**
 * Deinitialize and power-off UART hardware
 * \param uart UART to deinitialize
 */
void UART_Deinit(UART* uart);

/**
 * Assign receive buffer to the UART
 * \param uart UART to assign buffer too
 * \param buff Buffer
 * \param size Size of the buffer (bytes)
 */
void UART_AssignRxBuffer(UART* uart, uint8_t* buff, const size_t size);

/**
 * Assign transmit buffer to the UART
 * \param uart UART to assign buffer too
 * \param buff Buffer
 * \param size Size of the buffer (bytes)
 */
void UART_AssignTxBuffer(UART* uart, uint8_t* buff, const size_t size);

/**
 * Sets the UART BAUD rate to the specified rate
 * \param uart UART to configure
 * \param baud BAUD rate to configure
 */
void UART_SetBaud(const UART* uart, const uint32_t baud);

/**
 * Sets UART data format
 * \param uart UART format to configure
 * \param dataBits Data bits
 * \param parity Parity configuration
 * \param stopBits Stop bits
 */
void UART_SetFormat(const UART* uart, const size_t dataBits, const UARTParity parity, const size_t stopBits);

/**
 * Write/transmit data to UART
 * \param uart UART to transmit data
 * \param data Data buffer to transmit
 * \param size Size of buffer/data to transmit (bytes)
 * \return Number of bytes queued for transmission
 */
size_t UART_Write(UART* uart, const void* data, const size_t size);

/**
 * Read received data
 * \param uart UART to check for received data
 * \param data Storage buffer for received data
 * \param size Maximum number of bytes to receive and place in buffer
 * \return Number of bytes written to data
 */
size_t UART_Read(UART* uart, void* data, const size_t size);

/**
 * Checks if UART is busy transmitting data
 * \param uart UART to check for transmission activity
 * \return True if busy transmitting data, otherwise false
 */
bool UART_IsBusy(const UART* uart);

#endif
