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
	UART_PARITY_NONE, /** No parity */
	UART_PARITY_ODD,  /** Odd parity */
	UART_PARITY_EVEN, /** Even parity */
} UARTParity;

/**
 * Available hardware UART instances
 */
typedef enum _UARTInstance_
{
	UART0,
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
 * Initialize UART with data buffers
 * \param uart UART instance initialize
 * \param rxBuff Receive storage buffer
 * \param rxSize Size of the receive buffer (bytes)
 * \param txBuff Transmit storage buffer
 * \param txSize Size of the transmit buffer (bytes)
 */
void UART_Init(UART* uart, void* rxBuff, const size_t rxSize, void* txBuff, const size_t txSize);

/**
 * Enables UART for operation
 * \param uart UART to enable
 */
void UART_Enable(const UART* uart);

/**
 * Disables UART operation
 * \param uart UART to disable
 */
void UART_Disable(const UART* uart);

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
