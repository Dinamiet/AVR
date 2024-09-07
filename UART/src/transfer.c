#include "structure.h"
#include "uart.h"

#include <assert.h>
#include <avr/interrupt.h>

size_t UART_Write(UART* uart, const void* data, const size_t size)
{
	assert(uart != NULL);
	assert(_data != NULL);

	uart->Registers->StatusB &= ~(1 << UDRIE0); // Disable TX interrupt

	size_t written = FifoBuffer_Add(&uart->TxBuffer, data, size);

	uart->Registers->StatusB |= 1 << UDRIE0; // Enable TX interrupt

	return written;
}

size_t UART_Read(UART* uart, void* data, const size_t size)
{
	assert(uart != NULL);
	assert(_data != NULL);

	uart->Registers->StatusB &= ~(1 << RXCIE0); // Disable RX interrupt

	size_t read = FifoBuffer_Remove(&uart->RxBuffer, data, size);

	uart->Registers->StatusB |= 1 << RXCIE0; // Enable RX interrupt

	return read;
}

bool UART_IsBusy(const UART* uart)
{
	assert(uart != NULL);

	return (uart->Registers->StatusB & (1 << UDRIE0)); // Active if TX interrupt is enabled
}
