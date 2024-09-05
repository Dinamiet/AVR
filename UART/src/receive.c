#include "structure.h"
#include "uart.h"

#include <assert.h>
#include <avr/interrupt.h>
#include <avr/io.h>

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

// RX Ready interrupt handler
ISR(USART_RX_vect)
{
	UART*    uart = UART_GetInstance(UART0);
	uint8_t* data = FifoBuffer_Add(&uart->RxBuffer);
#ifdef UART_ENABLE_9BIT
	uint8_t* data9 = FifoBuffer_Add(&uart->RxBuffer);
	if (data9)
		*data9 = (uart->Registers->StatusB >> RXB80) & 0x01;
#endif
	uint8_t tmp = uart->Registers->Data; // Force read
	if (data)
		*data = tmp;
}
