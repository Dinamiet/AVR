#include "structure.h"
#include "uart.h"

#include <avr/io.h>

size_t UART_Write(UART* uart, void* _data, size_t size)
{
	uint8_t* data    = _data;
	size_t   written = 0;

	uart->Registers->StatusB &= (uint8_t) ~(1 << UDRE0); // Disable TX interrupt

	uint8_t* bufferElement = FifoBuffer_Add(&uart->TxBuffer);
	while (bufferElement)
	{
		*bufferElement = data[written++];
		if (written >= size)
			break;
		bufferElement = FifoBuffer_Add(&uart->TxBuffer);
	}

	uart->Registers->StatusB |= 1 << UDRE0; // Enable TX interrupt

	return written;
}

size_t UART_Read(UART* uart, void* _data, size_t size)
{
	uint8_t* data = _data;
	size_t   read = 0;

	uart->Registers->StatusB &= (uint8_t) ~(1 << RXC0); // Disable RX interrupt

	uint8_t* bufferElement = FifoBuffer_Remove(&uart->RxBuffer);
	while (bufferElement)
	{
		data[read++] = *bufferElement;
		if (read >= size)
			break;
		bufferElement = FifoBuffer_Remove(&uart->RxBuffer);
	}

	uart->Registers->StatusB |= 1 << RXC0; // Enable RX interrupt

	return read;
}

bool UART_IsBusy(UART* uart) { return (uart->Registers->StatusB & (1 << UDRE0)); /* Active if TX interrupt is enabled */ }
