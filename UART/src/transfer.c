#include "structure.h"
#include "uart.h"

#include <assert.h>
#include <avr/interrupt.h>

size_t UART_Write(UART* uart, const void* _data, const size_t size)
{
	assert(uart != NULL);
	assert(_data != NULL);

	const uint8_t* data    = _data;
	size_t         written = 0;

	uart->Registers->StatusB &= ~(1 << UDRIE0); // Disable TX interrupt

	uint8_t* bufferElement = FifoBuffer_Add(&uart->TxBuffer);
	while (bufferElement)
	{
		*bufferElement = data[written++];
		if (written >= size)
			break;
		bufferElement = FifoBuffer_Add(&uart->TxBuffer);
	}

	uart->Registers->StatusB |= 1 << UDRIE0; // Enable TX interrupt

	return written;
}

size_t UART_Read(UART* uart, void* _data, const size_t size)
{
	assert(uart != NULL);
	assert(_data != NULL);

	uint8_t* data = _data;
	size_t   read = 0;

	uart->Registers->StatusB &= ~(1 << RXCIE0); // Disable RX interrupt

	uint8_t* bufferElement = FifoBuffer_Remove(&uart->RxBuffer);
	while (bufferElement)
	{
		data[read++] = *bufferElement;
		if (read >= size)
			break;
		bufferElement = FifoBuffer_Remove(&uart->RxBuffer);
	}

	uart->Registers->StatusB |= 1 << RXCIE0; // Enable RX interrupt

	return read;
}

bool UART_IsBusy(const UART* uart)
{
	assert(uart != NULL);

	return (uart->Registers->StatusB & (1 << UDRIE0)); // Active if TX interrupt is enabled
}
